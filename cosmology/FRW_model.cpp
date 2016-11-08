//
// Created by David Seery on 11/08/2015.
// Copyright (c) 2015 University of Sussex. All rights reserved.
//

#include <iostream>

#include "FRW_model.h"


FRW_model::FRW_model(double om, double occ, double h_, Mpc_units::energy tc, double ne)
  : omega_m(om),
    omega_cc(occ),
    h(h_),
    T_CMB(tc),
    Neff(ne)
  {
//    std::cout << "New FRW cosmology with Omega_m=" << omega_m
//      << ", Omega_CC=" << omega_cc
//      << ", h=" << h
//      << ", T_CMB=" << T_CMB / Mpc_units::Kelvin
//      << ", Neff=" << Neff
//      << '\n';
  }
