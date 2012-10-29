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
  switch (t)
    {
    case NATIVE_INT: 
      tpl_read_int (datafile_context, &(p->i),"");
      break;
    case NATIVE_UL:
      tpl_read_ui (datafile_context, &(p->ui));
      break;
    case NATIVE_L:
      tpl_read_si (datafile_context, &(p->si));
      break;

#if 0
      /* TODO */
    case NATIVE_D:
      break;

    case GMP_Z:
      break;
    case GMP_Q:
      break;
    case GMP_F:
      break;
#endif

    case MPFR_INEX:
      tpl_read_mpfr_inex (datafile_context, &(p->mpfr_inex));
      break;
      
    case MPFR:
      tpl_read_mpfr (datafile_context, p->mpfr, &(p->known_signs.re));
      break;

    case MPFR_RND:
      tpl_read_mpfr_rnd (datafile_context, &p->mpfr_rnd);
      break;

    case MPC_INEX:
      tpl_read_mpc_inex (datafile_context, p->mpc_inex_check);
      break;
    case MPC:
      tpl_read_mpc (datafile_context, p->mpc, &(p->known_signs));
      break;
    case MPC_RND:
      tpl_read_mpc_rnd (datafile_context, &p->mpc_rnd);
      break;

    default:
      fprintf (stderr, "read_line: unsupported type.\n");
      exit (1);
    }
}

static void
set_precision (mpc_fun_param_t* params, int index)
{
  /* set output precision to reference precision */
  int index_ref = index + params->nbout + params->nbin;

  switch (params->T[index])
    {
    case GMP_F:
      fprintf (stderr, "set_precision: unsupported type.\n");
      exit (1);

    case MPFR:
      mpfr_set_prec (params->P[index].mpfr,
                     mpfr_get_prec (params->P[index_ref].mpfr));
      break;

    case MPC:
      mpfr_set_prec (mpc_realref (params->P[index].mpc),
                     MPC_PREC_RE (params->P[index_ref].mpc));
      mpfr_set_prec (mpc_imagref (params->P[index].mpc),
                     MPC_PREC_IM (params->P[index_ref].mpc));
      break;
    }
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
      set_precision (params, out);
    }

  for (in = params->nbout; in < total; in++)
    {
      read_param (datafile_context, &(params->P[in]), params->T[in]);
    }
}

/* read primitives */
static void
tpl_skip_line (mpc_datafile_context_t* datafile_context)
   /* skips characters until reaching '\n' or EOF; */
   /* '\n' is skipped as well                      */
{
   while (datafile_context->nextchar != EOF && datafile_context->nextchar != '\n')
     datafile_context->nextchar = getc (datafile_context->fd);
   if (datafile_context->nextchar != EOF)
     {
       datafile_context->line_number ++;
       datafile_context->nextchar = getc (datafile_context->fd);
     }
}

static void
tpl_skip_whitespace (mpc_datafile_context_t* datafile_context)
   /* skips over whitespace if any until reaching EOF */
   /* or non-whitespace                               */
{
   while (isspace (datafile_context->nextchar))
     {
       if (datafile_context->nextchar == '\n')
         datafile_context->line_number ++;
       datafile_context->nextchar = getc (datafile_context->fd);
     }
}

void
tpl_skip_whitespace_comments (mpc_datafile_context_t* datafile_context)
   /* skips over all whitespace and comments, if any */
{
   tpl_skip_whitespace (datafile_context);
   while (datafile_context->nextchar == '#') {
      tpl_skip_line (datafile_context);
      if (datafile_context->nextchar != EOF)
         tpl_skip_whitespace (datafile_context);
   }
}

/* All following read routines skip over whitespace and comments; */
/* so after calling them, nextchar is either EOF or the beginning */
/* of a non-comment token.                                        */
void
tpl_read_ternary (mpc_datafile_context_t* datafile_context, int* ternary)
{
  switch (datafile_context->nextchar)
    {
    case '!':
      *ternary = TERNARY_ERROR;
      break;
    case '?':
      *ternary = TERNARY_NOT_CHECKED;
      break;
    case '+':
      *ternary = +1;
      break;
    case '0':
      *ternary = 0;
      break;
    case '-':
      *ternary = -1;
      break;
    default:
      printf ("Error: Unexpected ternary value '%c' in file '%s' line %lu\n",
              datafile_context->nextchar, datafile_context->pathname, datafile_context->line_number);
      exit (1);
    }

  datafile_context->nextchar = getc (datafile_context->fd);
  tpl_skip_whitespace_comments (datafile_context);
}