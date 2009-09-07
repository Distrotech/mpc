/* mpc_mul_si -- Multiply a complex number by a signed integer.

Copyright (C) 2005, 2009 Andreas Enge, Philippe Th\'eveny

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

int
mpc_mul_si (mpc_ptr a, mpc_srcptr b, long int c, mpc_rnd_t rnd)
{
   int inex_re, inex_im, inex;

   if (mpfr_cmp_ui (MPC_RE (b), 0ul) == 0 || mpfr_cmp_ui (MPC_IM (b), 0ul) == 0
       || c == 0) {
      /* Signs of zeroes may pose problems; delegate work to mpc_mul_fr */
      mpfr_t fr;
      mpfr_init2 (fr, 2);
      mpfr_set_si (fr, c, GMP_RNDN);
      inex = mpc_mul_fr (a, b, fr, rnd);
      mpfr_clear (fr);
      return inex;
   }
   else {
      inex_re = mpfr_mul_si (MPC_RE(a), MPC_RE(b), c, MPC_RND_RE(rnd));
      inex_im = mpfr_mul_si (MPC_IM(a), MPC_IM(b), c, MPC_RND_IM(rnd));
      return MPC_INEX (inex_re, inex_im);
   }
}
