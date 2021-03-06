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

#ifndef LSSEFT_LOOP_INTEGRAL_H
#define LSSEFT_LOOP_INTEGRAL_H


#include "database/tokens.h"
#include "units/Mpc_units.h"

#include "boost/timer/timer.hpp"
#include "boost/serialization/serialization.hpp"


template <typename ValueType>
class loop_integral_result
  {

  public:
    
    typedef ValueType value_type;
    
    //! constructor initializes zero values, which should be overwritten later
    loop_integral_result()
      : value(value_type(0.0)),
        error(value_type(0.0)),
        regions(0),
        evaluations(0),
        time(0)
      {
      }
    
    //! destructor is default
    ~loop_integral_result() = default;

    
    // DATA
  
  public:
    
    value_type                    value;
    value_type                    error;
    
    unsigned int                  regions;
    unsigned int                  evaluations;
    boost::timer::nanosecond_type time;
  
  private:
    
    // enable boost::serialization support, and hence automated packing for transmission over MPI
    friend class boost::serialization::access;
    
    template <typename Archive>
    void serialize(Archive& ar, unsigned int version)
      {
        ar & value;
        ar & regions;
        ar & evaluations;
        ar & error;
        ar & time;
      }

  };


template <typename ValueType>
class loop_integral_output
  {
  
  public:
    
    typedef ValueType value_type;

    //! constructor initializes result values to zero
    //! these should be overwritten later
    loop_integral_output()
      : raw(),
        nowiggle()
      {
      }
    
    //! destructor is default
    ~loop_integral_output() = default;
    
    
    // ACCESSORS
    
  public:
    
    //! get raw value
    loop_integral_result<ValueType>& get_raw() { return this->raw; }
    const loop_integral_result<ValueType>& get_raw() const { return this->raw; }
    
    //! get no-wiggle value
    loop_integral_result<ValueType>& get_nowiggle() { return this->nowiggle; }
    const loop_integral_result<ValueType>& get_nowiggle() const { return this->nowiggle; }
    
    
    // INTERNAL DATA
    
  private:
    
    //! raw result
    loop_integral_result<ValueType> raw;
    
    //! wiggle result
    loop_integral_result<ValueType> nowiggle;
    
  
  private:
    
    // enable boost::serialization support, and hence automated packing for transmission over MPI
    friend class boost::serialization::access;
    
    template <typename Archive>
    void serialize(Archive& ar, unsigned int version)
      {
        ar & raw;
        ar & nowiggle;
      }
    
  };


typedef loop_integral_output<Mpc_units::inverse_energy3> inverse_energy3_integral;
typedef loop_integral_output<double>                     dimless_integral;


#include "autogenerated/kernel_class.cpp"


class loop_integral
  {

    // CONSTRUCTOR, DESTRUCTOR

  public:

    //! value constructor
    loop_integral(const k_token& kt, const loop_integral_params_token& pt, const linear_Pk_token& Pt,
                  const UV_cutoff_token& UVt, const IR_cutoff_token& IRt, const kernels& ker_);

    //! empty constructor, used for constructing an empty container to be overwritten by an MPI payload
    loop_integral();

    //! destructor is default
    ~loop_integral() = default;


    // INTERFACE

  public:
    
    //! get parameters token
    const loop_integral_params_token& get_params_token() const { return this->params; }

    //! get wavenumber token
    const k_token& get_k_token() const { return this->k; }
    
    //! get linear power spectrum token
    const linear_Pk_token& get_Pk_token() const { return this->Pk_lin; }

    //! get UV cutoff token
    const UV_cutoff_token& get_UV_token() const { return this->UV_cutoff; }

    //! get IR cutoff token
    const IR_cutoff_token& get_IR_token() const { return this->IR_cutoff; }
    
    
    
    //! get kernel container
    const kernels& get_kernels() const { return this->ker; }


    // INTERNAL DATA

  private:

    // CONFIGURATION DATA
    
    //! parameters token
    loop_integral_params_token params;

    //! wavenumber token
    k_token k;
    
    //! linear power spectrum token
    linear_Pk_token Pk_lin;

    //! UV cutoff token
    UV_cutoff_token UV_cutoff;

    //! IR cutoff token
    IR_cutoff_token IR_cutoff;


    // VALUES
    
    //! kernel results
    kernels ker;


    // enable boost::serialization support, and hence automated packing for transmission over MPI
    friend class boost::serialization::access;

    template <typename Archive>
    void serialize(Archive& ar, unsigned int version)
      {
        ar & k;
        ar & UV_cutoff;
        ar & IR_cutoff;
        ar & ker;
      }

  };


#endif //LSSEFT_LOOP_INTEGRAL_H
