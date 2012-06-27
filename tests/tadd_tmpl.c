/* tadd_tmpl.c -- templated test file for mpc_add.

Copyright (C) 2008, 2010, 2011, 2012 INRIA

This file is part of GNU MPC.

GNU MPC is free software; you can redistribute it and/or modify it under
the terms of the GNU Lesser General Public License as published by the
Free Software Foundation; either version 3 of the License, or (at your
option) any later version.

GNU MPC is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for
more details.

You should have received a copy of the GNU Lesser General Public License
along with this program. If not, see http://www.gnu.org/licenses/ .
*/

#include "templates.h"
#include "mpc-tests.h"

static void
check_ternary_value (void)
{
  mpc_t x, y, z;
  mpfr_prec_t prec;

  mpc_init2 (x, 2);
  mpc_init2 (y, 2);
  mpc_init2 (z, 2);

  for (prec = 2; prec <= 1000; prec++)
    {
      mpc_set_prec (x, prec);
      mpc_set_prec (y, prec);

      mpc_set_ui (x, 1, MPC_RNDNN);
      mpc_mul_2exp (x, x, (unsigned long int) prec, MPC_RNDNN);
      mpc_set_ui (y, 1, MPC_RNDNN);

      if (mpc_add (z, x, y, MPC_RNDNN) == 0)
        {
          fprintf (stderr, "Error in mpc_add: 2^(-prec)+1 cannot be exact\n");
          exit (1);
        }
    }

  mpc_clear (x);
  mpc_clear (y);
  mpc_clear (z);
}

#define MPC_FUNCTION_CALL                                               \
  P[0].mpc_inex = mpc_add (P[1].mpc, P[2].mpc, P[3].mpc, P[4].mpc_rnd)

#if 0
#define MPC_FUNCTION_CALL_REUSE_OP1                                     \
  P[0].mpc_inex = mpc_add (P[1].mpc, P[1].mpc, P[3].mpc, P[4].mpc_rnd)
#define MPC_FUNCTION_CALL_REUSE_OP2                                     \
  P[0].mpc_inex = mpc_add (P[1].mpc, P[2].mpc, P[1].mpc, P[4].mpc_rnd)
#endif

#include "data_check.tpl"

int
main (void)
{
  DECL_FUNC (C_CC, f, mpc_add);
  f.properties = FUNC_PROP_SYMETRIC;

  test_start ();

  check_ternary_value();

  data_check_template ("add.dsc", "add.dat");

  tgeneric (f, 2, 1024, 7, -1);

  test_end ();

  return 0;
}
