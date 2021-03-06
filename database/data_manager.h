//
// Created by David Seery on 11/08/2015.
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

#ifndef LSSEFT_DATA_MANAGER_H
#define LSSEFT_DATA_MANAGER_H


#include <memory>

#include "tokens.h"
#include "transaction_manager.h"
#include "z_database.h"
#include "k_database.h"
#include "IR_cutoff_database.h"
#include "UV_cutoff_database.h"
#include "IR_resum_database.h"

#include "cosmology/types.h"
#include "cosmology/FRW_model.h"
#include "cosmology/Pk_filter.h"
#include "cosmology/oneloop_momentum_integrator.h"
#include "cosmology/Matsubara_XY_calculator.h"
#include "cosmology/oneloop_growth_integrator.h"
#include "cosmology/concepts/transfer_function.h"
#include "cosmology/concepts/oneloop_growth.h"
#include "cosmology/concepts/range.h"
#include "cosmology/concepts/power_spectrum.h"
#include "cosmology/concepts/loop_integral.h"

#include "sqlite3_detail/sqlite3_policy.h"
#include "sqlite3_detail/operations.h"

#include "utilities/formatter.h"

#include "controller/argument_cache.h"

#include "boost/filesystem/operations.hpp"

#include "sqlite3.h"


// clearances used when filtering an input power spectrum
// the filtered power spectrum can't cover the same range as the input power spectrum, because
// by construction the filter needs to look at a range of the input spectrum.
// By default we allow 25% margin at both top and bottom in which to fit the filter window.
constexpr double FILTER_PK_DEFAULT_TOP_CLEARANCE    = 0.75;
constexpr double FILTER_PK_DEFAULT_BOTTOM_CLEARANCE = 1.25;


class data_manager
  {

    // CONSTRUCTOR, DESTRUCTOR

  public:

    //! constructor opens SQLite3 handle, and creates tables if this is a new database
    data_manager(const boost::filesystem::path& c, error_handler& e, const argument_cache& ac);

    //! destructor closes SQLite3 handle
    ~data_manager();


    // GENERATE WAVENUMBER AND REDSHIFT DATABASES

  public:

    //! generate redshift database from a set of samples
    std::unique_ptr<z_database> build_redshift_db(range<double>& sample);

    //! generate wavenumber database from a set of samples
    std::unique_ptr<k_database> build_k_db(range<Mpc_units::energy>& sample);
    
    //! generate wavenumber database from a linear power spectrum container
    //! only contains wavenumbers that can actually be evaluated by the container
    template <typename PkContainer>
    std::unique_ptr<k_database> build_k_db(transaction_manager& mgr, const PkContainer& Pk_lin,
                                           double bottom_clearance=SPLINE_PK_DEFAULT_BOTTOM_CLEARANCE,
                                           double top_clearance=SPLINE_PK_DEFAULT_TOP_CLEARANCE);

    //! generate IR cutoff database from a set of samples
    std::unique_ptr<IR_cutoff_database> build_IR_cutoff_db(range<Mpc_units::energy>& sample);

    //! generate UV cutoff database from a set of samples
    std::unique_ptr<UV_cutoff_database> build_UV_cutoff_db(range<Mpc_units::energy>& sample);
    
    //! generate IR resummation scale database from a set of samples
    std::unique_ptr<IR_resum_database> build_IR_resum_db(range<Mpc_units::energy>& sample);

  protected:

    template <typename Token>
    std::unique_ptr< wavenumber_database<Token> > build_wavenumber_db(range<Mpc_units::energy>& sample);


    // SERVICES

  public:

    //! build a work list representing z-values that are missing from the SQLite backing store
    //! for each k-value in a wavenumber database representing transfer functions.
    //! generates a new transaction on the database; will fail if a transaction is in progress
    std::unique_ptr<transfer_work_list>
    build_transfer_work_list(const FRW_model_token& model, k_database& k_db, z_database& z_db);

    //! build a work list representing z-values that are missing from the SQLite backing store
    //! for each z-value needed for the one-loop growth factors.
    //! generates a new transaction on the database; will fail if a transaction is in progress
    std::unique_ptr<z_database>
    build_loop_growth_work_list(const FRW_model_token& model, z_database& z_db, const growth_params_token& params);

    //! build a work list representing k-values that are missing from the SQLite backing store
    //! for each k-value in a wavenumber database representing the momentum loop integral.
    //! generates a new transaction on the database; will fail if a transaction is in progress
    std::unique_ptr<loop_integral_work_list>
    build_loop_momentum_work_list(const FRW_model_token& model, k_database& k_db, IR_cutoff_database& IR_db,
                                  UV_cutoff_database& UV_db, std::shared_ptr<initial_filtered_Pk>& Pk,
                                  const loop_integral_params_token& params_tok, const loop_integral_params& params);
    
    //! build a work list representing (k, z, IR, UV) combinations of the one-loop power spectra
    //! that are missing from the SQLite backing store.
    //! generates a new transaction on the database; will fail if a transaction is in progress
    std::unique_ptr<one_loop_Pk_work_list>
    build_one_loop_Pk_work_list(const FRW_model_token& model, const growth_params_token& growth_params,
                                const loop_integral_params_token& loop_params, z_database& z_db,
                                k_database& k_db, IR_cutoff_database& IR_db, UV_cutoff_database& UV_db,
                                std::shared_ptr<initial_filtered_Pk>& Pk_init,
                                std::shared_ptr<final_filtered_Pk>& Pk_final);

    //! build a work list representing (k, z, IR_cutoff, UV_cutoff, IR_resum) combinations of the one-loop
    //! multipole power spectra that are missing from the SQLite backing store.
    //! generates a new transaction on the database; will fail if a transaction is in progress
    std::unique_ptr<multipole_Pk_work_list>
    build_multipole_Pk_work_list(const FRW_model_token& model, const growth_params_token& growth_params,
                                 const loop_integral_params_token& loop_params,
                                 const MatsubaraXY_params_token& XY_params, z_database& z_db,
                                 k_database& k_db, IR_cutoff_database& IR_cutoff_db,
                                 UV_cutoff_database& UV_cutoff_db, IR_resum_database& IR_resum_db,
                                 std::shared_ptr<initial_filtered_Pk>& Pk_init,
                                 std::shared_ptr<final_filtered_Pk>& Pk_final);
    
    //! build a work list representing data for calculation of the Matsubara- X & Y coefficients
    std::unique_ptr<Matsubara_XY_work_list>
    build_Matsubara_XY_work_list(const FRW_model_token& model_tok, IR_resum_database& IR_resum_db,
                                 std::shared_ptr<initial_filtered_Pk>& Pk, const MatsubaraXY_params_token& XY_tok,
                                 const MatsubaraXY_params& params);
    
    //! build a work list representing k-modes for which we need to produce a filtered wiggle/no-wiggle power spectrum
    std::unique_ptr<filter_Pk_work_list>
    build_filter_Pk_work_list(const linear_Pk_token& Pk_token, std::shared_ptr<filterable_Pk>& Pk_lin,
                              const filter_params_token& filter_token, const Pk_filter_params& params);

    //! build a work list representing counterterms which need to be computed;
    //! at one-loop the UV and IR cutoffs are not needed, but they're included for future compatibility
    //! with >= two-loop calculations in which they might be required
    std::unique_ptr<counterterm_work_list>
    build_counterterm_work_list(const FRW_model_token& model, const growth_params_token& growth_params,
                                const MatsubaraXY_params_token& XY_params, z_database& z_db, k_database& k_db,
                                IR_cutoff_database& IR_cutoff_db, UV_cutoff_database& UV_cutoff_db,
                                IR_resum_database& IR_resum_db, std::shared_ptr<initial_filtered_Pk>& Pk_init,
                                std::shared_ptr<final_filtered_Pk>& Pk_final);

    //! exchange a linear power spectrum container for a wiggle-Pk container
    template <typename PkContainer>
    std::unique_ptr<typename PkContainer::filtered_Pk_type> build_wiggle_Pk(const linear_Pk_token& token, const PkContainer& Pk_lin);
    
    //! compute how to rescale a final power spectrum to the same amplitude as an initial power spectrum
    template <typename PkContainer>
    PkContainer& rescale_final_Pk(const FRW_model_token& model, const growth_params_token& params, PkContainer& Pk,
                                  const z_database& z_db);
    
  protected:
    
    //! tensor together (k, IR cutoff, UV cutoff) combinations for loop integrals
    loop_configs tensor_product(k_database& k_db, IR_cutoff_database& IR_db, UV_cutoff_database& UV_db);
    
    //! tensor together (k, IR cutoff, UV cutoff, IR resummation scale) combinations for loop integrals
    resum_Pk_configs tensor_product(k_database& k_db, IR_cutoff_database& IR_cutoff_db, UV_cutoff_database& UV_cutoff_db,
                                    IR_resum_database& IR_resum_db);


    // TOKENS
    // tokens are the basic unit of currency used when interacting with the database

  public:
    
    //! tokenize an FRW model
    //! generates a new transaction on the database; will fail if a transaction is in progress
    std::unique_ptr<FRW_model_token> tokenize(const FRW_model& obj);

    //! tokenize a redshift
    //! generates a new transaction on the database; will fail if a transaction is in progress
    std::unique_ptr<z_token> tokenize(double z);

    //! tokenize a wavenumber of the type specified in the template
    //! generates a new transaction on the database; will fail if a transaction is in progress
    template <typename Token>
    std::unique_ptr<Token> tokenize(const Mpc_units::energy& k);
    
    //! tokenize a linear power spectrum
    //! generates a new transaction on the database; will fail if a transaction is in progress
    template <typename PkContainer>
    std::unique_ptr<linear_Pk_token> tokenize(const FRW_model_token& model, const PkContainer& Pk_lin);
    
    //! tokenize a set of filtering parameters
    //! generates a new transaction on the database; will fail if a transaction is in progress
    std::unique_ptr<filter_params_token> tokenize(const Pk_filter_params& data);
    
    //! tokenize a set of one-loop momentum integral parameters
    //! generates a new transaction on the database; will fail if a transaction is in progress
    std::unique_ptr<loop_integral_params_token> tokenize(const loop_integral_params& data);
    
    //! tokenize a set of Matsubara XY integral parameters
    //! generates a new transaction on the database; will fail if a transaction is in progress
    std::unique_ptr<MatsubaraXY_params_token> tokenize(const MatsubaraXY_params& data);
    
    //! tokenize a set of growth function parameters
    //! generates a new transaction on the database; will fail if a transaction is in progress
    std::unique_ptr<growth_params_token> tokenize(const growth_params& data);
  
  protected:
    
    //! tokenize an FRW model
    std::unique_ptr<FRW_model_token> tokenize(transaction_manager& mgr, const FRW_model& obj);
    
    //! tokenize a redshift
    std::unique_ptr<z_token> tokenize(transaction_manager& mgr, double z);
    
    //! tokenize a wavenumber of the type specified in the template
    template <typename Token>
    std::unique_ptr<Token> tokenize(transaction_manager& mgr, const Mpc_units::energy& k);
    
    //! tokenize a linear power spectrum
    template <typename PkContainer>
    std::unique_ptr<linear_Pk_token> tokenize(transaction_manager& mgr, const FRW_model_token& model, const PkContainer& Pk_lin);
    
    //! tokenize a set of filtering parameters
    std::unique_ptr<filter_params_token> tokenize(transaction_manager& mgr, const Pk_filter_params& data);
    
    //! tokenize a set of one-loop momentum integral parameters
    std::unique_ptr<loop_integral_params_token> tokenize(transaction_manager& mgr, const loop_integral_params& data);
    
    //! tokenize a set of Matsubara XY integral parameters
    std::unique_ptr<MatsubaraXY_params_token> tokenize(transaction_manager& mgr, const MatsubaraXY_params& data);
    
    //! tokenize a set of growth function parameters
    std::unique_ptr<growth_params_token> tokenize(transaction_manager& mgr, const growth_params& data);
    
    
    // DATABASE SERVICES -- WRITE PREPARATION
    
  public:
    
    //! prepare to write to the growth-factor tables
    void setup_growth_write();
    
    //! prepare to write to the transfer function table
    void setup_write(transfer_work_list& work);
    
    //! prepare to write to the loop integral table
    void setup_write(loop_integral_work_list& work);
    
    //! prepare to write to the filtered Pk table
    void setup_write(filter_Pk_work_list& work);
    
    //! prepare to write to the Matsubara XY table
    void setup_write(Matsubara_XY_work_list& work);
    
    //! prepare to write to the one-loop power spectrum table
    void setup_write(one_loop_Pk_work_list& work);

    //! prepare to write to the multipole power spectrum table
    void setup_write(multipole_Pk_work_list& work);

    //! prepare to write to the counterterm table
    void setup_write(counterterm_work_list& work);
    
    
    // DATABASE SERVICES -- CLOSE DOWN AFTER WRITE
  
  public:
    
    //! finish writing to the growth-factor tables
    void finalize_growth_write();
    
    //! finish writing to the transfer function table
    void finalize_write(transfer_work_list& work);
    
    //! finish writing to the loop integral table
    void finalize_write(loop_integral_work_list& work);
    
    //! finish writing to the filtered Pk table
    void finalize_write(filter_Pk_work_list& work);
    
    //! finish writing to the Matsubara XY table
    void finalize_write(Matsubara_XY_work_list& work);
    
    //! finish writing to the one-loop power spectrum table
    void finalize_write(one_loop_Pk_work_list& work);

    //! finish writing to the multipole power spectrum table
    void finalize_write(multipole_Pk_work_list& work);

    //! finish writing to the counterterm table
    void finalize_write(counterterm_work_list& work);
    

    // DATA STORAGE

  public:

    //! store a sample of some kind (the exact behaviour is determined by template specialization)
    //! generates a new transaction on the database; will fail if a transaction is in progress
    template <typename SampleType>
    void store(const FRW_model_token& model, const SampleType& sample);
    
    
    // DATA EXTRACTION
    
  protected:
    
    //! extract a sample of a z-dependent but not k-dependent quantity, of the type specified by
    //! the payload
    template <typename PayloadType>
    std::unique_ptr<oneloop_growth>
    find(transaction_manager& mgr, const FRW_model_token& model, const growth_params_token& params,
             const z_database& z_db);
    
    //! extract a sample of a power spectrum-like quantity that depends on k but not z
    template <typename PayloadType>
    std::unique_ptr<PayloadType> find(transaction_manager& mgr, const linear_Pk_token& token, const k_database& k_db);
    
    //! extract a sample of a loop integral-like quantity that is k-dependent, UV and IR cutoff-dependent
    //! but not z-dependent
    template <typename PayloadType>
    std::unique_ptr<loop_integral>
    find(transaction_manager& mgr, const FRW_model_token& model, const loop_integral_params_token& params,
         const k_token& k, const linear_Pk_token& Pk, const IR_cutoff_token& IR_cutoff,
         const UV_cutoff_token& UV_cutoff);
    
    //! extract a sample of a P(k)-like quantity that is k-dependent, z-dependent,
    //! and IR/UV-cutoff dependent
    template <typename PayloadType>
    std::unique_ptr<oneloop_Pk_set>
    find(transaction_manager& mgr, const FRW_model_token& model, const growth_params_token& growth_params,
         const loop_integral_params_token& loop_params, const k_token& k, const z_token& z,
         const linear_Pk_token& Pk_init, const boost::optional<linear_Pk_token>& Pk_final,
         const IR_cutoff_token& IR_cutoff, const UV_cutoff_token& UV_cutoff);
    
    //! extract a quantity of a IR-resummation-scale dependent quantity
    template <typename PayloadType>
    std::unique_ptr<Matsubara_XY>
    find(transaction_manager& mgr, const FRW_model_token& model, const MatsubaraXY_params_token& params,
         const linear_Pk_token& Pk, const IR_resum_token& IR_resum);

    
    // TRANSACTIONS

  protected:

    //! open a transaction; throws an exception if a transaction is already held open
    //! note we have to use a std::shared_ptr<> here, rather than a std::unique_ptr<>,
    //! because we hold a std::weak_ptr<> internally to keep track of whether a transaction is open
    std::shared_ptr<transaction_manager> open_transaction();

    //! begin a new transaction on the database
    void begin_transaction();

    //! commit a transaction on the database
    void commit_transaction();

    //! rollback a transaction on the database
    void rollback_transaction();

    //! release a transaction
    void release_transaction();


    // LOOKUP OR INSERT RECORDS

  protected:

    //! lookup or insert a new FRW model
    unsigned int lookup_or_insert(transaction_manager& mgr, const FRW_model &obj);

    //! lookup or insert a redshift
    unsigned int lookup_or_insert(transaction_manager& mgr, double z);

    //! lookup or insert a wavenumber
    template <typename Token>
    unsigned int lookup_or_insert(transaction_manager& mgr, const Mpc_units::energy &k);
    
    //! lookup or insert a linear power spectrum identifier
    template <typename PkContainer>
    unsigned int lookup_or_insert(transaction_manager& mgr, const FRW_model_token& model, const PkContainer& Pk_lin);
    
    //! lookup or insert a filtering parameter set
    unsigned int lookup_or_insert(transaction_manager& mgr, const Pk_filter_params& data);
    
    //! lookup or insert a one-loop momentum integral parameter set
    unsigned int lookup_or_insert(transaction_manager& mgr, const loop_integral_params& data);
    
    //! lookup or insert a Matsubara X & Y integral parameter set
    unsigned int lookup_or_insert(transaction_manager& mgr, const MatsubaraXY_params& data);
    
    //! lookup or insert a growth function parameter set
    unsigned int lookup_or_insert(transaction_manager& mgr, const growth_params& data);
    
    // INTERNAL DATA

  private:

    //! path to data container
    boost::filesystem::path container;

    //! SQLite3 handler for container
    sqlite3* handle;


    // TRANSACTIONS

    //! current transaction manager, if one exists
    std::weak_ptr<transaction_manager> current_transaction;


    // POLICIES

    //! arguments and global policies
    const argument_cache& arg_cache;
    
    //! SQLite3 policies
    const sqlite3_policy policy;
    
    
    // DELEGATES
    
    //! error handler agent
    error_handler& err_handler;


    // SEARCH TOLERANCES

    //! tolerance to use when searching for FRW model parameters
    double FRW_model_tol;

    //! tolerance to use when searching for redshift configurations
    double z_tol;

    //! tolerance to use when searching for wavenumber configurations
    double k_tol;
    
    //! tolerance to use when searching for filter-data configurations
    double filter_tol;
    
    //! tolerance to use when searching for one-loop momentum integral configurations
    double oneloop_tol;
    
    //! tolerance to use when searching for Matsubara X&Y integral configurations
    double MatsubaraXY_tol;
    
    //! tolerance to use when searching for growth-function parameter configurations
    double growth_tol;

  };


template <typename SampleType>
void data_manager::store(const FRW_model_token& model, const SampleType& sample)
  {
    // open a transaction on the database
    auto transaction = this->open_transaction();

    sqlite3_operations::store(this->handle, *transaction, this->policy, model, sample);

    // commit the transaction
    transaction->commit();
  }


template <typename Token>
std::unique_ptr< wavenumber_database<Token> > data_manager::build_wavenumber_db(range<Mpc_units::energy>& sample)
  {
    // construct an empty wavenumber database
    auto k_db = std::make_unique< wavenumber_database<Token> >();
    
    // grab the grid of wavenumber samples
    const auto& k_samples = sample.grid();
    
    for(auto t = k_samples.cbegin(); t != k_samples.cend(); ++t)
      {
        auto tok = this->tokenize<Token>(*t);
        k_db->add_record(*t, *tok);
      }
    
    return(k_db);
  }


template <typename PkContainer>
std::unique_ptr<k_database> data_manager::build_k_db(transaction_manager& mgr, const PkContainer& Pk_lin,
                                                     double bottom_clearance, double top_clearance)
  {
    // construct an empty wavenumber database
    auto k_db = std::make_unique<k_database>();
    
    // get power spectrum database underlying this container
    const auto& Pk_db = Pk_lin.get_db();
    
    for(auto t = Pk_db.record_cbegin(); t != Pk_db.record_cend(); ++t)
      {
        // ask initial_Pk container whether this P(k) value is acceptable
        const auto& k = t->get_wavenumber();
        if(Pk_lin.is_valid(k, bottom_clearance, top_clearance))
          {
            auto tok = this->tokenize<k_token>(mgr, k);
            k_db->add_record(k, *tok);
          }
      }
    
    return k_db;
  }


template <typename Token>
std::unique_ptr<Token> data_manager::tokenize(const Mpc_units::energy& k)
  {
    // open a new transaction on the database
    auto transaction = this->open_transaction();
    
    // lookup id for this wavenumber, or generate one if it does not already exist
    auto id = this->tokenize<Token>(*transaction, k);
    
    // commit the transaction
    transaction->commit();
    
    return std::move(id);
  }


template <typename Token>
std::unique_ptr<Token> data_manager::tokenize(transaction_manager& mgr, const Mpc_units::energy& k)
  {
    // lookup id for this wavenumber, or generate one if it does not already exist
    unsigned int id = this->lookup_or_insert<Token>(mgr, k);
    return std::make_unique<Token>(id);
  }


template <typename PkContainer>
std::unique_ptr<linear_Pk_token>
data_manager::tokenize(const FRW_model_token& model, const PkContainer& Pk_lin)
  {
    // open a new transaction on the database
    auto transaction = this->open_transaction();
    
    // lookup id for this power spectrum, or generate one if it doesn't already exist
    auto id = this->tokenize(*transaction, model, Pk_lin);
    
    // commit the transaction
    transaction->commit();
    
    return std::move(id);
  }


template <typename PkContainer>
std::unique_ptr<linear_Pk_token>
data_manager::tokenize(transaction_manager& mgr, const FRW_model_token& model, const PkContainer& Pk_lin)
  {
    // lookup id for this power spectrum, or generate one if it doesn't already exist
    unsigned int id = this->lookup_or_insert(mgr, model, Pk_lin);
    return std::make_unique<linear_Pk_token>(id);
  }


template <typename Token>
unsigned int data_manager::lookup_or_insert(transaction_manager& mgr, const Mpc_units::energy& k)
  {
    auto id = sqlite3_operations::lookup_wavenumber<Token>(this->handle, mgr, k, this->policy, this->k_tol);
    if(id) return(*id);
    
    return sqlite3_operations::insert_wavenumber<Token>(this->handle, mgr, k, this->policy);
  }


template <typename PkContainer>
unsigned int data_manager::lookup_or_insert(transaction_manager& mgr, const FRW_model_token& model, const PkContainer& Pk_lin)
  {
    auto id = sqlite3_operations::lookup_Pk_linear(this->handle, mgr, model, Pk_lin, this->policy);
    if(id) return(*id);
    
    return sqlite3_operations::insert_Pk_linear(this->handle, mgr, model, Pk_lin, this->policy);
  }


template <typename PkContainer>
std::unique_ptr<typename PkContainer::filtered_Pk_type> data_manager::build_wiggle_Pk(const linear_Pk_token& token, const PkContainer& Pk_lin)
  {
    // open a transaction on the database
    auto mgr = this->open_transaction();
    
    // extract database of wavenumber configurations from linear power spectrum container;
    // what we get is a database of k-values that can be evaluated by the container *given the specified clearances*
    // at both top and bottom.
    // Here we override the defaults (intended for use with splines) to allow clearance for the filtering
    // window.
    auto k_db = this->build_k_db(*mgr, Pk_lin, FILTER_PK_DEFAULT_BOTTOM_CLEARANCE, FILTER_PK_DEFAULT_TOP_CLEARANCE);
    
    // extract initial_filtered_Pk container
    auto payload = this->find<typename PkContainer::filtered_Pk_type>(*mgr, token, *k_db);
    
    // close transaction
    mgr->commit();
    
    return std::move(payload);
  }


template <typename PkContainer>
PkContainer& data_manager::rescale_final_Pk(const FRW_model_token& model, const growth_params_token& params, PkContainer& Pk,
                                            const z_database& z_db)
  {
    // open a transaction on the database
    auto mgr = this->open_transaction();
    
    // extract growth functions for the redshift database
    auto data = this->find<oneloop_growth>(*mgr, model, params, z_db);

    // assume that the "final" power spectrum is evaluated at the same time as the lowest redshift in the
    // z-database
    // The rescaling factor to give it the same amplitude as the initial power spectrum is that
    // [D(z_init)/D(z_final)]^2.
    oneloop_value z_init = *data->begin();
    oneloop_value z_final = *(--data->end());
    
    double rescale = z_init.second.D_lin / z_final.second.D_lin;
    
    // rescaling for power spectrum goes like the square of the growth factor
    Pk.set_rescaling(rescale*rescale);
    
    // close transaction
    mgr->commit();
    
    return Pk;
  }


#endif //LSSEFT_DATA_MANAGER_H
