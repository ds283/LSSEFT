//
// Created by David Seery on 05/12/2016.
// Copyright (c) 2016 University of Sussex. All rights reserved.
//

#include "filtered_Pk.h"


filtered_Pk::filtered_Pk(const k_token& kt, const linear_Pk_token& Pt, Mpc_units::inverse_energy3 _Pk_w,
                         Mpc_units::inverse_energy3 _Pk_raw)
  : k_tok(kt),
    Pk_tok(Pt),
    Pk_w(_Pk_w),
    Pk_raw(_Pk_raw)
  {
  }


filtered_Pk::filtered_Pk()
  : k_tok(0),
    Pk_tok(0),
    Pk_w(0.0),
    Pk_raw(0.0)
  {
  }
