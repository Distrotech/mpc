/* Data_check.tpl -- template file for checking result against data
   file.

   Usage: Before including this template file in your source file,
   #define the prototype of the function under test in the
   CALL_MPC_FUNCTION symbol, see tadd_tmpl.c for an example.

   To test the reuse of the first parameter, #define the
   MPC_FUNCTION_CALL_REUSE_OP1 and MPC_FUNCTION_CALL_REUSE_OP2 symbols
   with the first and second input parameter reused as the output, see
   tadd_tmpl.c for an example. It is not possible to test parameter
   reuse in functions with two output (like mpc_sin_cos) with this
   system.

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

int
data_check_template (const char* descr_file, const char * data_file)
{
  mpc_datafile_context_t datafile_context;
  mpc_datafile_context_t *dc = &datafile_context;

  mpc_fun_param_t params;
  mpc_operand_t *P = params.P; /* developer-friendly alias */

  read_description (&params, descr_file);
  init_parameters (&params);

  open_datafile (dc, data_file);
  while (datafile_context.nextchar != EOF) {
    read_line (dc, &params);
    MPC_FUNCTION_CALL;
    check_data (dc, &params);

#ifdef MPC_FUNCTION_CALL_REUSE_OP1
    copy_parameter (&params, 2);
    MPC_FUNCTION_CALL_REUSE_OP1;
    check_data_reuse_op1 (dc, &params);
#endif

#ifdef MPC_FUNCTION_CALL_REUSE_OP2
    copy_parameter (&params, 3);
    MPC_FUNCTION_CALL_REUSE_OP2;
    check_data_reuse_op2 (dc, &params);
#endif
  }
  close_datafile (dc);

  clear_parameters (&params);

  return 0;
}
