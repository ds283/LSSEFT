//
// Created by David Seery on 17/08/2015.
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

#ifndef LSSEFT_ONELOOP_INTEGRATOR_H
#define LSSEFT_ONELOOP_INTEGRATOR_H


#include <memory>

#include "FRW_model.h"
#include "concepts/oneloop_growth.h"

#include "database/tokens.h"
#include "database/z_database.h"

#include "defaults.h"

#include "boost/timer/timer.hpp"


class growth_params
  {
    
    // CONSTRUCTOR, DESTRUCTOR
  
  public:
    
    //! constructor
    growth_params(bool EdS=false, bool EdSi=true,
                  double a=LSSEFT_DEFAULT_ODE_ABS_ERR, double r=LSSEFT_DEFAULT_ODE_REL_ERR)
      : EdS_mode(EdS),
        EdS_ics(EdSi),
        abs_err(a),
        rel_err(r)
      {
      }
    
    //! destructor is default
    ~growth_params() = default;
    
    
    // INTERFACE
  
  public:
    
    //! use EdS mode?
    bool use_EdS() const { return this->EdS_mode; }
    
    //! use EdS initial conditions?
    bool use_EdS_ics() const { return this->EdS_ics; }
    
    //! get abserr
    double get_abserr() const { return this->abs_err; }
    
    //! get relerr
    double get_relerr() const { return this->rel_err; }
    
    
    // INTERNAL DATA
  
  private:
    
    //! use Einstein-de Sitter approximations to growth factors?
    bool EdS_mode;
    
    //! use Einstien-de Sitter initial conditions for growth factors?
    bool EdS_ics;
    
    //! absolute tolerance
    double abs_err;
    
    //! relative tolerance
    double rel_err;
    
    // enable boost::serialization support, and hence automated packing for transmission over MPI
    friend class boost::serialization::access;
    
    
    template <typename Archive>
    void serialize(Archive& ar, unsigned int version)
      {
        ar & EdS_mode;
        ar & EdS_ics;
        ar & abs_err;
        ar & rel_err;
      }
    
  };


struct growth_integrator_data
  {
    
    growth_integrator_data(std::unique_ptr<oneloop_growth> c, boost::timer::nanosecond_type t, size_t s)
      : container(std::move(c)),
        time(t),
        steps(s)
      {
      }
    
    std::unique_ptr<oneloop_growth> container;
    boost::timer::nanosecond_type time;
    size_t steps;
    
  };


class oneloop_growth_integrator
  {

    // CONSTRUCTOR, DESTRUCTOR

  public:

    //! constructor
    oneloop_growth_integrator(const growth_params& p, const growth_params_token& t);

    //! destructor is default
    ~oneloop_growth_integrator() = default;


    // ONE-LOOP GROWTH FACTORS

  public:

    //! integrate one-loop growth factors for a given set of redshift samples
    growth_integrator_data integrate(const FRW_model& model, z_database& z_db);


    // INTERNAL DATA

  private:

    //! parameter block
    const growth_params params;
    
    //! token for parameter block
    const growth_params_token token;

  };


#endif //LSSEFT_ONELOOP_INTEGRATOR_H
