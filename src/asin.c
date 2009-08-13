/* mpc_asin -- arcsine of a complex number.

Copyright (C) 2009 Philippe Th\'eveny

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

#include "mpc-impl.h"

extern int set_pi_over_2 (mpfr_ptr rop, int s, mpfr_rnd_t rnd);

int
mpc_asin (mpc_ptr rop, mpc_srcptr op, mpc_rnd_t rnd)
{
  /* special values */
  if (mpfr_nan_p (MPC_RE (op)) || mpfr_nan_p (MPC_IM (op)))
    {
      if (mpfr_inf_p (MPC_RE (op)) || mpfr_inf_p (MPC_IM (op)))
        {
          mpfr_set_nan (MPC_RE (rop));
          mpfr_set_inf (MPC_IM (rop), mpfr_signbit (MPC_IM (op)) ? -1 : +1);
        }
      else if (mpfr_zero_p (MPC_RE (op)))
        {
          mpfr_set (MPC_RE (rop), MPC_RE (op), GMP_RNDN);
          mpfr_set_nan (MPC_IM (rop));
        }
      else
        {
          mpfr_set_nan (MPC_RE (rop));
          mpfr_set_nan (MPC_IM (rop));
        }

      return 0;
    }

  if (mpfr_inf_p (MPC_RE (op)) || mpfr_inf_p (MPC_IM (op)))
    {
      int inex_re;
      if (mpfr_inf_p (MPC_RE (op)))
        {
          inex_re = set_pi_over_2 (MPC_RE (rop), -mpfr_signbit (MPC_RE (op)),
                                   MPC_RND_RE (rnd));
          mpfr_set_inf (MPC_IM (rop), -mpfr_signbit (MPC_IM (op)));

          if (mpfr_inf_p (MPC_IM (op)))
            mpfr_div_2ui (MPC_RE (rop), MPC_RE (rop), 1, GMP_RNDN);
        }
      else
        {
          int s;
          s = mpfr_signbit (MPC_RE (op));
          inex_re = mpfr_set_ui (MPC_RE (rop), 0, GMP_RNDN);
          if (s)
            mpfr_neg (MPC_RE (rop), MPC_RE (rop), GMP_RNDN);
          mpfr_set_inf (MPC_IM (rop), -mpfr_signbit (MPC_IM (op)));
        }

      return MPC_INEX (inex_re, 0);
    }

  /* pure real argument */
  if (mpfr_zero_p (MPC_IM (op)))
    {
      int inex_re;
      int inex_im;
      int s_im;
      s_im = mpfr_signbit (MPC_IM (op));

      if (mpfr_cmp_ui (MPC_RE (op), 1) > 0)
        {
          if (s_im)
            inex_im = -mpfr_acosh (MPC_IM (rop), MPC_RE (op),
                                   INV_RND (MPC_RND_IM (rnd)));
          else
            inex_im = mpfr_acosh (MPC_IM (rop), MPC_RE (op),
                                  MPC_RND_IM (rnd));
          inex_re = set_pi_over_2 (MPC_RE (rop), -mpfr_signbit (MPC_RE (op)),
                                   MPC_RND_RE (rnd));
          if (s_im)
            mpc_conj (rop, rop, MPC_RNDNN);
        }
      else if (mpfr_cmp_si (MPC_RE (op), -1) < 0)
        {
          mpfr_t minus_op_re;
          minus_op_re[0] = MPC_RE (op)[0];
          MPFR_CHANGE_SIGN (minus_op_re);

          if (s_im)
            inex_im = -mpfr_acosh (MPC_IM (rop), minus_op_re,
                                   INV_RND (MPC_RND_IM (rnd)));
          else
            inex_im = mpfr_acosh (MPC_IM (rop), minus_op_re,
                                  MPC_RND_IM (rnd));
          inex_re = set_pi_over_2 (MPC_RE (rop), -mpfr_signbit (MPC_RE (op)),
                                   MPC_RND_RE (rnd));
          if (s_im)
            mpc_conj (rop, rop, MPC_RNDNN);
        }
      else
        {
          inex_im = mpfr_set_ui (MPC_IM (rop), 0, MPC_RND_IM (rnd));
          if (s_im)
            mpfr_neg (MPC_IM (rop), MPC_IM (rop), GMP_RNDN);
          inex_re = mpfr_asin (MPC_RE (rop), MPC_RE (op), MPC_RND_RE (rnd));
        }

      return MPC_INEX (inex_re, inex_im);      
    }

  /* pure imaginary argument */
  if (mpfr_zero_p (MPC_RE (op)))
    {
      int inex_im;
      int s;
      s = mpfr_signbit (MPC_RE (op));
      mpfr_set_ui (MPC_RE (rop), 0, GMP_RNDN);
      if (s)
        mpfr_neg (MPC_RE (rop), MPC_RE (rop), GMP_RNDN);
      inex_im = mpfr_asinh (MPC_IM (rop), MPC_IM (op), MPC_RND_IM (rnd));

      return MPC_INEX (0, inex_im);      
    }

  /* regular complex */
  /* TODO */
  mpfr_set_nan (MPC_RE (rop));
  mpfr_set_nan (MPC_RE (rop));
  return 0;
}
