/* read_line.c -- Read line of test data in file.

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
read_param  (mpc_datafile_context_t* datafile_context,
             mpc_operand_t* p, mpc_param_t t)
{
#if 0
  switch (t)
    {
      /*         case NATIVE_INT: */
      /*           break; */
    case NATIVE_UL:
      read_ui (datafile_context, &(p->ui));
      break;
    case NATIVE_L:
      read_si (datafile_context, &(p->si));
      break;
    case NATIVE_D:
      read_d (datafile_context, &(p->d));
      break;

      /* TODO */
      /*         case GMP_Z: */
      /*           break; */
      /*         case GMP_Q: */
      /*           break; */
      /*         case GMP_F: */
      /*           break; */

    case MPFR_INEX:
      read_mpfr_inex (datafile_context, &(p->mpfr_inex));
      break;
    case MPFR:
      read_mpfr (datafile_context, &(p->mpfr));
      break;

    case MPC_INEX:
      read_mpc_inex (datafile_context, &(p->mpc_inex));
      break;
    case MPC:
      read_mpc (datafile_context, &(p->mpc));
      break;

    default:
      fprintf (stderr, "read_line: unsupported type.\n");
      exit (1);
    }
#endif
}

void
read_line (mpc_datafile_context_t* datafile_context,
           mpc_fun_param_t* params)
{
  int in, out;
  int total = params->nbout + params->nbin;

  datafile_context->test_line_number = datafile_context->line_number;

  for (out = 0; out < params->nbout; out++)

    {
      read_param (datafile_context, &(params->P[total + out]),
                  params->T[total + out]);
    }

  for (in = params->nbout; in < total; in++)
    {
      read_param (datafile_context, &(params->P[in]), params->T[in]);
    }
}
