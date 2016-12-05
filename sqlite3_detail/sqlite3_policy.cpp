//
// Created by David Seery on 12/08/2015.
// Copyright (c) 2015 University of Sussex. All rights reserved.
//

#include "sqlite3_policy.h"

#include "sqlite3_defaults.h"


sqlite3_policy::sqlite3_policy()
  : FRW_model(SQLITE3_DEFAULT_FRW_TABLE_NAME),
    redshift_config(SQLITE3_DEFAULT_REDSHIFT_CONFIGURATION_TABLE),
    wavenumber_config(SQLITE3_DEFAULT_WAVENUMBER_CONFIGURATION_TABLE),
    IR_config(SQLITE3_DEFAULT_IR_CUTOFF_CONFIGURATION_TABLE),
    UV_config(SQLITE3_DEFAULT_UV_CUTOFF_CONFIGURATION_TABLE),
    IR_resum_config(SQLITE3_DEFAULT_IR_RESUM_CONFIGURATION_TABLE),
    Pk_linear_config(SQLITE3_DEFAULT_LINEAR_PK_CONFIGURATION_TABLE),
    Pk_linear(SQLITE3_DEFAULT_LINEAR_PK_DATA_TABLE),
    transfer(SQLITE3_DEFAULT_TRANSFER_TABLE),
    growth_g_factor(SQLITE3_DEFAULT_GROWTH_G_TABLE),
    growth_f_factor(SQLITE3_DEFAULT_GROWTH_F_TABLE),
    AA(SQLITE3_DEFAULT_LOOP_AA_TABLE),
    AB(SQLITE3_DEFAULT_LOOP_AB_TABLE),
    BB(SQLITE3_DEFAULT_LOOP_BB_TABLE),
    D(SQLITE3_DEFAULT_LOOP_D_TABLE),
    E(SQLITE3_DEFAULT_LOOP_E_TABLE),
    F(SQLITE3_DEFAULT_LOOP_F_TABLE),
    G(SQLITE3_DEFAULT_LOOP_G_TABLE),
    J1(SQLITE3_DEFAULT_LOOP_J1_TABLE),
    J2(SQLITE3_DEFAULT_LOOP_J2_TABLE),
    RSD13_a(SQLITE3_DEFAULT_LOOP_RSD13_A_TABLE),
    RSD13_b(SQLITE3_DEFAULT_LOOP_RSD13_B_TABLE),
    RSD13_c(SQLITE3_DEFAULT_LOOP_RSD13_C_TABLE),
    RSD13_d(SQLITE3_DEFAULT_LOOP_RSD13_D_TABLE),
    RSD13_e(SQLITE3_DEFAULT_LOOP_RSD13_E_TABLE),
    RSD13_f(SQLITE3_DEFAULT_LOOP_RSD13_F_TABLE),
    RSD13_g(SQLITE3_DEFAULT_LOOP_RSD13_G_TABLE),
    RSD22_A1(SQLITE3_DEFAULT_LOOP_RSD22_A1_TABLE),
    RSD22_A2(SQLITE3_DEFAULT_LOOP_RSD22_A2_TABLE),
    RSD22_A3(SQLITE3_DEFAULT_LOOP_RSD22_A3_TABLE),
    RSD22_A4(SQLITE3_DEFAULT_LOOP_RSD22_A4_TABLE),
    RSD22_A5(SQLITE3_DEFAULT_LOOP_RSD22_A5_TABLE),
    RSD22_B2(SQLITE3_DEFAULT_LOOP_RSD22_B2_TABLE),
    RSD22_B3(SQLITE3_DEFAULT_LOOP_RSD22_B3_TABLE),
    RSD22_B6(SQLITE3_DEFAULT_LOOP_RSD22_B6_TABLE),
    RSD22_B8(SQLITE3_DEFAULT_LOOP_RSD22_B8_TABLE),
    RSD22_B9(SQLITE3_DEFAULT_LOOP_RSD22_B9_TABLE),
    RSD22_C1(SQLITE3_DEFAULT_LOOP_RSD22_C1_TABLE),
    RSD22_C2(SQLITE3_DEFAULT_LOOP_RSD22_C2_TABLE),
    RSD22_C4(SQLITE3_DEFAULT_LOOP_RSD22_C4_TABLE),
    RSD22_D1(SQLITE3_DEFAULT_LOOP_RSD22_D1_TABLE),
    dd_Pk(SQLITE3_DEFAULT_LOOP_DD_TABLE),
    dd_rsd_mu0_Pk(SQLITE3_DEFAULT_LOOP_DD_RSD_MU0_TABLE),
    dd_rsd_mu2_Pk(SQLITE3_DEFAULT_LOOP_DD_RSD_MU2_TABLE),
    dd_rsd_mu4_Pk(SQLITE3_DEFAULT_LOOP_DD_RSD_MU4_TABLE),
    dd_rsd_mu6_Pk(SQLITE3_DEFAULT_LOOP_DD_RSD_MU6_TABLE),
    dd_rsd_mu8_Pk(SQLITE3_DEFAULT_LOOP_DD_RSD_MU8_TABLE),
    P0(SQLITE3_DEFAULT_LOOP_P0_TABLE),
    P2(SQLITE3_DEFAULT_LOOP_P2_TABLE),
    P4(SQLITE3_DEFAULT_LOOP_P4_TABLE),
    Matsubara_A(SQLITE3_DEFAULT_MATSUBARA_A_TABLE),
    temp(SQLITE3_DEFAULT_TEMPORARY_TABLE)
  {
  }
