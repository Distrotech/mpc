/* check_data.c -- Check computed data against test data.

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

static void
check_param  (mpc_datafile_context_t* datafile_context,
              mpc_operand_t* got, mpc_operand_t* expected, mpc_param_t t)
{
  switch (t)
    {
      /*         case NATIVE_INT: */
      /*           break; */
/*     case NATIVE_UL: */
/*       break; */
/*     case NATIVE_L: */
/*       break; */
/*     case NATIVE_D: */
/*       break; */

      /* TODO */
      /*         case GMP_Z: */
      /*           break; */
      /*         case GMP_Q: */
      /*           break; */
      /*         case GMP_F: */
      /*           break; */

/*     case MPFR_INEX: */
/*       break; */
/*     case MPFR: */
/*       break; */

/*     case MPC_INEX: */
/*       break; */
/*     case MPC: */
/*       break; */

    default:
      fprintf (stderr, "check_data: unsupported type.\n");
      exit (1);
    }
}

void
check_data (mpc_datafile_context_t* datafile_context,
            mpc_fun_param_t* params)
{
  int out;
  int total = params->nbout + params->nbin;

  for (out = 0; out < params->nbout; out++)
    {
      check_param (datafile_context, &(params->P[out]),
                   &(params->P[total + out]), params->T[out]);
    }
}
