//
// Created by David Seery on 07/12/2016.
// Copyright (c) 2016 University of Sussex. All rights reserved.
//

#include <cmath>

#include "Pk_filter.h"

#include "boost/optional.hpp"

#include "cuba.h"


namespace Pk_filter_impl
  {
    
    constexpr unsigned int dimensions = 2;       // Cuhre only works in >= 2 dimensions, not 1
    constexpr unsigned int components = 1;
    constexpr unsigned int points_per_invocation = 1;
    
    constexpr unsigned int verbosity_none = 0;
    constexpr unsigned int verbosity_reasonable = 1;
    constexpr unsigned int verbosity_progress = 2;
    constexpr unsigned int verbosity_subregions = 3;
    
    constexpr unsigned int samples_all = 0;
    constexpr unsigned int samples_last = 4;
    
    constexpr unsigned int min_eval = 0;
    constexpr unsigned int max_eval = 20000000;
    
    constexpr unsigned int ngiven = 0;
    constexpr unsigned int ldxgiven = 0;
    constexpr unsigned int nextra = 0;
    
    constexpr unsigned int pcores = 10000;   // matches default Cuba value
    
    constexpr unsigned int cuhre_key = 13;      // degree-13 only available in 2-dimensions
    constexpr unsigned int divonne_key1 = 47;
    constexpr unsigned int divonne_key2 = 13;      // degree-13 only available in 2-dimensions
    constexpr unsigned int divonne_key3 = 1;
    constexpr unsigned int divonne_maxpass = 5;
    constexpr unsigned int divonne_border = 0;
    constexpr double divonne_maxchisq = 10.0;
    constexpr double divonne_minchisq = 0.25;
    
    
    class integrand_data
      {
      
      public:
        
        integrand_data(double sl_min, double sl_max, double kl, double lm, const linear_Pk& _Pk, const approx_Pk& _Pka)
          : slog_min(sl_min),
            slog_max(sl_max),
            klog(kl),
            lambda(lm),
            Pk(_Pk),
            Pk_approx(_Pka),
            jacobian(slog_max - slog_min),
            slog_range(slog_max - slog_min)
          {
          }

        const double slog_min;
        const double slog_max;
        const double klog;
        const double lambda;

        const linear_Pk& Pk;
        const approx_Pk& Pk_approx;

        const double jacobian;
        const double slog_range;
      };
    
    
    static int integrand(const int* ndim, const cubareal* x, const int* ncomp, cubareal* f, void* userdata)
      {
        Pk_filter_impl::integrand_data* data = static_cast<Pk_filter_impl::integrand_data*>(userdata);
    
        double slog = data->slog_min + data->slog_range*x[0];
        Mpc_units::energy s = std::pow(10.0, slog) / Mpc_units::Mpc;
    
        f[0] = data->jacobian * (data->Pk(s) / data->Pk_approx(s)) * std::exp(-(data->klog-slog)*(data->klog-slog)/ (2.0*data->lambda*data->lambda));
    
        return(0);  // return value irrelevant unless = -999, which means stop integration
      }
    
  }   // namespace Pk_filter_impl


Mpc_units::inverse_energy3 Pk_filter::operator()(const FRW_model& model, const linear_Pk& Pk_lin, const Mpc_units::energy& k)
  {
    // build reference Eisenstein & Hu power spectrum
    std::unique_ptr<approx_Pk> Papprox = this->eisenstein_hu(model, Pk_lin);
    
    cubareal integral[Pk_filter_impl::dimensions];
    cubareal error[Pk_filter_impl::dimensions];
    cubareal prob[Pk_filter_impl::dimensions];
    
    int regions;
    int evaluations;
    int fail;
    
    // get maximum available scale from linear power spectrum
    constexpr double TEN_PERCENT_CLEARANCE_UPPER = 0.9;
    constexpr double TEN_PERCENT_CLEARANCE_LOWER = 1.1;
    const Mpc_units::energy k_min = TEN_PERCENT_CLEARANCE_LOWER * Pk_lin.get_db().get_k_min();
    const Mpc_units::energy k_max = TEN_PERCENT_CLEARANCE_UPPER * Pk_lin.get_db().get_k_max();

    const double klog = std::log10(k * Mpc_units::Mpc);
    
    const double slog_max = std::log10(k_max * Mpc_units::Mpc);
    const double slog_min = std::log10(k_min * Mpc_units::Mpc);

    double lambda = std::log10(0.25 * model.get_h());
    
    std::unique_ptr<Pk_filter_impl::integrand_data> data =
      std::make_unique<Pk_filter_impl::integrand_data>(slog_min, slog_max, klog, lambda, Pk_lin, *Papprox);
    
    Cuhre(Pk_filter_impl::dimensions,
          Pk_filter_impl::components,
          Pk_filter_impl::integrand, data.get(),
          Pk_filter_impl::points_per_invocation,
          this->rel_err, this->abs_err,
          Pk_filter_impl::verbosity_none | Pk_filter_impl::samples_last,
          Pk_filter_impl::min_eval, Pk_filter_impl::max_eval,
          Pk_filter_impl::cuhre_key,
          nullptr, nullptr,
          &regions, &evaluations, &fail,
          integral, error, prob);
    
    return Pk_lin(k) - (*Papprox)(k) * integral[0] / (std::sqrt(2.0*M_PI) * lambda);
  }


std::unique_ptr<approx_Pk> Pk_filter::eisenstein_hu(const FRW_model& model, const linear_Pk& Pk_lin)
  {
    // extract database of power spectrum sample points
    const tree_Pk::database_type& db = Pk_lin.get_db();
    
    // compute Eisenstein & Hu parameters
    double omega_m          = model.get_omega_m();
    double omega_cc         = model.get_omega_cc();
    double h                = model.get_h();
    double Neff             = model.get_Neff();
    Mpc_units::energy T_CMB = model.get_T_CMB();
    double f_baryon         = model.get_f_baryon();
    double z_star           = model.get_z_star();
    double z_drag           = model.get_z_drag();
    double z_eq             = model.get_z_eq();
    double A_curv           = model.get_A_curv();
    double ns               = model.get_ns();
    Mpc_units::energy k_piv = model.get_k_piv();
    
    constexpr double omega0 = 1.0;
    
    double Theta27 = T_CMB / Mpc_units::Kelvin / 2.7;

    double omega_b = f_baryon * omega_m;
    double omega_c = omega_m - omega_b;
    
    // from Einstein & Hu p.6
    double a1 = std::pow(46.9*omega0*h*h, 0.670) * (1.0 + std::pow(32.1*omega0*h*h, -0.532));
    double a2 = std::pow(12.0*omega0*h*h, 0.424) * (1.0 + std::pow(45.0*omega0*h*h, -0.582));
    double alpha_c = std::pow(a1, -omega_b/omega0) * std::pow(a2, -std::pow(omega_b/omega0, 3));
    
    double b1 = 0.944 / (1.0 + std::pow(458.0*omega0*h*h, -0.708));
    double b2 = std::pow(0.395*omega0*h*h, -0.0266);
    double beta_c = 1.0 / (1.0 + b1*(std::pow(omega_c/omega0, b2) - 1.0));

    Mpc_units::inverse_energy s = 44.5*std::log(9.83/(omega_m*h*h)) / std::sqrt(1.0 + 10.0*std::pow(omega_b*h*h, 3.0/4.0)) * Mpc_units::Mpc;

    approx_Pk::database_type approx_db;
    
    // normalization is adjusted so that we match the input power spectrum on the largest scale
    boost::optional<double> normalization = boost::none;

    for(tree_Pk::database_type::const_record_iterator t = db.record_cbegin(); t != db.record_cend(); ++t)
      {
        const Mpc_units::energy& k = t->get_wavenumber();
        double q = (k*h * Mpc_units::Mpc) * Theta27*Theta27 / (omega0*h*h);
        
        double f = 1.0 / (1.0 + std::pow(k*h*s / 5.4, 4));
    
        auto C = [=](double alpha) -> double { return 14.2 / alpha + 386.0 / (1.0 + 69.9*std::pow(q, 1.08)); };
    
        double C1 = C(1.0);
        double Calpha = C(alpha_c);
        
        auto T = [=](double C) -> double { return std::log(std::exp(1.0) + 1.8*beta_c*q) / (std::log(std::exp(1.0) + 1.8*beta_c*q) + C*q*q); };
        
        double Tc = f*T(C1) + (1.0-f)*T(Calpha);
        
        // power spectrum is proportional to k^4 T^2 P_Phi(k), but we don't have to get the constant
        // of proportionality correct since it will be adjusted below
        Mpc_units::inverse_energy3 Pk = Tc*Tc * std::pow(k*h / k_piv, ns-1.0) * k * Mpc_units::Mpc4;
        
        if(normalization)
          {
            // normalization has already been calculated
            Pk = *normalization * Pk;
          }
        else
          {
            // adjust normalization to match input power spectrum
            // (accounts for linear growth factor between Eisenstein & Hu formula which is valid at z=0,
            // and the input power spectrum which will be defined at z >> 1)
            normalization = t->get_Pk() / Pk;
            Pk = t->get_Pk();
          }
        
        approx_db.add_record(k, Pk);
      }
    
    return std::make_unique<approx_Pk>(approx_db);
  }
