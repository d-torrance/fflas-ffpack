# Copyright (c) 2011 Fflas-Ffpack
# This file is part of Fflas-Ffpack
# written by BB <bboyer@imag.fr>
# see COPYING for licence

AC_PREREQ(2.61)

AC_INIT([Fflas-Ffpack], [1.4.0],[ffpack-devel@googlegroups.com],[fflasffpack],
		[http://www.linalg.org/])
AC_CONFIG_MACRO_DIR([macros])
AC_CONFIG_AUX_DIR([build-aux])
AM_INIT_AUTOMAKE([1.8 gnu no-dependencies -Wall -Wno-portability])
AM_CONFIG_HEADER([config.h])
AX_PREFIX_CONFIG_H(fflas-ffpack/fflasffpack-config.h, __FFLAFLAS)
AC_PATH_PROG(RM, rm, $FALSE)
RM="$RM -f"


AM_MAINTAINER_MODE
AM_DEP_TRACK
AM_OUTPUT_DEPENDENCY_COMMANDS

AM_ACLOCAL_INCLUDE(macros)

# work around to fix the backward compatibility issue of automake 1.10 with 1.9 (pb with MKDIR_P)
AC_SUBST([MKDIR_P])


# Give a sane default for CFLAGS and CXXFLAGS
# The sane thing is to define the flags differenly in tests, examples, interfaces, etc.  -bds
#DEFAULT_CFLAGS="-O2 -Wall"
#DEFAULT_CXXFLAGS="-std=c++0x"

# CFLAGS=${CFLAGS:-$DEFAULT_CFLAGS}
# CXXFLAGS=${CXXFLAGS:-$DEFAULT_CXXFLAGS}

AC_SUBST([DEFAULT_CFLAGS])
AC_SUBST([DEBUG_CFLAGS])
AC_SUBST([TESTS_CFLAGS])

TESTS_CFLAGS="-O0"
DEFAULT_CFLAGS="-O2 -pipe"
DEBUG_CFLAGS="-g -Wall"

# disable default -g -O2 CXXFLAGS
: ${CXXFLAGS=""}

#set CXX
AC_PROG_CC([gcc icc])
AC_PROG_CXX([g++ icpc icc])

if  test "x${CXX}" = "xicpc" -o "x${CXX}" = "xicc"  ; then
	DEBUG_CFLAGS="${DEBUG_CFLAGS} -Wbrief -Wpointer-arith -Wuninitialized"
	# DEBUG_CFLAGS+="-Wcheck"
	# DEFAULT_CFLAGS+="-fast"
	#warning #2259: non-pointer conversion from "XXX" to "YYY" may lose significant bits -> -wd2259
else
	if test "x${CXX}" = "xg++"  ; then
		DEBUG_CFLAGS="${DEBUG_CFLAGS} -Wextra -Wno-unused-parameter"
	else
		AC_MSG_ERROR([please set environment variable 'CXX' to 'g++' or 'icpc']) ;
	fi
fi

# CFLAGS=${CFLAGS:-$(DEFAULT_CFLAGS)}
# CXXFLAGS="${DEFAULT_CFLAGS} ${DEBUG_CFLAGS}"
DEFAULT_CFLAGS="${DEFAULT_CFLAGS} ${DEBUG_CFLAGS}"
TESTS_CFLAGS="${TESTS_CFLAGS} ${DEBUG_CFLAGS}"


AC_PROG_CC
AC_PROG_CXX
AC_HEADER_STDC
#AC_DISABLE_SHARED
#AC_DISABLE_STATIC
#AM_PROG_LIBTOOL
# JGD 08.06.2004 : new name
AC_PROG_LIBTOOL

# newer libtool...
LT_PREREQ([2.2])
LT_INIT




# Machine characteristics

AC_CHECK_SIZEOF(char, 8)
AC_CHECK_SIZEOF(short, 16)
AC_CHECK_SIZEOF(int, 32)
AC_CHECK_SIZEOF(long, 32)
AC_CHECK_SIZEOF(long long, 64)
AC_CHECK_SIZEOF(__int64, 64)

# Checks for header files.
AC_HEADER_STDC
AC_CHECK_HEADERS([float.h limits.h stddef.h stdlib.h string.h sys/time.h stdint.h])


# check endianness of the architecture
AC_C_BIGENDIAN(
  [AC_DEFINE(HAVE_BIG_ENDIAN, 1, [Define that architecture uses big endian storage])],
  [AC_DEFINE(HAVE_LITTLE_ENDIAN, 1, [Define that architecture uses little endian storage])],
  [])

# Create some useful data types of fixed, known lengths

# We hereby assume that a character is always one byte
LINBOX_INT8="char";

case $ac_cv_sizeof_char in
  1)
     TWO_BYTES=2;
     FOUR_BYTES=4;
     EIGHT_BYTES=8;
     ;;
  8)
     TWO_BYTES=16;
     FOUR_BYTES=32;
     EIGHT_BYTES=64;
esac

case $TWO_BYTES in
  $ac_cv_sizeof_short)
     LINBOX_INT16="short";
     ;;
  $ac_cv_sizeof_int)
     LINBOX_INT16="int";
     ;;
esac

case $FOUR_BYTES in
  $ac_cv_sizeof_short)
     LINBOX_INT32="short";
     ;;
  $ac_cv_sizeof_int)
     LINBOX_INT32="int";
     ;;
  $ac_cv_sizeof_long)
     LINBOX_INT32="long";
     ;;
esac

case $EIGHT_BYTES in
  $ac_cv_sizeof_short)
     LINBOX_INT64="short";
     ;;
  $ac_cv_sizeof_int)
     LINBOX_INT64="int";
     ;;
  $ac_cv_sizeof_long)
     LINBOX_INT64="long";
     ;;
  $ac_cv_sizeof_long_long)
     LINBOX_INT64="long long";
     ;;
  $ac_cv_sizeof___int64)
     LINBOX_INT64="__int64";
     ;;
esac

AC_DEFINE_UNQUOTED(INT8, $LINBOX_INT8, Canonical 8-bit data type)
AC_DEFINE_UNQUOTED(INT16, $LINBOX_INT16, Canonical 16-bit data type)
AC_DEFINE_UNQUOTED(INT32, $LINBOX_INT32, Canonical 32-bit data type)
AC_DEFINE_UNQUOTED(INT64, $LINBOX_INT64, Canonical 64-bit data type)

# Feature checks
FF_MISC
# LB_DRIVER

AC_LANG_CPLUSPLUS


LB_CHECK_GMP(,,[
echo '*******************************************************************************'
echo ' ERROR: GMP not found!'
echo
echo ' GMP version 3.1.1 or greater with --enable-cxx is required for this library to compile. Please'
echo ' make sure GMP is installed and specify its location with the option'
echo ' --with-gmp=<prefix> when running configure.'
echo '*******************************************************************************'
exit 1
])

# LB_CHECK_IML

# LB_CHECK_NTL
LB_CHECK_GIVARO(,,[
echo '*******************************************************************************'
echo ' ERROR: GIVARO not found!'
echo
echo ' GIVARO version 3.4.0 or greater (<3.5) is required for this library to compile.'
echo ' Please make sure GIVARO is installed and specify its location with the'
echo ' option --with-givaro=<prefix> when running configure.'
echo '*******************************************************************************'
exit 1
])
# LB_CHECK_SACLIB
# LB_CHECK_LIDIA
# LB_CHECK_MAPLE
# LB_CHECK_SAGE
#LB_CHECK_ATLAS
FF_CHECK_BLAS(,,[
echo ''
echo '*******************************************************************************'
echo ' ERROR: BLAS not found!'
echo
echo ' BLAS routines are required for this library to compile. Please'
echo ' make sure BLAS are installed and specify its location with the option'
echo ' --with-blas=<lib> when running configure.'
echo '*******************************************************************************'
exit 1
])
# LB_CHECK_EXPAT

# LB_BENCH

FF_OPT

FF_DOC


# if test ! -d ./benchmarks/data  ; then
	# echo "Creating data dir in benchmark" ;
	# mkdir ./benchmarks/data  ;
# fi

# this is bogus so far as I can tell.  -bds
#CXXFLAGS="${GMP_CFLAGS} ${NTL_CFLAGS} ${GIVARO_CFLAGS} ${SACLIB_CFLAGS} ${CXXFLAGS}"
#AC_SUBST(CXXFLAGS)

CXXFLAGS="${GMP_CFLAGS} ${CXXFLAGS}"
AC_SUBST(CXXFLAGS)


AC_CONFIG_FILES([
Makefile
macros/Makefile
fflasffpack-config
fflas-ffpack/Makefile
fflas-ffpack/fflas/Makefile
fflas-ffpack/ffpack/Makefile
fflas-ffpack/field/Makefile
doc/Makefile
tests/Makefile
benchmark/Makefile
])
AC_OUTPUT