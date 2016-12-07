//
// Created by David Seery on 07/12/2016.
// Copyright (c) 2016 University of Sussex. All rights reserved.
//

#ifndef LSSEFT_PK_FILTER_H
#define LSSEFT_PK_FILTER_H


#include "FRW_model.h"
#include "cosmology/concepts/power_spectrum.h"

#include "database/tokens.h"

#include "defaults.h"


class Pk_filter
  {
    
    // CONSTRUCTOR, DESTRUCTOR
  
  public:
    
    //! constructor is default
    Pk_filter(double r = LSSEFT_DEFAULT_INTEGRAL_REL_ERR_22,
              double a = LSSEFT_DEFAULT_INTEGRAL_ABS_ERR_22)
      : rel_err(std::abs(r)),
        abs_err(std::abs(a))
      {
      }
    
    //! destructor is default
    ~Pk_filter() = default;
    
    
    // INTERFACE
    
  public:
    
    //! filter a linear power spectrum
    Mpc_units::inverse_energy3 operator()(const FRW_model& model, const linear_Pk& Pk_lin, const Mpc_units::energy& k);
    
    
    // INTERNAL API
    
  private:
    
    //! compute Eisenstein & Hu approximation to the power spectrum
    std::unique_ptr<approx_Pk> eisenstein_hu(const FRW_model& model, const linear_Pk& Pk_lin);
    
    
    // INTERNAL DATA
    
  private:
    
    //! relative tolerance
    double rel_err;
    
    //! absolute tolerance
    double abs_err;
    
  };


#endif //LSSEFT_PK_FILTER_H
