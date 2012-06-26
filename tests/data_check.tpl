/* data_check.tpl -- template file for checking result against data
   file.

   Usage: Before including this template file in your source file,
   #define the prototype of the function under test in the CALL
   symbol, see tadd_tmpl.c for an example.

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
  }
/*   close_datafile (dc); */

  clear_parameters (&params);

  return 0;
}
