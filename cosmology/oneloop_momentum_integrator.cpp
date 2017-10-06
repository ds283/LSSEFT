//
// Created by David Seery on 21/11/2015.
// --@@ // Copyright (c) 2017 University of Sussex. All rights reserved.
//
// This file is part of the Sussex Effective Field Theory for
// Large-Scale Structure platform (LSSEFT).
//
// LSSEFT is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 2 of the License, or
// (at your option) any later version.
//
// LSSEFT is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with LSSEFT.  If not, see <http://www.gnu.org/licenses/>.
//
// @license: GPL-2
// @contributor: David Seery <D.Seery@sussex.ac.uk>
// --@@
//


#include <iostream>
#include <fstream>

#include "oneloop_momentum_integrator.h"
#include "oneloop_integrands/integrands.h"

#include "cuba.h"

#include "boost/timer/timer.hpp"



oneloop_momentum_integrator::oneloop_momentum_integrator(const loop_integral_params& p, error_handler& e)
  : params(p),
    err_handler(e)
  {
    // seed random number generator
    mersenne_twister.seed(random_device());
  }


loop_integral
oneloop_momentum_integrator::integrate(const FRW_model& model, const loop_integral_params_token& params_tok, const Mpc_units::energy& k,
                                       const k_token& k_tok, const Mpc_units::energy& UV_cutoff, const UV_cutoff_token& UV_tok,
                                       const Mpc_units::energy& IR_cutoff, const IR_cutoff_token& IR_tok, const initial_filtered_Pk& Pk)
  {

#include "autogenerated/integrate_stmts.cpp"

    loop_integral container(k_tok, params_tok, Pk.get_token(), UV_tok, IR_tok, ker);

    return container;
  }


template <typename KernelRecord>
bool oneloop_momentum_integrator::kernel_integral(const FRW_model& model, const Mpc_units::energy& k,
                                                  const Mpc_units::energy& UV_cutoff,
                                                  const Mpc_units::energy& IR_cutoff, const initial_filtered_Pk& Pk,
                                                  integrand_t integrand, KernelRecord& result, loop_integral_type type,
                                                  const std::string& name)
  {
    // disable CUBA's internal auto-parallelization
    // we're handling multiprocessor activity ourselves via the scheduler,
    // so it's preferable to keep each core fully active rather than have threads
    // trying to manage Cuba's subworkers
    cubacores(0, oneloop_momentum_impl::pcores);
    
    wiggle_Pk_raw_adapter raw(Pk, IR_cutoff, UV_cutoff);
    wiggle_Pk_nowiggle_adapter nw(Pk, IR_cutoff, UV_cutoff);
    
    bool fail_raw = this->evaluate_integral(model, k, UV_cutoff, IR_cutoff, raw, integrand, result.get_raw(), type, name, "raw");
    bool fail_nw  = this->evaluate_integral(model, k, UV_cutoff, IR_cutoff, nw, integrand, result.get_nowiggle(), type, name, "no-wiggle");
    
    return fail_raw || fail_nw;
  }


template <typename IntegralRecord>
bool oneloop_momentum_integrator::evaluate_integral(const FRW_model& model, const Mpc_units::energy& k,
                                                    const Mpc_units::energy& UV_cutoff,
                                                    const Mpc_units::energy& IR_cutoff, const spline_Pk& Pk,
                                                    integrand_t integrand, IntegralRecord& result,
                                                    loop_integral_type type, const std::string& name,
                                                    const std::string& component)
  {
    cubareal integral[oneloop_momentum_impl::dimensions];
    cubareal error[oneloop_momentum_impl::dimensions];
    cubareal prob[oneloop_momentum_impl::dimensions];
    
    int regions;
    int evaluations;
    int fail;

    boost::timer::cpu_timer raw_timer;
    
    auto data = std::make_unique<oneloop_momentum_impl::integrand_data>(model, k, UV_cutoff, IR_cutoff, Pk);
    
    double re = (type == loop_integral_type::P13 ? this->params.get_relerr_13() : this->params.get_relerr_22());
    double ae = (type == loop_integral_type::P13 ? this->params.get_abserr_13() : this->params.get_abserr_22());

    constexpr unsigned int MAX_13_TRIES = 5;
    constexpr unsigned int MAX_22_TRIES = 3;
    unsigned int max_tries = (type == loop_integral_type::P13 ? MAX_13_TRIES : MAX_22_TRIES);
    unsigned int tries = 0;
    
    while(tries < max_tries)
      {
        if(tries > 0)
          {
            re = re * 4.0;
            std::ostringstream msg;
            msg << "relaxing error tolerance for kernel = " << name << " (" << component << "), attempt "
                << tries << ", now abstol = " << ae << ", reltol = " << re;
            this->err_handler.info(msg.str());
          }

        Cuhre(oneloop_momentum_impl::dimensions,
              oneloop_momentum_impl::components,
              integrand, data.get(),
              oneloop_momentum_impl::points_per_invocation,
              re, ae,
              oneloop_momentum_impl::verbosity_none | oneloop_momentum_impl::samples_last,
              oneloop_momentum_impl::min_eval, oneloop_momentum_impl::max_eval,
              oneloop_momentum_impl::cuhre_key,
              nullptr, nullptr,
              &regions, &evaluations, &fail,
              integral, error, prob);
        
        if(fail == 0)
          {
            raw_timer.stop();
    
            // write values into return structure
            result.value = typename IntegralRecord::value_type(integral[0]);
            result.regions = regions;
            result.evaluations = evaluations;
            result.error = typename IntegralRecord::value_type(error[0]);
            result.time = raw_timer.elapsed().wall;
            
            break;
          }
        
        ++tries;
      }
    
    if(tries >= max_tries)
      {
        std::ostringstream msg;
        msg << "integration failure: kernel = " << name << " (" << component << "), "
            << "regions = " << regions << ", evaluations = " << evaluations << ", fail = "
            << fail << ", value = " << integral[0] << ", error = " << error[0] << ", probability = " << prob[0];
        this->err_handler.warn(msg.str());
      }
    
    
    return (tries >= max_tries);
  }

// Alternative Divonne integrator

//    Divonne(oneloop_momentum_impl::dimensions, oneloop_momentum_impl::components,
//            integrand, data.get(),
//            oneloop_momentum_impl::points_per_invocation,
//            this->rel_err, this->abs_err,
//            oneloop_momentum_impl::verbosity_none | oneloop_momentum_impl::samples_last,
//            this->mersenne_twister(),                                                          // seed for internal Cuba random number generator
//            oneloop_momentum_impl::min_eval, oneloop_momentum_impl::max_eval,
//            oneloop_momentum_impl::divonne_key1, oneloop_momentum_impl::divonne_key2, oneloop_momentum_impl::divonne_key3,
//            oneloop_momentum_impl::divonne_maxpass,
//            oneloop_momentum_impl::divonne_border, oneloop_momentum_impl::divonne_maxchisq, oneloop_momentum_impl::divonne_minchisq,
//            oneloop_momentum_impl::ngiven, oneloop_momentum_impl::ldxgiven, nullptr,
//            oneloop_momentum_impl::nextra, nullptr,
//            nullptr, nullptr,
//            &regions, &evaluations, &fail,
//            integral, error, prob);


