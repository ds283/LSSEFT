//
// Created by David Seery on 21/11/2015.
// Copyright (c) 2015 University of Sussex. All rights reserved.
//

#ifndef LSSEFT_MPI_TRAITS_H
#define LSSEFT_MPI_TRAITS_H


#include "cosmology/types.h"

#include "mpi_operations.h"


namespace MPI_detail
  {

    template <typename WorkItem> struct work_item_traits;

    template <> struct work_item_traits<transfer_work_record>
      {
        typedef new_transfer_integration   outgoing_payload_type;
        typedef transfer_integration_ready incoming_payload_type;

        static constexpr unsigned int new_task_message() { return(MESSAGE_NEW_TRANSFER_TASK); }
        static constexpr unsigned int new_item_message() { return(MESSAGE_NEW_TRANSFER_INTEGRATION); }
      };

    template <> struct work_item_traits<loop_momentum_work_record>
      {
        typedef new_loop_momentum_integration   outgoing_payload_type;
        typedef loop_momentum_integration_ready incoming_payload_type;

        static constexpr unsigned int new_task_message() { return(MESSAGE_NEW_LOOP_INTEGRAL_TASK); }
        static constexpr unsigned int new_item_message() { return(MESSAGE_NEW_LOOP_INTEGRATION); }
      };

  }   // namespace MPI_detail


#endif //LSSEFT_MPI_TRAITS_H