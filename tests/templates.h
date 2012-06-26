/* templates.h -- Tests parameters functions.

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

#ifndef __TEMPLATES_H
#define __TEMPLATES_H

/* 
    function descriptions 
*/

/* type for return, output and inputs parameters */
typedef enum {
   NATIVE_INT,          /* int */
   NATIVE_UL,           /* unsigned long */
   NATIVE_L,            /* signed long */
   NATIVE_D,            /* double */
   NATIVE_LD,           /* long double */
   NATIVE_DC,           /* double _Complex */
   NATIVE_LDC,          /* long double _Complex */
   NATIVE_IM,           /* intmax_t */
   NATIVE_UIM,          /* uintmax_t */
   GMP_Z,               /* mpz_t */
   GMP_Q,               /* mpq_t */
   GMP_F,               /* mpf_t */
   MPFR_INEX,           /* mpfr_inex */
   MPFR,                /* mpfr_t  */
   MPFR_RND,            /* mpfr_rnd_t */
   MPC_INEX,            /* mpc_inex */
   MPC,                 /* mpc_t */
   MPC_RND              /* mpc_rnd_t */
} mpc_param_t;


/* value of the parameters */
typedef union {
  int                  i;
  unsigned long        ui;
  signed long          si;
  double               d;
  long double          ld;
#ifdef _MPC_H_HAVE_INTMAX_T
  intmax_t             im;
  uintmax_t            uim;
#endif
#ifdef _Complex_I
  double _Complex      dc;
  long double _Complex ldc;
#endif
  mpz_t                mpz;
  mpq_t                mpq;
  mpfr_t               mpfr;
  mpfr_rnd_t           mpfr_rnd;
  mpc_t                mpc;
  mpc_rnd_t            mpc_rnd;
} mpc_operand_t;

/* function parameters */
typedef struct {
    char         *name;  /* name of the function */
    int           nbout; /* number of output parameters */ 
    int           nbin;  /* number of input parameters */
    mpc_operand_t P[10]; /* value of parameters */
    mpc_param_t   T[10]; /* type of parameters */    
} mpc_fun_param_t; 


void read_description(mpc_fun_param_t* param, const char *file);


#endif /*__TEMPLATES_H*/