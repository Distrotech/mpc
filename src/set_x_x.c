/* mpc_set_x_x -- Set complex number real and imaginary parts from parameters
   whose type is known by mpfr.

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

#include "config.h"

#if HAVE_INTTYPES_H
# include <inttypes.h> /* for intmax_t */
#else
# if HAVE_STDINT_H
#  include <stdint.h>
# endif
#endif

#include "mpc.h"

#define MPC_SET_X_X(type, z, real_value, imag_value, rnd)       \
  MPC_SET_X_Y (type, type, z, real_value, imag_value, rnd)

int
mpc_set_d_d (mpc_ptr z, double a, double b, mpc_rnd_t rnd)
  MPC_SET_X_X (_d, z, a, b, rnd)

int
mpc_set_f_f (mpc_ptr z, mpf_srcptr a, mpf_srcptr b, mpc_rnd_t rnd)
  MPC_SET_X_X (_f, z, a, b, rnd)

int
mpc_set_fr_fr (mpc_ptr z, mpfr_srcptr a, mpfr_srcptr b, mpc_rnd_t rnd)
  MPC_SET_X_X (, z, a, b, rnd)

int
mpc_set_ld_ld (mpc_ptr z, long double a, long double b, mpc_rnd_t rnd)
  MPC_SET_X_X (_ld, z, a, b, rnd)

int
mpc_set_q_q (mpc_ptr z, mpq_srcptr a, mpq_srcptr b, mpc_rnd_t rnd)
  MPC_SET_X_X (_q, z, a, b, rnd)
     
int
mpc_set_si_si (mpc_ptr z, long int a, long int b, mpc_rnd_t rnd)
  MPC_SET_X_X (_si, z, a, b, rnd)

int
mpc_set_ui_ui (mpc_ptr z, unsigned long int a, unsigned long int b,
               mpc_rnd_t rnd)
  MPC_SET_X_X (_ui, z, a, b, rnd)

int
mpc_set_z_z (mpc_ptr z, mpz_srcptr a, mpz_srcptr b, mpc_rnd_t rnd)
  MPC_SET_X_X (_z, z, a, b, rnd)

#ifdef _MPC_H_HAVE_INTMAX_T
int
mpc_set_uj_uj (mpc_ptr z, uintmax_t a, uintmax_t b, mpc_rnd_t rnd)
  MPC_SET_X_X (_uj, z, a, b, rnd)

int
mpc_set_sj_sj (mpc_ptr z, intmax_t a, intmax_t b, mpc_rnd_t rnd)
  MPC_SET_X_X (_si, z, a, b, rnd)
#endif
