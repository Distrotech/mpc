/* mpc_acosh -- inverse hyperbolic cosine of a complex number.

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

int
mpc_acosh (mpc_ptr rop, mpc_srcptr op, mpc_rnd_t rnd)
{
  /* acosh(z) =
      i*acos(z), if sign(Im(z)) = +  
     -i*acos(z), if sign(Im(z)) = -
  */
  mpfr_t tmp;
  int inex;
  
  if (mpfr_signbit (MPC_IM (op)))
    {
      inex = mpc_acos (rop, op,
                       RNDC (MPC_RND_IM (rnd), INV_RND (MPC_RND_RE (rnd))));

      /* change rop to -i*rop */
      tmp[0] = MPC_RE (rop)[0];
      MPC_RE (rop)[0] = MPC_IM (rop)[0];
      MPC_IM (rop)[0] = tmp[0];
      MPFR_CHANGE_SIGN (MPC_IM (rop));
      inex = MPC_INEX (-MPC_INEX_IM (inex), MPC_INEX_RE (inex));
    }
  else
    {
      inex = mpc_acos (rop, op,
                       RNDC (INV_RND (MPC_RND_IM (rnd)), MPC_RND_RE (rnd)));

      /* change rop to i*rop */
      tmp[0] = MPC_RE (rop)[0];
      MPC_RE (rop)[0] = MPC_IM (rop)[0];
      MPC_IM (rop)[0] = tmp[0];
      MPFR_CHANGE_SIGN (MPC_RE (rop));
      inex = MPC_INEX (MPC_INEX_IM (inex), -MPC_INEX_RE (inex));
    }

  return inex;
}
