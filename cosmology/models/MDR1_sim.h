//
// Created by David Seery on 13/03/2017.
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

#ifndef LSSEFT_MDR1_SIM_H
#define LSSEFT_MDR1_SIM_H


#include "units/Mpc_units.h"


// Parameters for the MDR1 simulation from https://www.cosmosim.org/cms/simulations/mdr1/
namespace MDR1
  {
    
    const     std::string       name     = "Matches MDR1 simulation https://www.cosmosim.org/cms/simulations/mdr1";
    
    constexpr double            omega_cc = 0.73;
    constexpr double            omega_m  = 0.27;
    constexpr double            h        = 0.70;
    constexpr double            f_baryon = 0.0469/omega_m;
    constexpr Mpc_units::energy H0       = 100 * h * Mpc_units::Kilometre / (Mpc_units::Second * Mpc_units::Mpc);
    constexpr Mpc_units::energy T_CMB    = 2.7255 * Mpc_units::Kelvin;
    constexpr double            Neff     = 3.046;   // Standard Model value
    
    // fluctuation two-point function
    constexpr double Acurv           = 2.3650e-9;
    constexpr double ns              = 0.95;
    constexpr Mpc_units::energy kpiv = 0.05 / Mpc_units::Mpc;
    constexpr double sigma8          = 0.8200;
    
    // CMB-related redshifts (extracted from CAMB)
    constexpr double z_star = 1088.02;
    constexpr double z_drag = 1060.39;
    constexpr double z_eq   = 3161.49;
    
  }


// Parameters for the Big MultiDark Planck simulation from https://www.cosmosim.org/cms/simulations/bigmdpl/
namespace BigMDPL
  {
    
    const     std::string       name     = "Matches BigMDPL simulation https://www.cosmosim.org/cms/simulations/bigmdpl/";
    
    constexpr double            omega_cc = 0.692885;
    constexpr double            omega_m  = 0.307115;
    constexpr double            h        = 0.6777;
    constexpr double            f_baryon = 0.048206/omega_m;
    constexpr Mpc_units::energy H0       = 100 * h * Mpc_units::Kilometre / (Mpc_units::Second * Mpc_units::Mpc);
    constexpr Mpc_units::energy T_CMB    = 2.7255 * Mpc_units::Kelvin;
    constexpr double            Neff     = 3.046;   // Standard Model value
    
    // fluctuation two-point function
    constexpr double Acurv           = 1.9716e-9;
    constexpr double ns              = 0.96;
    constexpr Mpc_units::energy kpiv = 0.05 / Mpc_units::Mpc;
    constexpr double sigma8          = 0.8228;
    
    // CMB-related redshifts (extracted from CAMB)
    constexpr double z_star = 1088.75;
    constexpr double z_drag = 1063.06;
    constexpr double z_eq   = 3596.22;
    
  }


#endif //LSSEFT_MDR1_SIM_H
