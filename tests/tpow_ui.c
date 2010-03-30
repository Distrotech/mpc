/* test file for mpc_pow_ui.

Copyright (C) 2009 Paul Zimmermann

This file is part of the MPC Library.

The MPC Library is free software; you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as published by
the Free Software Foundation; either version 2.1 of the License, or (at your
option) any later version.

The MPC Library is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public
License for more details.

You should have received a copy of the GNU Lesser General Public License
along with the MPC Library; see the file COPYING.LIB.  If not, write to
the Free Software Foundation, Inc., 59 Temple Place - Suite 330, Boston,
MA 02111-1307, USA. */

#include <limits.h> /* for CHAR_BIT */
#include "mpc-tests.h"

static void
compare_mpc_pow (mp_prec_t pmax, int iter, unsigned long nbits)
{
  mp_prec_t p;
  mpc_t x, y, z, t;
  unsigned long n;
  int i, inex_pow, inex_pow_ui;
  gmp_randstate_t state;
  mpc_rnd_t rnd;
  
  gmp_randinit_default (state);
  mpc_init3 (y, sizeof (unsigned long) * CHAR_BIT, MPFR_PREC_MIN);
  for (p = MPFR_PREC_MIN; p <= pmax; p++)
    for (i = 0; i < iter; i++)
      {
        mpc_init2 (x, p);
        mpc_init2 (z, p);
        mpc_init2 (t, p);
        mpc_urandom (x, state);
        n = gmp_urandomb_ui (state, nbits); /* 0 <= n < 2^nbits */
        mpc_set_ui (y, n, MPC_RNDNN);
        for (rnd = 0; rnd < 16; rnd ++)
          {
            inex_pow = mpc_pow (z, x, y, rnd);
            inex_pow_ui = mpc_pow_ui (t, x, n, rnd);
            if (mpc_cmp (z, t) != 0)
              {
                printf ("mpc_pow and mpc_pow_ui differ for x=");
                mpc_out_str (stdout, 10, 0, x, MPC_RNDNN);
                printf (" n=%lu\n", n);
                printf ("mpc_pow gives ");
                mpc_out_str (stdout, 10, 0, z, MPC_RNDNN);
                printf ("\nmpc_pow_ui gives ");
                mpc_out_str (stdout, 10, 0, t, MPC_RNDNN);
                printf ("\n");
                exit (1);
              }
            if (inex_pow != inex_pow_ui)
              {
                printf ("mpc_pow and mpc_pow_ui gives different flags for x=");
                mpc_out_str (stdout, 10, 0, x, MPC_RNDNN);
                printf (" n=%lu\n", n);
                printf ("mpc_pow gives %d\n", inex_pow);
                printf ("mpc_pow_ui gives %d\n", inex_pow_ui);
                exit (1);
              }
          }
        mpc_clear (x);
        mpc_clear (z);
        mpc_clear (t);
      }
  mpc_clear (y);
  gmp_randclear (state);
}

int
main (void)
{
  mpc_t z;

  test_start ();

  mpc_init2 (z, 5);
  mpc_set_ui_ui (z, 3, 2, MPC_RNDNN);
  mpc_pow_ui (z, z, 3, MPC_RNDNN);
  if (mpc_cmp_si_si (z, -9, 46) != 0)
    {
      printf ("Error for mpc_pow_ui (1)\n");
      exit (1);
    }
  mpc_clear (z);

  compare_mpc_pow (100, 5, 19);

  test_end ();

  return 0;
}
