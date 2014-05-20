AC_INIT([SISCone], [2.0.5])
AC_CONFIG_SRCDIR([siscone/siscone.cpp])
AM_INIT_AUTOMAKE
dnl uncomment the following line if you want to use autoheader
AM_CONFIG_HEADER(siscone/config.h:config.h.in)
dnl and also uncomment the call to autoheader in autogen.sh

dnl check autoconf version
AC_PREREQ(2.57)

dnl check basic types
AC_CHECK_TYPE(int)
AC_CHECK_TYPE(long)

dnl set default compilation and link flags
dnl those can be changed at configure time so we don't use AM_CXXFLAGS here
test "x${CXXFLAGS+yes}" = xyes || CXXFLAGS="-O3 -Wall -ffast-math"
dnl CXXFLAGS=" -Wall -O3 -ffast-math "

dnl check useful programs
AC_PROG_CXX
AC_PROG_INSTALL
AC_PROG_LN_S
AC_PROG_MAKE_SET

dnl check standard C headers
AC_STDC_HEADERS

dnl set the default destination directory
AC_PREFIX_DEFAULT(/usr/local)

dnl if the debug flag is set, build with -g
dnl default is "yes"
AC_ARG_ENABLE(debug, 
	[  --enable-debug      Turn on debug compiler information],
	[ENABLE_DEBUG_FLAG="$enableval"],
	[ENABLE_DEBUG_FLAG="yes"])
if [[ "x$ENABLE_DEBUG_FLAG" == "xyes" ]] ; then
   CXXFLAGS=${CXXFLAGS}" -g "
fi

dnl uncomment the next line not to build the shared lib by default
dnl AM_DISABLE_SHARED

dnl-----------------
dnl check libraries
dnl ----------------

dnl math lib
AC_CHECK_LIB(m, cos)
dnl already included into LIBS by the previous check
dnl AM_LDFLAGS=" -lm "

dnl if the debug flags are on, check if we can also use 
dnl some profiling tools
dnl COMMENTED: Pass LDFLAGS to configure instead
dnl if [[ "x$ENABLE_DEBUG_FLAG" == "xyes" ]] ; then
dnl    AC_CHECK_LIB(profiler, google_initializer_module_profiler)
dnl    AC_CHECK_LIB(pthread, pthread_create)
dnl    AC_CHECK_LIB(tcmalloc, malloc)
dnl fi

dnl enable libtool
AC_PROG_LIBTOOL

AC_OUTPUT( Makefile siscone/Makefile siscone/spherical/Makefile examples/Makefile examples/events/Makefile )
