/* templates.h -- Templated parameters functions.

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

#include "mpc-tests.h"

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
typedef struct {
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
  mpf_t                mpf;
  mpfr_t               mpfr;
  mpfr_rnd_t           mpfr_rnd;
  int                  mpfr_inex;
  mpc_t                mpc;
  mpc_rnd_t            mpc_rnd;
  int                  mpc_inex;
  
  int                  mpc_inex_check[2];
  known_signs_t        known_signs; 
} mpc_operand_t;

/* function parameters */
typedef struct {
    char         *name;  /* name of the function */
    int           nbout; /* number of output parameters */ 
    int           nbin;  /* number of input parameters */
    mpc_operand_t P[10]; /* value of parameters */
    mpc_param_t   T[10]; /* type of parameters */    
} mpc_fun_param_t; 


void    read_description    (mpc_fun_param_t* param, const char *file);
const char* read_description_findname (mpc_param_t e);

/* file functions */
typedef struct {
  char *pathname;
  FILE *fd;
  unsigned long line_number;
  unsigned long test_line_number;
  int nextchar;
} mpc_datafile_context_t;

void    open_datafile       (mpc_datafile_context_t* datafile_context,
                             const char * data_filename);
void    close_datafile      (mpc_datafile_context_t *dc);

/* data file functions */
void    read_line           (mpc_datafile_context_t* datafile_context,
                             mpc_fun_param_t* params);
void    check_data          (mpc_datafile_context_t* datafile_context,
                             mpc_fun_param_t* params, int index_reused_operand);

/* parameters templated functions */
void    init_parameters     (mpc_fun_param_t *params);
void    clear_parameters    (mpc_fun_param_t *params);
void    print_parameter     (mpc_fun_param_t *params, int index);
int     copy_parameter      (mpc_fun_param_t *params,
                             int index_dest, int index_src);

void    tpl_read_int        (mpc_datafile_context_t* datafile_context,
                             int *nread, const char *name);
void    tpl_read_ui         (mpc_datafile_context_t* datafile_context,
                             unsigned long int *ui);
void    tpl_read_si         (mpc_datafile_context_t* datafile_context,
                             long int *si);
void    tpl_skip_whitespace_comments (mpc_datafile_context_t* datafile_context);
void    tpl_read_ternary    (mpc_datafile_context_t* datafile_context,
                             int* ternary);
void    tpl_read_mpfr       (mpc_datafile_context_t* datafile_context,
                             mpfr_ptr x, int *known_sign);
void    tpl_read_mpfr_rnd   (mpc_datafile_context_t* datafile_context,
                             mpfr_rnd_t* rnd);
void    tpl_read_mpfr_inex  (mpc_datafile_context_t* datafile_context,
                             int *nread);
void    tpl_read_mpc_inex   (mpc_datafile_context_t* datafile_context,
                             int *nread);
void    tpl_read_mpc        (mpc_datafile_context_t* datafile_context,
                             mpc_ptr z, known_signs_t *ks);
void    tpl_read_mpc_rnd    (mpc_datafile_context_t* datafile_context,
                             mpc_rnd_t* rnd);

int     tpl_same_mpfr_value (mpfr_ptr x1, mpfr_ptr x2, int known_sign);
int     tpl_same_mpc_value  (mpc_ptr z1, mpc_ptr z2, known_signs_t known_signs);

void    tpl_copy_int        (int *dest, const int * const src);
void    tpl_copy_ui         (unsigned long int *dest,
                             const unsigned long int * const src);
void    tpl_copy_si         (long int *dest, const long int * const src);
void    tpl_copy_d          (double *dest, const double * const src);
void    tpl_copy_mpfr       (mpfr_ptr dest, mpfr_srcptr src);
void    tpl_copy_mpc        (mpc_ptr dest, mpc_srcptr src);

#endif /*__TEMPLATES_H*/
