/* mpc_atan -- arctangent of a complex number.

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

/* set rop to
   -pi/2 if s < 0
   +pi/2 else
   rounded in the direction rnd
*/
int
set_pi_over_2 (mpfr_ptr rop, int s, mpfr_rnd_t rnd)
{
  int inex;

  inex = mpfr_const_pi (rop, s < 0 ? INV_RND (rnd) : rnd);
  mpfr_div_2ui (rop, rop, 1, GMP_RNDN);
  if (s < 0)
    {
      inex = -inex;
      mpfr_neg (rop, rop, GMP_RNDN);
    }

  return inex;
}

int
mpc_atan (mpc_ptr rop, mpc_srcptr op, mpc_rnd_t rnd)
{
  int s_re;
  int s_im;
  int inex_re;
  int inex_im;
  int inex;

  inex_re = 0;
  inex_im = 0;
  s_re = mpfr_signbit (MPC_RE (op));
  s_im = mpfr_signbit (MPC_IM (op));

  /* special values */
  if (mpfr_nan_p (MPC_RE (op)) || mpfr_nan_p (MPC_IM (op)))
    {
      if (mpfr_nan_p (MPC_RE (op)))
        {
          mpfr_set_nan (MPC_RE (rop));
          if (mpfr_zero_p (MPC_IM (op)) || mpfr_inf_p (MPC_IM (op)))
            {
              mpfr_set_ui (MPC_IM (rop), 0, GMP_RNDN);
              if (s_im)
                mpc_conj (rop, rop, MPC_RNDNN);
            }
          else
            mpfr_set_nan (MPC_IM (rop));
        }
      else
        {
          if (mpfr_inf_p (MPC_RE (op)))
            {
              inex_re = set_pi_over_2 (MPC_RE (rop), -s_re, MPC_RND_RE (rnd));
              mpfr_set_ui (MPC_IM (rop), 0, GMP_RNDN);
            }
          else
            {
              mpfr_set_nan (MPC_RE (rop));
              mpfr_set_nan (MPC_IM (rop));
            }
        }
      return MPC_INEX (inex_re, 0);
    }

  if (mpfr_inf_p (MPC_RE (op)) || mpfr_inf_p (MPC_IM (op)))
    {
      inex_re = set_pi_over_2 (MPC_RE (rop), -s_re, MPC_RND_RE (rnd));

      mpfr_set_ui (MPC_IM (rop), 0, GMP_RNDN);
      if (s_im)
        mpc_conj (rop, rop, GMP_RNDN);

      return MPC_INEX (inex_re, 0);
    }

  /* pure real argument */
  if (mpfr_zero_p (MPC_IM (op)))
    {
      inex_re = mpfr_atan (MPC_RE (rop), MPC_RE (op), MPC_RND_RE (rnd));

      mpfr_set_ui (MPC_IM (rop), 0, GMP_RNDN);
      if (s_im)
        mpc_conj (rop, rop, GMP_RNDN);

      return MPC_INEX (inex_re, 0);
    }

  /* pure imaginary argument */
  if (mpfr_zero_p (MPC_RE (op)))
    {
      int cmp_1;

      if (s_im)
        cmp_1 = -mpfr_cmp_si (MPC_IM (op), -1);
      else
        cmp_1 = mpfr_cmp_ui (MPC_IM (op), +1);

      if (cmp_1 < 0)
        {
          /* atan(+0+iy) = +0 +i*atanh(y), if |y| < 1
             atan(-0+iy) = -0 +i*atanh(y), if |y| < 1 */

          mpfr_set_ui (MPC_RE (rop), 0, GMP_RNDN);
          if (s_re)
            mpfr_neg (MPC_RE (rop), MPC_RE (rop), GMP_RNDN);

          inex_im = mpfr_atanh (MPC_IM (rop), MPC_IM (op), MPC_RND_IM (rnd));
        }
      else if (cmp_1 == 0)
        {
          /* atan(+/-0+i) = NaN +i*inf
             atan(+/-0-i) = NaN -i*inf */
          mpfr_set_nan (MPC_RE (rop));
          mpfr_set_inf (MPC_IM (rop), s_im ? -1 : +1);
        }
      else
        {
          /* atan(+0+iy) = +pi/2 +i*atanh(1/y), if |y| > 1
             atan(-0+iy) = -pi/2 +i*atanh(1/y), if |y| > 1 */
          mp_rnd_t rnd_im;
          mpfr_t y;
          mp_prec_t p, p_im;
          int ok;

          rnd_im = MPC_RND_IM (rnd);
          mpfr_init (y);
          p_im = mpfr_get_prec (MPC_IM (rop));
          p = p_im;

          /* a = o(1/y)      with error(a) < 1 ulp(a)
             b = o(atanh(a)) with error(b) < (1+2^{1+Exp(a)-Exp(b)}) ulp(b)

             As |atanh (1/y)| > |1/y| we have Exp(a)-Exp(b) <=0 so, at most,
             2 bits of precision are lost.
          */
          do
            {
              p += mpc_ceil_log2 (p) + 2;
              mpfr_set_prec (y, p);
              inex_im = mpfr_ui_div (y, 1, MPC_IM (op), GMP_RNDZ);
              if (mpfr_zero_p (y))
                break; /* underflow. */
              /* FIXME: should we consider the case with unreasonably huge
                 precision prec(y)>3*exp_min, where atanh(1/Im(op)) could be
                 representable while 1/Im(op) underflows ? */

              /* atanh cannot underflow: |atanh(x)| > |x| for |x| < 1 */
              inex_im |= mpfr_atanh (y, y, GMP_RNDZ);

              ok = inex_im == 0
                || mpfr_can_round (y, p-2, GMP_RNDZ, rnd_im,
                                   p_im + (rnd_im == GMP_RNDN));
            } while (ok == 0);

          inex_re = set_pi_over_2 (MPC_RE (rop), -s_re, MPC_RND_RE (rnd));
          inex_im = mpfr_set (MPC_IM (rop), y, rnd_im);
          mpfr_clear (y);
        }
      return MPC_INEX (inex_re, inex_im);
    }

  /* regular number argument */
  /* TODO */
  mpfr_set_nan (MPC_RE (rop));
  mpfr_set_nan (MPC_IM (rop));
  return 0;
}
