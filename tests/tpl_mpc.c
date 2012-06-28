/* tpl_mpc.c -- Helper functions for mpc data.

Copyright (C) 2012 INRIA

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

void
tpl_read_mpc_rnd (mpc_datafile_context_t* datafile_context, mpc_rnd_t* rnd)
{
   mpfr_rnd_t re, im;
   tpl_read_mpfr_rnd (datafile_context, &re);
   tpl_read_mpfr_rnd (datafile_context, &im);
   *rnd = RNDC (re, im);
}

void
tpl_read_mpc (mpc_datafile_context_t* datafile_context, mpc_ptr z, known_signs_t *ks)
{
  tpl_read_mpfr (datafile_context, mpc_realref (z), ks == NULL ? NULL : &ks->re);
  tpl_read_mpfr (datafile_context, mpc_imagref (z), ks == NULL ? NULL : &ks->im);
}

void
tpl_read_mpc_inex (mpc_datafile_context_t* datafile_context, int *nread)
{
  tpl_read_ternary(datafile_context, &(nread[0]));
  tpl_read_ternary(datafile_context, &(nread[1]));
}


void
tpl_copy_mpc (mpc_ptr dest, mpc_srcptr src)
{
  /* source and destination are assumed to be of the same precision , so the
     copy is exact (no rounding) */
  mpc_set (dest, src, MPC_RNDNN);
}

int
tpl_same_mpc_value (mpc_ptr z1, mpc_ptr z2, known_signs_t known_signs)
{
   return tpl_same_mpfr_value (mpc_realref (z1), mpc_realref (z2),
                               known_signs.re)
     && tpl_same_mpfr_value (mpc_imagref (z1), mpc_imagref (z2),
                             known_signs.im);
}
