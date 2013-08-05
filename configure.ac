dnl $Id$
dnl
dnl This is main autoconf bootstrap script of libLAS project.
dnl
m4_define([laszip_version_major], [2])
m4_define([laszip_version_minor], [2])
m4_define([laszip_version_micro], [0])
m4_define([laszip_version],
          [laszip_version_major.laszip_version_minor.laszip_version_micro])

AC_PREREQ([2.59])
AC_INIT([laszip], [laszip_version], [howard@hobu.co],[laszip-src])
AC_CANONICAL_BUILD

RELEASE_VERSION=laszip_version
AC_SUBST([RELEASE_VERSION])

dnl #########################################################################
dnl Default compilation flags
dnl #########################################################################

m4_define([debug_default],[no])

CFLAGS="-Wall -Wno-long-long -pedantic $CFLAGS"
CXXFLAGS="-Wall -Wno-long-long -pedantic -std=c++98 $CXXFLAGS"

dnl #########################################################################
dnl Checks for programs.
dnl #########################################################################

AM_INIT_AUTOMAKE([dist-bzip2])
AC_PROG_CXX
AC_PROG_CXXCPP
AC_PROG_INSTALL
AC_PROG_LN_S
AC_PROG_MAKE_SET
AC_PROG_LIBTOOL

dnl #########################################################################
dnl Check platform endianness
dnl #########################################################################

AC_C_BIGENDIAN

dnl #########################################################################
dnl Checks for header files.
dnl #########################################################################

AC_CHECK_HEADERS([string.h],, [AC_MSG_ERROR([cannot find string.h, bailing out])])
AC_CHECK_HEADERS([stdio.h],, [AC_MSG_ERROR([cannot find stdio.h, bailing out])])
AC_CHECK_HEADERS([stdlib.h],, [AC_MSG_ERROR([cannot find stdlib.h, bailing out])])


LIBS="${LIBS}"


dnl #########################################################################
dnl Build mode configuration (debug/optimized)
dnl #########################################################################

AC_ARG_ENABLE([debug],
    AC_HELP_STRING([--enable-debug=ARG], [Enable debug compilation mode @<:@yes|no@:>@, default=debug_default]),,)
    
AC_MSG_CHECKING([for debug enabled])

if test "x$enable_debug" = "xyes"; then
    CFLAGS="$CFLAGS -g -DDEBUG"
    CXXFLAGS="$CXXFLAGS -g -DDEBUG"
    AC_MSG_RESULT(yes)
else
    CFLAGS="$CFLAGS -O3 -DNDEBUG"
    CXXFLAGS="$CXXFLAGS -O3 -DNDEBUG"
    AC_MSG_RESULT(no)
fi

dnl #########################################################################
dnl Definiion of custom Autoconf macros
dnl #########################################################################

AC_DEFUN([LOC_MSG],[
echo "$1"
])

AC_DEFUN([AC_HAVE_LONG_LONG],
[
    AC_MSG_CHECKING([for 64bit integer type])

    echo 'int main() { long long off=0; }' >> conftest.c
    if test -z "`${CC} -o conftest conftest.c 2>&1`" ; then
        AC_DEFINE(HAVE_LONG_LONG, 1, [Define to 1, if your compiler supports long long data type])
        AC_MSG_RESULT([long long])
    else
        AC_MSG_RESULT([no])
    fi
    rm -f conftest*
])




dnl #########################################################################
dnl Determine other features of compiler
dnl #########################################################################

AC_HAVE_LONG_LONG

dnl #########################################################################
dnl Checks for library functions.
dnl #########################################################################

AC_CHECK_FUNCS([gettimeofday bzero memset memcpy bcopy])

dnl #########################################################################
dnl Generate makefiles
dnl #########################################################################

AC_CONFIG_FILES([
	Makefile
	include/laszip/Makefile
	src/Makefile
	tools/Makefile
	tools/laszip-config

])

AC_OUTPUT

dnl #########################################################################
dnl Print configuration summary
dnl #########################################################################

LOC_MSG()
LOC_MSG([laszip configuration summary:])
LOC_MSG()
LOC_MSG([  Version..................: ${RELEASE_VERSION}])
LOC_MSG([  Installation directory...: ${prefix}])
LOC_MSG([  C compiler...............: ${CC} ${CFLAGS}])
LOC_MSG([  C++ compiler.............: ${CXX} ${CXXFLAGS}])
LOC_MSG([  Debugging support........: ${enable_debug}])

LOC_MSG()
LOC_MSG([  LIBS.....................: ${LIBS}])
LOC_MSG()
LOC_MSG()

dnl EOF
