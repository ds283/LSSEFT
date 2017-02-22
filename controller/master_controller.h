//
// Created by David Seery on 10/08/2015.
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

#ifndef LSSEFT_MASTER_CONTROLLER_H
#define LSSEFT_MASTER_CONTROLLER_H


#include <memory>

#include "argument_cache.h"
#include "local_environment.h"
#include "scheduler.h"

#include "database/data_manager.h"
#include "database/tokens.h"

#include "cosmology/types.h"
#include "cosmology/FRW_model.h"

#include "error/error_handler.h"

#include "boost/mpi.hpp"


class master_controller
  {

    // CONSTRUCTOR, DESTRUCTOR

  public:

    //! construct a master controller delegate
    master_controller(boost::mpi::environment& me, boost::mpi::communicator& mw, argument_cache& ac);

    //! destructor is default
    ~master_controller() = default;


    // INTERFACE

  public:

    //! process command-line arguments
    void process_arguments(int argc, char* argv[]);

    //! execute
    void execute();


    // RANK TO WORKER NUMBER CONVERSIONS (worker number runs from 1 .. n-1, rank runs from 1 .. n, based on master process on rank 0)

  protected:

    //! Get worker number
    unsigned int worker_number() { return(static_cast<unsigned int>(this->mpi_world.rank()-1)); }

    //! Return MPI rank of this process
    unsigned int get_rank(void) const { return(static_cast<unsigned int>(this->mpi_world.rank())); }

    //! Map worker number to communicator rank
    unsigned int worker_rank(unsigned int worker_number) const { return(worker_number+1); }

    //! Map communicator rank to worker number
    unsigned int worker_number(unsigned int worker_rank) const { return(worker_rank-1); }


    // WORKER HANDLING

  protected:

    //! execute a job specified by a work list
    template <typename WorkItemList>
    void scatter(const FRW_model& model, const FRW_model_token& token, WorkItemList& work, data_manager& dmgr);

    //! store a payload returned by a worker
    template <typename WorkItem>
    void store_payload(const FRW_model_token& token, unsigned int source, data_manager& dmgr);

    //! terminate worker processes
    void terminate_workers();

    //! instruct workers to await new tasks
    std::unique_ptr<scheduler> set_up_workers(unsigned int tag);

    //! instruct workers that the current batch of tasks is finished
    void close_down_workers();


    // COMPUTE ONE-LOOP KERNELS

  protected:

    //! compute kernels at given redshifts
    void integrate_oneloop(const FRW_model& model, const FRW_model_token& token, z_database& z_db, data_manager& dmgr);


    // INTERNAL DATA

  private:

    // MPI environment

    //! MPI environment object, inherited from parent task manager
    boost::mpi::environment& mpi_env;

    //! MPI communicator, inherited from parent task manager
    boost::mpi::communicator& mpi_world;


    // Caches

    //! argument cache, inherited from parent task manager
    argument_cache& arg_cache;

    //! local environment properties (constructed locally)
    local_environment local_env;


    // Functional blocks

    //! error handler
    error_handler err_handler;

  };


#endif //LSSEFT_MASTER_CONTROLLER_H
