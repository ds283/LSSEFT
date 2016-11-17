//
// Created by David Seery on 11/08/2015.
// Copyright (c) 2015 University of Sussex. All rights reserved.
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
#include "cosmology/concepts/transfer_function.h"
#include "cosmology/concepts/oneloop_growth.h"
#include "cosmology/concepts/range.h"
#include "cosmology/concepts/tree_power_spectrum.h"
#include "cosmology/concepts/loop_integral.h"

#include "sqlite3_detail/sqlite3_policy.h"
#include "sqlite3_detail/operations.h"

#include "boost/filesystem/operations.hpp"

#include "sqlite3.h"


class data_manager
  {

    // CONSTRUCTOR, DESTRUCTOR

  public:

    //! constructor opens SQLite3 handle, and creates tables if this is a new database
    data_manager(const boost::filesystem::path& c);

    //! destructor closes SQLite3 handle
    ~data_manager();


    // GENERATE WAVENUMBER AND REDSHIFT DATABASES

  public:

    //! generate redshift database from a set of samples
    std::unique_ptr<z_database> build_redshift_db(range<double>& sample);

    //! generate wavenumber database from a set of samples
    std::unique_ptr<k_database> build_k_db(range<Mpc_units::energy>& sample);

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
    std::unique_ptr<transfer_work_list> build_transfer_work_list(FRW_model_token& model, k_database& k_db,
                                                                 z_database& z_db);

    //! build a work list representing z-values that are missing from the SQLite backing store
    //! for each z-value needed for the one-loop growth factors.
    //! generates a new transaction on the database; will fail if a transaction is in progress
    std::unique_ptr<z_database> build_oneloop_work_list(FRW_model_token& model, z_database& z_db);

    //! build a work list representing k-values that are missing from the SQLite backing store
    //! for each k-value in a wavenumber database representing the momentum loop integral.
    //! generates a new transaction on the database; will fail if a transaction is in progress
    std::unique_ptr<loop_momentum_work_list> build_loop_momentum_work_list(FRW_model_token& model, k_database& k_db,
                                                                           IR_cutoff_database& IR_db, UV_cutoff_database& UV_db,
                                                                           std::shared_ptr<tree_power_spectrum>& Pk);
    
    //! build a work list representing (k, z) combinations that are missing from the SQLite backing store
    //! generates a new transaction on the database; will fail if a transaction is in progress
    std::unique_ptr<one_loop_Pk_work_list> build_one_loop_Pk_work_list(FRW_model_token& model,
                                                                       z_database& z_db, k_database& k_db,
                                                                       IR_cutoff_database& IR_db, UV_cutoff_database& UV_db,
                                                                       std::shared_ptr<tree_power_spectrum>& Pk);
    
  protected:
    
    //! tensor together (k, IR cutoff, UV cutoff) combinations for loop integrals
    loop_configs tensor_product(k_database& k_db, IR_cutoff_database& IR_db, UV_cutoff_database& UV_db);


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


    // DATA STORAGE

  public:

    //! store a sample of some kind (the exact behaviour is determined by template specialization)
    //! generates a new transaction on the database; will fail if a transaction is in progress
    template <typename SampleType>
    void store(const FRW_model_token& model, const SampleType& sample);
    
    
    // DATA EXTRACTION
    
  public:
    
    //! extract a sample of a z-dependent but not k-dependent quantity, of the type specified by
    //! the payload
    template <typename PayloadType>
    PayloadType find(transaction_manager& mgr, const FRW_model_token& model, z_database& z_db);
    
    //! extract a sample of a loop integral-like quantity that is k-dependent, UV and IR cutoff-dependent
    //! but not z-dependent
    template <typename PayloadType>
    PayloadType find(transaction_manager& mgr, const FRW_model_token& model, const k_token& k,
                     const IR_cutoff_token& IR_cutoff, const UV_cutoff_token& UV_cutoff);
    
    //! extract a sample of a P(k)-like quantity that is k-dependent, z-dependent,
    //! and IR/UV-cutoff dependent
    template <typename PayloadType>
    PayloadType find(transaction_manager& mgr, const FRW_model_token& model,
                     const k_token& k, const z_token& z,
                     const IR_cutoff_token& IR_cutoff, const UV_cutoff_token& UV_cutoff);

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


    // INTERNAL DATA

  private:

    //! path to data container
    boost::filesystem::path container;

    //! SQLite3 handler for container
    sqlite3* handle;


    // TRANSACTIONS

    //! current transaction manager, if one exists
    std::weak_ptr<transaction_manager> current_transaction;


    // SQLite3 policies

    //! sqlite3_policy object
    sqlite3_policy policy;


    // SEARCH TOLERANCES

    //! tolerance to use when searching for FRW model parameters
    double FRW_model_tol;

    //! tolerance to use when searching for redshift configurations
    double z_tol;

    //! tolerance to use when searching for wavenumber configurations
    double k_tol;

  };


template <typename SampleType>
void data_manager::store(const FRW_model_token& model, const SampleType& sample)
  {
    // open a transaction on the database
    std::shared_ptr<transaction_manager> transaction = this->open_transaction();

    sqlite3_operations::store(this->handle, *transaction, this->policy, model, sample);

    // commit the transaction
    transaction->commit();
  }


#endif //LSSEFT_DATA_MANAGER_H
