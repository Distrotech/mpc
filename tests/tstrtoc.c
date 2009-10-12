/* test file for mpc_strtoc.

Copyright (C) 2009 Philippe Th\'eveny, Andreas Enge

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

#include <string.h>
#include <stdlib.h>

#include "mpc-tests.h"

extern unsigned long line_number;
extern int nextchar;
extern char *pathname;

/* names of rounding modes */
extern const char *rnd_mode[];

static void
check_file (const char* file_name)
{
  FILE *fp;

  size_t str_len = 255;
  char *str = NULL;
  size_t rstr_len = 255;
  char *rstr = NULL;
  char *end = NULL;

  int base;
  int inex_re;
  int inex_im;
  mpc_t expected, got;
  mpc_rnd_t rnd = MPC_RNDNN;
  int inex = 0, inex_expected;
  known_signs_t ks = {1, 1};


  fp = open_data_file (file_name);
  
  /* initializations */
  str = (char *) malloc (str_len);
  if (str == NULL)
    {
      printf ("Cannot allocate memory\n");
      exit (1);
    }
  rstr = (char *) malloc (rstr_len);
  if (rstr == NULL)
    {
      printf ("Cannot allocate memory\n");
      exit (1);
    }
  mpc_init2 (expected, 53);
  mpc_init2 (got, 53);

  /* read data file */
  line_number = 1;
  nextchar = getc (fp);
  while (nextchar != EOF)
    {
      skip_whitespace_comments (fp);

      /* 1. read a line of data: expected result, base, rounding mode */
      read_ternary (fp, &inex_re);
      read_ternary (fp, &inex_im);
      read_mpc (fp, expected, NULL);
      if (inex_re == TERNARY_ERROR || inex_im == TERNARY_ERROR)
         inex_expected = -1;
      else
         inex_expected = MPC_INEX (inex_re, inex_im);

      str_len = read_string (fp, &str, str_len, "number string");
      rstr_len = read_string (fp, &rstr, rstr_len, "string remainder");
      read_int (fp, &base, "base");
      read_mpc_rounding_mode (fp, &rnd);

      /* 2. convert string at the same precision as the expected result */
      mpfr_set_prec (MPC_RE (got), MPC_PREC_RE (expected));
      mpfr_set_prec (MPC_IM (got), MPC_PREC_IM (expected));
      inex = mpc_strtoc (got, str, &end, base, rnd);

      /* 3. compare this result with the expected one */
      if (inex != inex_expected
          || !same_mpc_value (got, expected, ks)
          || strcmp (end, rstr) != 0)
        {
          printf ("mpc_strtoc(str) failed (line %lu)\nwith base=%d and "
                  "rounding mode %s\n", line_number-1, base, rnd_mode[rnd]);
          if (inex != MPC_INEX (inex_re, inex_im))
            printf ("ternary value: got %s, expected (%s, %s)\n",
                    MPC_INEX_STR (inex),
                    (inex_re == +1 ? "+1" : (inex_re == -1 ? "-1" : "0")),
                    (inex_im == +1 ? "+1" : (inex_im == -1 ? "-1" : "0")));
          printf ("str = \"%s\"\n", str);
          if (strcmp (end, rstr) != 0)
            printf ("string remainder expected \"%s\"\n"
                    "                 got      \"%s\"\n",
                    rstr, end);
          else
            {
              printf ("     ");
              OUT (got);
              OUT (expected);
            }
          exit (1);
        }

      end = NULL;
    }

  mpc_clear (expected);
  mpc_clear (got);
  if (str != NULL)
    free (str);
  if (rstr != NULL)
    free (rstr);
  close_data_file (fp);
}

static void
check_null (void)
{
  int inex;
  char *end;
  mpc_t z;

  mpc_init2 (z, 53);

  inex = mpc_strtoc (z, NULL, &end, 10, MPC_RNDNN);
  if (end != NULL || inex != -1 || mpfr_nan_p (MPC_RE (z)) == 0
      || mpfr_nan_p (MPC_IM (z)) == 0)
    {
      printf ("Error: mpc_strtoc(z, NULL) with a NULL pointer should fail"
              " and the z value should be set to NaN +I*NaN\ngot ");
      OUT (z);
      exit (1);
    }

  mpc_clear (z);
}

int
main (void)
{
  check_null ();
  check_file ("strtoc.dat");
  return 0;
}
