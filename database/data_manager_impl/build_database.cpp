//
// Created by David Seery on 09/12/2016.
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
#include <sstream>
#include <assert.h>

#include <set>
#include <unordered_set>

#include "database/data_manager.h"
#include "database/data_manager_impl/types.h"

#include "sqlite3_detail/utilities.h"
#include "sqlite3_detail/operations.h"

#include "utilities/formatter.h"

#include "defaults.h"

#include "boost/timer/timer.hpp"


// GENERATE REDSHIFT AND WAVENUMBER DATABASES


std::unique_ptr<z_database> data_manager::build_redshift_db(range<double>& sample)
  {
    // construct an empty redshift database
    std::unique_ptr<z_database> z_db(new z_database);
    
    // grab the grid of redshift samples
    const std::vector<double>& z_samples = sample.grid();
    
    for(std::vector<double>::const_iterator t = z_samples.begin(); t != z_samples.end(); ++t)
      {
        std::unique_ptr<z_token> tok = this->tokenize(*t);
        z_db->add_record(*t, *tok);
      }
    
    return(z_db);
  }


std::unique_ptr<k_database> data_manager::build_k_db(range<Mpc_units::energy>& sample)
  {
    return this->build_wavenumber_db<k_token>(sample);
  }


std::unique_ptr<IR_cutoff_database> data_manager::build_IR_cutoff_db(range<Mpc_units::energy>& sample)
  {
    return this->build_wavenumber_db<IR_cutoff_token>(sample);
  }


std::unique_ptr<UV_cutoff_database> data_manager::build_UV_cutoff_db(range<Mpc_units::energy>& sample)
  {
    return this->build_wavenumber_db<UV_cutoff_token>(sample);
  }


std::unique_ptr<IR_resum_database> data_manager::build_IR_resum_db(range<Mpc_units::energy>& sample)
  {
    return this->build_wavenumber_db<IR_resum_token>(sample);
  }
