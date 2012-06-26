/* read_description.c -- read parameters from description file

Copyright (C)  2012 INRIA

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

#include <string.h>
#include "mpc-tests.h"
#include "templates.h"

static size_t read_keyworddesc (FILE *fp, char **buffer_ptr, size_t buffer_length, 
                                const char *name);
static mpc_param_t description_findtype (const char *name);

static int nextchar;
static int line_number;

/* read primitives */
static void
skip_line (FILE *fp)
   /* skips characters until reaching '\n' or EOF; */
   /* '\n' is skipped as well                      */
{
   while (nextchar != EOF && nextchar != '\n')
     nextchar = getc (fp);
   if (nextchar != EOF)
     {
       line_number ++;
       nextchar = getc (fp);
     }
}

static void
skip_whitespace (FILE *fp)
   /* skips over whitespace if any until reaching EOF */
   /* or non-whitespace                               */
{
   while (isspace (nextchar))
     {
       if (nextchar == '\n')
         line_number ++;
       nextchar = getc (fp);
     }
}

static void
skip_whitespace_comments2 (FILE *fp)
   /* skips over all whitespace and comments, if any */
{
   skip_whitespace (fp);
   while (nextchar == '#') {
      skip_line (fp);
      if (nextchar != EOF)
         skip_whitespace (fp);
   }
}

/* return the enum associated to name */
mpc_param_t 
description_findtype (const char *name)
{
    /* tuple to link name with the enum value */
    typedef struct  {
        const char *typename;  /* type name */
        mpc_param_t  typeval ;  /* type enum */
    } param_typeval_t;

    /* available types for function description */
    const param_typeval_t sparam_typeval[]= {
       { "int"                 , NATIVE_INT  },
       { "unsigned long int"   , NATIVE_UL   },
       { "long int"            , NATIVE_L    },
       { "double"              , NATIVE_D    },
       { "long double"         , NATIVE_LD   },
       { "double _Complex"     , NATIVE_DC   },
       { "long double _Complex", NATIVE_LDC  },
       { "intmax_t"            , NATIVE_IM   },
       { "uintmax_t"           , NATIVE_UIM  },
       { "mpz_ptr"             , GMP_Z       },
       { "mpz_srcptr"          , GMP_Z       },
       { "mpq_ptr"             , GMP_Q       },
       { "mpq_srcptr"          , GMP_Q       },
       { "mpf_ptr"             , GMP_F       },
       { "mpf_srcptr"          , GMP_F       },
       { "mpfr_inex"           , MPFR_INEX   },
       { "mpfr_ptr"            , MPFR        }, 
       { "mpfr_srcptr"         , MPFR        }, 
       { "mpfr_rnd_t"          , MPFR_RND    },
       { "mpc_inex"            , MPC_INEX    },
       { "mpc_ptr"             , MPC         }, 
       { "mpc_srcptr"          , MPC         }, 
       { "mpc_rnd_t"           , MPC_RND     } 
    };
    
    mpc_param_t r = sparam_typeval[0].typeval;
    size_t s = 0;
    const size_t send = sizeof(sparam_typeval)/sizeof(param_typeval_t);
    
    while (s<send && strcmp(sparam_typeval[s].typename, name)!=0) s++;    

    if (s<send) {
        r = sparam_typeval[s].typeval;
    }
    else {
        printf ("Error: Unable to find the type  '%s'\n",name);
        exit (1);
    }
    return r;
}

/* read the description file and fill param */
void 
read_description (mpc_fun_param_t* param, const char *filename)
{
    FILE *fp;
    const char *pathname = filename;
    char *namestr = NULL;
    char *buffer = NULL;
    size_t len = 0;
    int nbout = 0;
    int nbin = 0;
    int j;
    
    fp = open_data_file (filename);
    nextchar= ' ';
    
    /* read NAME fields */
    skip_whitespace_comments2(fp);
    len = read_keyworddesc (fp, &namestr, len, filename);
    if (namestr==NULL || strcmp(namestr,"NAME:")!=0) {
        printf ("Error: Unable to read 'NAME:' in file '%s'\n",
          filename);
        exit (1);
    }
    
    skip_whitespace_comments2(fp);
    read_keyworddesc (fp, &namestr, len, filename);
    if (namestr==NULL) {
        printf ("Error: Unable to read the name of the function in file '%s'\n",
                filename);
        exit (1);
    }
    param->name = namestr; 
    namestr = NULL;
    
    /* read RETURN fields */
    skip_whitespace_comments2(fp);
    len = 0;
    len = read_keyworddesc (fp, &buffer, len, filename);
    if (buffer==NULL || strcmp(buffer,"RETURN:")!=0) {
        printf ("Error: Unable to read 'NAME:' in file '%s'\n",
          pathname);
        exit (1);
    }
    
    skip_whitespace_comments2(fp);
    len = read_keyworddesc (fp, &buffer, len, filename);
    if (buffer==NULL) {
        printf ("Error: Unable to read the return type of the function in file '%s'\n",
                pathname);
        exit (1);
    }
    param->T[nbout++] = description_findtype (buffer); 
    
    /* read OUPUT fields */
    skip_whitespace_comments2(fp);
    len = read_keyworddesc (fp, &buffer, len, filename);
    if (buffer==NULL || strcmp(buffer,"OUTPUT:")!=0) {
        printf ("Error: Unable to read 'OUTPUT:' in file '%s'\n",
          pathname);
        exit (1);
    }

    while (!feof(fp)) {
        skip_whitespace_comments2(fp);
        len = read_keyworddesc (fp, &buffer, len, filename);
        if (buffer==NULL) {
            printf ("Error: Unable to read the output type of the function in file '%s'\n",
                pathname);
            exit (1);
        }
        if (strcmp(buffer,"INPUT:")==0) {
            break; /* exit the output loop */
        }
        param->T[nbout++] = description_findtype (buffer); 
    }
    
    /* read INPUT fields */
    while (!feof(fp)) {
        skip_whitespace_comments2(fp);
        len = read_keyworddesc (fp, &buffer, len, filename);
        if (buffer==NULL) {
            printf ("Error: Unable to read the input type of the function in file '%s'\n",
                pathname);
            exit (1);
        }
        if (strlen(buffer)==0 && feof(fp)) {
            break;
        }
        param->T[nbout+nbin] = description_findtype (buffer); 
        nbin++;
    }
    free (buffer);
    
    param->nbout = nbout;
    param->nbin = nbin;
    
    /* duplicate output parameters at the end for the expected values */
    for (j=0; j<param->nbout; j++) {
        param->T[nbout+nbin+j] = param->T[j];
    }
}

static size_t
read_keyworddesc (FILE *fp, char **buffer_ptr, size_t buffer_length, const char *name)
{
  size_t pos;
  char *buffer;
  
  pos = 0;
  buffer = *buffer_ptr;

  while (nextchar != EOF && nextchar !='#' && nextchar !='\n')
    {
      if (pos + 1 > buffer_length)
        {
          if (buffer_length==0) buffer_length = pos;
          buffer = (char *) realloc (buffer, 2 * buffer_length);
          if (buffer == NULL)
            {
              printf ("Cannot allocate memory\n");
              exit (1);
            }
          buffer_length *= 2;
        }
      buffer[pos++] = (char) nextchar;
      nextchar = getc (fp);
    }
  while (pos>0 && isspace(buffer[pos-1])) pos--;
  
  if (pos + 1 > buffer_length)
    {
      buffer = (char *) realloc (buffer, buffer_length + 1);
      if (buffer == NULL)
        {
          printf ("Cannot allocate memory\n");
          exit (1);
        }
      buffer_length *= 2;
    }
  buffer[pos] = '\0';
  
  *buffer_ptr = buffer;

  return buffer_length;
}
