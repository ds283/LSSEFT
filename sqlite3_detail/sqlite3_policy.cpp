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
    IR_config(SQLITE3_DEFAULT_IR_CONFIGURATION_TABLE),
    UV_config(SQLITE3_DEFAULT_UV_CONFIGURATION_TABLE),
    transfer(SQLITE3_DEFAULT_TRANSFER_TABLE),
    growth_g_factor(SQLITE3_DEFAULT_GROWTH_G_TABLE),
    growth_f_factor(SQLITE3_DEFAULT_GROWTH_F_TABLE),
    growth_fg_metadata(SQLITE3_DEFAULT_LOOP_FG_META_TABLE),
    AA(SQLITE3_DEFAULT_LOOP_AA_TABLE),
    AB(SQLITE3_DEFAULT_LOOP_AB_TABLE),
    BB(SQLITE3_DEFAULT_LOOP_BB_TABLE),
    D(SQLITE3_DEFAULT_LOOP_D_TABLE),
    E(SQLITE3_DEFAULT_LOOP_E_TABLE),
    F(SQLITE3_DEFAULT_LOOP_F_TABLE),
    G(SQLITE3_DEFAULT_LOOP_G_TABLE),
    J1(SQLITE3_DEFAULT_LOOP_J1_TABLE),
    J2(SQLITE3_DEFAULT_LOOP_J2_TABLE),
    dd_Pk(SQLITE3_DEFAULT_LOOP_DD_TABLE),
    dd_rsd_mu0_Pk(SQLITE3_DEFAULT_LOOP_DD_RSD_MU0_TABLE),
    dd_rsd_mu2_Pk(SQLITE3_DEFAULT_LOOP_DD_RSD_MU2_TABLE),
    dd_rsd_mu4_Pk(SQLITE3_DEFAULT_LOOP_DD_RSD_MU4_TABLE),
    dd_rsd_mu6_Pk(SQLITE3_DEFAULT_LOOP_DD_RSD_MU6_TABLE),
    dd_rsd_mu8_Pk(SQLITE3_DEFAULT_LOOP_DD_RSD_MU8_TABLE),
    temp(SQLITE3_DEFAULT_TEMPORARY_TABLE)
  {
  }
