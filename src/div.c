/* mpc_div -- Divide two complex numbers.

Copyright (C) 2002, 2003, 2004, 2005, 2008, 2009 Andreas Enge, Paul Zimmermann, Philippe Th\'eveny

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

#include <stdio.h>
#include "mpc-impl.h"

int
mpc_div (mpc_ptr a, mpc_srcptr b, mpc_srcptr c, mpc_rnd_t rnd)
{
   int ok_re = 0, ok_im = 0;
   mpc_t  res, c_conj;
   mpfr_t q;
   mp_prec_t prec;
   int inexact_prod, inexact_norm, inexact_re, inexact_im, loops = 0;

   /* check for real divisor */
   if (mpfr_zero_p(MPC_IM(c))) /* (re_b+i*im_b)/c = re_b/c + i * (im_b/c) */
   {
      /* warning: a may overlap with b,c so treat the imaginary part first */
      inexact_im = mpfr_div (MPC_IM(a), MPC_IM(b), MPC_RE(c), MPC_RND_IM(rnd));
      inexact_re = mpfr_div (MPC_RE(a), MPC_RE(b), MPC_RE(c), MPC_RND_RE(rnd));
      return MPC_INEX(inexact_re, inexact_im);
   }

   /* check for purely imaginary divisor */
   if (mpfr_zero_p(MPC_RE(c)))
   {
      /* (re_b+i*im_b)/(i*c) = im_b/c - i * (re_b/c) */
      int overlap = (a == b) || (a == c);
      mpfr_t cloc;

      if (overlap)
         mpc_init3 (res, MPFR_PREC (MPC_RE (a)), MPFR_PREC (MPC_IM (a)));
      else
         res [0] = *a;
      cloc [0] = MPC_IM (c) [0];
      inexact_re = mpfr_div (MPC_RE(res), MPC_IM(b), cloc, MPC_RND_RE(rnd));
      mpfr_neg (cloc, cloc, GMP_RNDN);
      /* changes the sign only in cloc, not in c; no need to correct later */
      inexact_im = mpfr_div (MPC_IM(res), MPC_RE(b), cloc, MPC_RND_IM(rnd));
      if (overlap)
         mpc_clear (a);
      a [0] = res [0];
      return MPC_INEX(inexact_re, inexact_im);
   }

   /* check for NaN anywhere */
   if (mpfr_nan_p (MPC_RE (b)) || mpfr_nan_p (MPC_IM (b))
       || mpfr_nan_p (MPC_RE (c)) || mpfr_nan_p (MPC_IM (c)))
   {
      mpfr_set_nan (MPC_RE (a));
      mpfr_set_nan (MPC_IM (a));
      return MPC_INEX (0, 0);
    }

   prec = MPC_MAX_PREC(a);

   mpc_init2 (res, 2);
   mpfr_init (q);

   /* create the conjugate of c in c_conj without allocating new memory */
   MPC_RE (c_conj)[0] = MPC_RE (c)[0];
   MPC_IM (c_conj)[0] = MPC_IM (c)[0];
   MPFR_CHANGE_SIGN (MPC_IM (c_conj));

   do
   {
      loops ++;
      prec += (loops <= 2) ? mpc_ceil_log2 (prec) + 5 : prec / 2;

      mpc_set_prec (res, prec);
      mpfr_set_prec (q, prec);

      /* first compute norm(c)^2 */
      inexact_norm = mpc_norm (q, c, GMP_RNDD);

      /* now compute b*conjugate(c) */
      /* We need rounding away from zero for both the real and the imagin-  */
      /* ary part; then the final result is also rounded away from zero.    */
      /* The error is less than 1 ulp. Since this is not implemented in     */
      /* mpfr, we round towards zero and add 1 ulp to the absolute values   */
      /* if they are not exact. */
      inexact_prod = mpc_mul (res, b, c_conj, MPC_RNDZZ);
      inexact_re = MPC_INEX_RE (inexact_prod);
      inexact_im = MPC_INEX_IM (inexact_prod);
      if (inexact_re != 0)
         mpfr_add_one_ulp (MPC_RE (res), GMP_RNDN);
      if (inexact_im != 0)
         mpfr_add_one_ulp (MPC_IM (res), GMP_RNDN);

      /* divide the product by the norm */
      if (inexact_norm == 0 && (inexact_re == 0 || inexact_im == 0))
      {
         /* The division has good chances to be exact in at least one part.   */
         /* Since this can cause problems when not rounding to the nearest,   */
         /* we use the division code of mpfr, which handles the situation.    */
         if (MPFR_SIGN (MPC_RE (res)) > 0)
         {
            inexact_re |= mpfr_div (MPC_RE (res), MPC_RE (res), q, GMP_RNDU);
            ok_re = mpfr_can_round (MPC_RE (res), prec - 4, GMP_RNDU,
                  MPC_RND_RE(rnd), MPFR_PREC(MPC_RE(a)));
         }
         else
         {
            inexact_re |= mpfr_div (MPC_RE (res), MPC_RE (res), q, GMP_RNDD);
            ok_re = mpfr_can_round (MPC_RE (res), prec - 4, GMP_RNDD,
                  MPC_RND_RE(rnd), MPFR_PREC(MPC_RE(a)));
         }

         if (ok_re || !inexact_re) /* compute imaginary part */
         {
            if (MPFR_SIGN (MPC_IM (res)) > 0)
            {
               inexact_im |= mpfr_div (MPC_IM (res), MPC_IM (res), q, GMP_RNDU);
               ok_im = mpfr_can_round (MPC_IM (res), prec - 4, GMP_RNDU,
                     MPC_RND_IM(rnd), MPFR_PREC(MPC_IM(a)));
            }
            else
            {
               inexact_im |= mpfr_div (MPC_IM (res), MPC_IM (res), q, GMP_RNDD);
               ok_im = mpfr_can_round (MPC_IM (res), prec - 4, GMP_RNDD,
                     MPC_RND_IM(rnd), MPFR_PREC(MPC_IM(a)));
            }
         }
      }
      else
      {
         /* The division is inexact, so for efficiency reasons we invert q */
         /* only once and multiply by the inverse. */
         /* We do not decide about the sign of the difference. */
         if (mpfr_ui_div (q, 1, q, GMP_RNDU) || inexact_norm)
           {
             /* if 1/q is inexact, the approximations of the real and
                imaginary part below will be inexact, unless RE(res)
                or IM(res) is zero */
             inexact_re = inexact_re || !mpfr_zero_p (MPC_RE (res));
             inexact_im = inexact_im || !mpfr_zero_p (MPC_IM (res));
           }
         if (MPFR_SIGN (MPC_RE (res)) > 0)
         {
           inexact_re = mpfr_mul (MPC_RE (res), MPC_RE (res), q, GMP_RNDU)
             || inexact_re;
           ok_re = mpfr_can_round (MPC_RE (res), prec - 4, GMP_RNDU,
                                   MPC_RND_RE(rnd), MPFR_PREC(MPC_RE(a)));
         }
         else
         {
           inexact_re = mpfr_mul (MPC_RE (res), MPC_RE (res), q, GMP_RNDD)
             || inexact_re;
           ok_re = mpfr_can_round (MPC_RE (res), prec - 4, GMP_RNDD,
                                   MPC_RND_RE(rnd), MPFR_PREC(MPC_RE(a)));
         }

         if (ok_re) /* compute imaginary part */
         {
            if (MPFR_SIGN (MPC_IM (res)) > 0)
            {
              inexact_im = mpfr_mul (MPC_IM (res), MPC_IM (res), q, GMP_RNDU)
                || inexact_im;
               ok_im = mpfr_can_round (MPC_IM (res), prec - 4, GMP_RNDU,
                                       MPC_RND_IM(rnd), MPFR_PREC(MPC_IM(a)));
            }
            else
            {
              inexact_im = mpfr_mul (MPC_IM (res), MPC_IM (res), q, GMP_RNDD)
                || inexact_im;
              ok_im = mpfr_can_round (MPC_IM (res), prec - 4, GMP_RNDD,
                                      MPC_RND_IM(rnd), MPFR_PREC(MPC_IM(a)));
            }
         }
      }
   }
   while ((!ok_re && inexact_re) || (!ok_im && inexact_im));

   mpc_set (a, res, rnd);

   mpc_clear (res);
   mpfr_clear (q);

   return (MPC_INEX (inexact_re, inexact_im));
      /* Only exactness vs. inexactness is tested, not the sign. */
}
