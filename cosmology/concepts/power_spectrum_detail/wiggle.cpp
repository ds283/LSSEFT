//
// Created by David Seery on 05/12/2016.
// Copyright (c) 2016 University of Sussex. All rights reserved.
//

#include "wiggle.h"


wiggle_Pk::wiggle_Pk(const linear_Pk_token& t, const tree_Pk_w::database_type& w, const tree_Pk::database_type& r)
  : tok(t),
    wiggle(w),
    raw(r)
  {
  }
