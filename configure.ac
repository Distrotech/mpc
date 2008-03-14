#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ(2.61)
AC_INIT(mpc, 0.4.6, mpc@loria.fr)
AC_CONFIG_SRCDIR([set_d_d.c])
AC_CONFIG_HEADER([config.h])

AM_INIT_AUTOMAKE([1.9 foreign])
AM_MAINTAINER_MODE

# Checks for programs.
AC_PROG_CC
AC_PROG_LIBTOOL

# Checks for libraries.
AC_CHECK_LIB([mpfr], [mpfr_clear])

# Checks for header files.
AC_HEADER_STDC
AC_CHECK_HEADERS([limits.h stdlib.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_C_CONST
AC_TYPE_SIZE_T

# Checks for library functions.

AC_CONFIG_FILES([Makefile])
AC_OUTPUT
