AC_INIT([FastJet],[3.0.3])
AC_CONFIG_SRCDIR([src/JetDefinition.cc])
AM_INIT_AUTOMAKE(tar-ustar)

dnl Make sure users can't install in build directory
if test "$prefix" = "$PWD"; then
  AC_MSG_ERROR([Installation into the build directory is not supported: use a different --prefix argument])
fi

dnl (un_comment the following lines if you want to use autoheader
dnl and also (un_comment the call to autoheader in autogen.sh
dnl 
dnl Notes:
dnl  - the first macro needs to place the files in the correct 
dnl    dir in order to have the correct -I options for the remote build
dnl  - see Makefile.am for distcleaning AX_PREFIX_CONFIG_H leftovers
AM_CONFIG_HEADER(include/fastjet/config_raw.h:config.h.in)
AX_PREFIX_CONFIG_H(include/fastjet/config_auto.h,FASTJET,include/fastjet/config_raw.h)

dnl check autoconf version
dnl
dnl 2011-07-07 tested explicitly to be working with
dnl   autoconf-2.59 (lxplus)
dnl   autoconf-2.61 (karnak)
dnl   autoconf-2.65 (ipht)
dnl   autoconf-2.68 (virtual&lpthe)
AC_PREREQ(2.59)

dnl load our M4 macro directory
dnl  - this is recommended by latest versions of libtoolize
dnl  - this is equivalent to adding ACLOCAL_AMFLAGS = -I m4 in Makefile.am
dnl  - the libtoolize manpage says that for the moment it's more 
dnl    portable to keep using ACLOCAL_AMFLAGS.
dnl  - the autoconf manual mentions that since we use 'aclocal -I m4'
dnl    we need ACLOCAL_AMFLAGS anyway
dnl as a consequence, we don't use AC_CONFIG_MACRO_DIR for the moment
dnl Question: is it compatible with older versions of libtool?
dnl   AC_CONFIG_MACRO_DIR([m4])

dnl check basic types
AC_CHECK_TYPE(int)
AC_CHECK_TYPE(long)

dnl-------------------------------------------------------------------
dnl if the user has not set CXXFLAGS, then set our own default
dnl-------------------------------------------------------------------
USE_CUSTOM_FLAGS="yes"
if [[ "x$CXXFLAGS" == "x" ]]; then
   CXXFLAGS="-O3 -Wall"
   USE_CUSTOM_FLAGS="no"
fi

# test "x${CXXFLAGS+yes}" == "xyes" || CXXFLAGS="-O3 -Wall"

dnl-------------------------------------------------------------------
dnl check useful programs
dnl-------------------------------------------------------------------
AC_PROG_CXX

dnl ensure CXX presence
cxx_progname=$(basename $CXX)
cxx_dirname=$(dirname $CXX)
AC_CHECK_PROG([cxx_found],[$cxx_progname],[yes],[no].[$cxx_dirname$PATH_SEPARATOR$PATH])
if test "x$cxx_found" == "xno"; then
   AC_MSG_ERROR([No functional C++ compiler found])
fi

AC_PROG_INSTALL
AC_PROG_LN_S
AC_PROG_SED
AC_PROG_MAKE_SET

AM_CXXFLAGS=""

dnl check standard C headers
AC_STDC_HEADERS

dnl set the default destination directory
AC_PREFIX_DEFAULT(/usr/local)

dnl-------------------------------------------------------------------
dnl initialize compilation and link flags to set in fastjet-config
dnl-------------------------------------------------------------------
CONFIG_CXXFLAGS=""
CONFIG_RUNPATH_FLAGS=""
CONFIG_LIBS=""
CONFIG_LIBS_PLUGINS=""
CONFIG_LIBS_PLUGINS_STATIC=""

dnl-------------------------------------------------------------------
dnl deal with debug option
dnl-------------------------------------------------------------------
dnl if the debug flag is set, build with -g
dnl default is "yes"
AC_ARG_ENABLE(debug,
	[  --enable-debug          Turn on debug compiler information [default=yes]],
	[ENABLE_DEBUG_FLAG="$enableval"],
	[ENABLE_DEBUG_FLAG="yes"])
if [[ "$ENABLE_DEBUG_FLAG" == "yes" ]] ; then
   AM_CXXFLAGS=${AM_CXXFLAGS}" -g "

   dnl-------------------------------------------------------------------
   dnl check if the compiler supports -Woverloaded-virtual
   dnl-------------------------------------------------------------------
   AC_MSG_CHECKING([if $CXX supports -Woverloaded-virtual flag])
   SAVE_CXXFLAGS="$CXXFLAGS"
   CXXFLAGS="-Woverloaded-virtual"
   AC_LANG_PUSH(C++)
   AC_COMPILE_IFELSE([AC_LANG_SOURCE([int main(void){ return 0;}])],
       [ac_overloaded_virtual_ok=yes],[ac_overloaded_virtual_ok=no])
   AC_LANG_POP(C++)
   CXXFLAGS="$SAVE_CXXFLAGS"
   AC_MSG_RESULT([$ac_overloaded_virtual_ok])
   if [[ "x${ac_overloaded_virtual_ok}" == "xyes" ]]; then
      AM_CXXFLAGS=${AM_CXXFLAGS}" -Woverloaded-virtual "
   fi
fi

dnl check if one wants a high level of warnings
AC_ARG_ENABLE(extra-warnings,
	[  --enable-extra-warnings Turn on additional compiler warnings [default=no]],
	[ENABLE_EXTRA_WARNINGS_FLAG="$enableval"],
	[ENABLE_EXTRA_WARNINGS_FLAG="no"])
if [[ "$ENABLE_EXTRA_WARNINGS_FLAG" == "yes" ]] ; then
   AM_CXXFLAGS=${AM_CXXFLAGS}" -ansi -pedantic -Wextra -Wshadow "
fi

dnl-------------------------------------------------------------------
dnl deal with the choice of shared/static libs
dnl-------------------------------------------------------------------
dnl build the shared lib by default
dnl even though they're enabled by default, AM_ENABLE_SHARED has to
dnl be called so that ${enable_shared} is set to 'yes' instead of ''
AM_ENABLE_SHARED
dnl
dnl comment the previous line and uncomment this one if you want shared 
dnl libs to be disabled by default
dnl AM_DISABLE_SHARED
HAS_SHARED=${enable_shared}

dnl-------------------------------------------------------------------
dnl check headers
dnl-------------------------------------------------------------------
dnl execinfo.h (use in Error.cc to have the backtrace)
AC_CHECK_HEADERS(execinfo.h)

dnl-------------------------------------------------------------------
dnl check libraries
dnl-------------------------------------------------------------------
dnl math lib
AC_CHECK_LIB(m, cos)
CONFIG_CXXFLAGS=""
CONFIG_LIBS=${CONFIG_LIBS}" -lm "

dnl-------------------------------------------------------------------
dnl deal with CGAL
dnl-------------------------------------------------------------------
CGAL_CPPFLAGS=""
CGAL_LIBS=""

dnl first check if the user wants to use CGAL
dnl default value is "no"
dnl Note that CGAL compilation flags are considered as required
dnl so we do store them in "AM_CXXFLAGS"
AC_ARG_ENABLE(cgal,
       [  --enable-cgal           enables link with the CGAL library [default=no]],
       [ENABLE_CGAL_FLAG="$enableval"],
       [ENABLE_CGAL_FLAG="no"])
dnl if asked for, check if the CGAL library can be used
if [[ "x${ENABLE_CGAL_FLAG}" == "xyes" ]] ; then
   dnl if the search for CGAL has been explicitly asked and not found, return an error
   dnl ACX_CHECK_CGAL([],[ENABLE_CGAL_FLAG="no"])
   ACX_CHECK_CGAL([],AC_MSG_ERROR([CGAL explicitly requested and not found or not functional]))
fi

dnl update compilation/link flags for the CGAL lib
if [[ "x$ENABLE_CGAL_FLAG" == "xyes" ]] ; then
   AM_CXXFLAGS=${AM_CXXFLAGS}" $CGAL_CPPFLAGS "
   CONFIG_LIBS=${CONFIG_LIBS}" $CGAL_LDFLAGS $CGAL_LIBS "
else
   AM_CXXFLAGS=${AM_CXXFLAGS}" -DDROP_CGAL "
dnl   CONFIG_CXXFLAGS=${CONFIG_CXXFLAGS}" -DDROP_CGAL "
fi
AC_SUBST(CGAL_CPPFLAGS)
AC_SUBST(CGAL_LIBS)

dnl-------------------------------------------------------------------
dnl check if the linker supports --enable-new-dtags (rpath-related)
dnl-------------------------------------------------------------------
AC_MSG_CHECKING([if $CXX supports -Wl,--enable-new-dtags flag])
SAVE_CXXFLAGS="$CXXFLAGS"
CXXFLAGS="-Werror -Wl,--enable-new-dtags"
AC_LANG_PUSH(C++)
AC_LINK_IFELSE([AC_LANG_SOURCE([int main(void){ return 0;}])],
   [ac_enable_new_dtags_ok=yes],[ac_enable_new_dtags_ok=no])
AC_LANG_POP(C++)
CXXFLAGS="$SAVE_CXXFLAGS"
AC_MSG_RESULT([$ac_enable_new_dtags_ok])
HAS_RUNPATH_SUPPORT="no"
CONFIG_RUNPATH_FLAGS=""
dnl if test x"$ac_enable_new_dtags_ok" = x"yes"; then
if [[ "x${ac_enable_new_dtags_ok}" == "xyes" ]]; then
   HAS_RUNPATH_SUPPORT="yes"
   CONFIG_RUNPATH_FLAGS="-Wl,--enable-new-dtags"
fi
AC_SUBST(HAS_RUNPATH_SUPPORT)
AC_SUBST(CONFIG_RUNPATH_FLAGS)

dnl substitutions
AC_SUBST(AM_CXXFLAGS)

dnl create the substitutions variables for the configuration script
AC_SUBST(PACKAGE_VERSION)
AC_SUBST(HAS_SHARED)

dnl-------------------------------------------------------------------
dnl plugin section
dnl
dnl each plugin can be enabled or disabled
dnl We also define a ENABLED_PLUGIN_<name> in config.h when plugin
dnl <name> is enabled
dnl WARNING: for make dist reasons all Makefiles MUST be created
dnl
dnl See m4/plugins.m4 to see what is enabled by default
dnl-------------------------------------------------------------------
dnl general
AC_CONFIG_FILES( plugins/Makefile )
CONFIG_LIBS_PLUGINS=""
CONFIG_LIBS_PLUGINS_STATIC=""

dnl deal with a couple of arguments to allow a whole bunch of plugins at a time
ACX_INIT_PLUGINS
ACX_CHECK_ALLPLUGINS
ACX_CHECK_ALLCXXPLUGINS

dnl initialise the list of allowed plugins
dnl This is meant to enable fastjet-config --list-plugins
LIST_ALL_PLUGINS=""

dnl allow to build all plugins in the same lib
AC_ARG_ENABLE(monolithic,
[  --enable-monolithic  Build all the (compiled) plugins in a single lib [default=yes]],
[case "${enableval}" in
  yes) monolithic=yes ;;
  no)  monolithic=no ;;
  *) AC_MSG_ERROR(bad value ${enableval} for --enable-monolithic) ;;
esac],[monolithic=yes])
AM_CONDITIONAL(MONOLITHIC_PLUGINS, test x$monolithic = xyes)
if [[ "x$monolithic" == "xyes" ]] ; then
   CONFIG_LIBS_PLUGINS=" -lfastjetplugins "
   CONFIG_LIBS_PLUGINS_STATIC=" \${installationdir}/lib/libfastjetplugins.a "
fi

dnl SISCone
ACX_CHECK_PLUGIN(SISCone, siscone, SISCONE, yes,
	         [CONFIG_LIBS_PLUGINS=${CONFIG_LIBS_PLUGINS}" -lsiscone_spherical -lsiscone"
                  CONFIG_LIBS_PLUGINS_STATIC=${CONFIG_LIBS_PLUGINS_STATIC}" \${installationdir}/lib/libsiscone.a \${installationdir}/lib/libsiscone_spherical.a"])
AC_CONFIG_SUBDIRS(plugins/SISCone/siscone)
dnl alternatively, to build SISCone directly from the FastJet build
dnl system, comment out the previoous lines and uncomment the ones
dnl below. Note that the main drawback of that method is that the
dnl SISCone version number need to be hardcoded in config.h.siscone.in
dnl   AM_CONFIG_HEADER(config.siscone.in.h:plugins/SISCone/siscone/siscone/config.h.in)
dnl   AC_CONFIG_FILES(plugins/SISCone/siscone/Makefile plugins/SISCone/siscone/siscone/Makefile plugins/SISCone/siscone/siscone/spherical/Makefile plugins/SISCone/siscone/examples/Makefile plugins/SISCone/siscone/examples/events/Makefile)

dnl CDFCones
ACX_CHECK_PLUGIN(CDFCones, cdfcones, CDFCONES, yes)
AC_CONFIG_FILES(plugins/CDFCones/CDFcode/Makefile)

dnl PxCone
ACX_CHECK_PLUGIN(PxCone, pxcone, PXCONE, no)

dnl D0RunIICone
ACX_CHECK_PLUGIN(D0RunIICone, d0runiicone, D0RUNIICONE, no)

dnl NestedDefs
ACX_CHECK_PLUGIN(NestedDefs, nesteddefs, NESTEDDEFS, yes)

dnl TrackJet
ACX_CHECK_PLUGIN(TrackJet, trackjet, TRACKJET, no)

dnl ATLASCone
ACX_CHECK_PLUGIN(ATLASCone, atlascone, ATLASCONE, no)

dnl CMSIterativeCone
ACX_CHECK_PLUGIN(CMSIterativeCone, cmsiterativecone, CMSITERATIVECONE, no)

dnl EECambridge
ACX_CHECK_PLUGIN(EECambridge, eecambridge, EECAMBRIDGE, yes)

dnl Jade
ACX_CHECK_PLUGIN(Jade, jade, JADE, yes)

dnl D0RunICone
ACX_CHECK_PLUGIN(D0RunICone, d0runicone, D0RUNICONE, no)

dnl GridJet
ACX_CHECK_PLUGIN(GridJet, gridjet, GRIDJET, yes)

dnl end of plugin section --- don't modify this line

dnl-------------------------------------------------------------------
dnl enable libtool (has to be done after the previous lib checks!)
dnl-------------------------------------------------------------------
AC_PROG_LIBTOOL

dnl-------------------------------------------------------------------
dnl add Fortran flags if any plugin that needs it is enabled
dnl-------------------------------------------------------------------
if [[ "x$ENABLE_PXCONE" == "xyes" ]] ; then
   dnl set up the fortran compiler and libs
   if test "x$FCFLAGS" == "x"; then
      FCFLAGS="-O2"
   fi
   ## Try to respect users' Fortran compiler variables
   if test "x$FC" == "x"; then
      if test "x$F77" != "x"; then
      	 FC="$F77"
      fi
   fi
   AC_PROG_FC
   if test "x$FC" == "x"; then
      AC_MSG_ERROR([Fortran compiler required to build PxCone but not found or not functional])
   fi
   AC_FC_LIBRARY_LDFLAGS
   F77=$FC
   AM_FFLAGS=$AM_FCFLAGS
   FFLAGS="$FFLAGS $FCFLAGS"
   FLIBS="$FLIBS $FCLIBS"
   AC_SUBST(FCFLAGS)
   AC_SUBST(FFLAGS)
   AC_SUBST(FLIBS)
   AC_SUBST(F77)
   AC_SUBST(AM_FFLAGS)
   AC_SUBST(AM_FCFLAGS)

   CONFIG_LIBS_PLUGINS=${CONFIG_LIBS_PLUGINS}" "${FLIBS}
   CONFIG_LIBS_PLUGINS_STATIC=${CONFIG_LIBS_PLUGINS_STATIC}" "${FLIBS}
fi

dnl-------------------------------------------------------------------
dnl be a bit clearer and use only AM_CXXFLAGS
dnl WARNING: this it totally unnecessary! Except for the D0 plugin:
dnl          since the CXXFLAGS are placed after the AM_CXXFLAGS in
dnl          the command line, a potential -O0 would be spoiled by
dnl          the -O3 in CXXFLAGS. By putting  CXXFLAGS in AM_CXXFLAGS
dnl          we have a better control on the order
dnl------------------------------------------------------------------
if [[ "x$USE_CUSTOM_FLAGS" == "xno" ]]; then
   AM_CXXFLAGS=${CXXFLAGS}" "${AM_CXXFLAGS}
   CXXFLAGS=""
fi


dnl------------------------------------------------------------------
dnl there is a problem with optimisation of the D0 code.
dnl One of the solutions to this issue is to disble inlie functions
dnl We ony do that if the compiler supports the option, otherwise, 
dnl we disable optimisation for that plugin
dnl------------------------------------------------------------------
if [[ "x$ENABLE_D0RUNIICONE" == "xyes" ]] ; then
   # check if the compiler supports -fno-inline
   AC_MSG_CHECKING([if $CXX supports -fno-inline])
   SAVE_CXXFLAGS="$CXXFLAGS"
   CXXFLAGS="-Werror -fno-inline"
   AC_LANG_PUSH(C++)
   AC_LINK_IFELSE([AC_LANG_SOURCE([int main(void){ return 0;}])],
      [ac_support_no_inline=yes],[ac_support_no_inline=no])
   AC_LANG_POP(C++)
   CXXFLAGS="$SAVE_CXXFLAGS"
   AC_MSG_RESULT([$ac_support_no_inline])
   D0_CXXFLAGS=""

   dnl if test x"$ac_enable_new_dtags_ok" = x"yes"; then
   if [[ "x${ac_support_no_inline}" == "xyes" ]]; then
      D0_CXXFLAGS=${AM_CXXFLAGS}" -fno-inline"
   else
      # in case of multiple -O options, the last one prevails
      D0_CXXFLAGS=${AM_CXXFLAGS}" -O0"
   fi
   
   AC_SUBST(D0_CXXFLAGS)
fi

dnl-------------------------------------------------------------------
dnl substitutions useful to know what we have to do in fastjet-config
dnl-------------------------------------------------------------------
AC_SUBST(CONFIG_LIBS_PLUGINS)
AC_SUBST(CONFIG_LIBS_PLUGINS_STATIC)
AC_SUBST(CONFIG_CXXFLAGS)
AC_SUBST(CONFIG_LIBS)
AC_SUBST(CONFIG_RUNPATH_FLAGS)
AC_SUBST(LIST_ALL_PLUGINS)

dnl-------------------------------------------------------------------
dnl tools section
dnl-------------------------------------------------------------------
AC_CONFIG_FILES( tools/Makefile tools/fastjet/Makefile tools/fastjet/tools/Makefile )


dnl-------------------------------------------------------------------
dnl make sure examples dir is there
dnl-------------------------------------------------------------------
AC_CONFIG_FILES( example/Makefile example/data/Makefile example/root/Makefile )

dnl-------------------------------------------------------------------
dnl and the documentation
dnl-------------------------------------------------------------------
AC_CONFIG_FILES( doc/Makefile doc/figs/Makefile )

dnl-------------------------------------------------------------------
dnl configuration script
dnl-------------------------------------------------------------------
AC_CONFIG_FILES([fastjet-config:fastjet-config.in], [chmod u+x fastjet-config])

dnl-------------------------------------------------------------------
dnl sample fortran wrapper
dnl-------------------------------------------------------------------
AC_CONFIG_FILES( fortran_wrapper/Makefile )

dnl-------------------------------------------------------------------
dnl main library outputs
dnl-------------------------------------------------------------------
AC_CONFIG_FILES( Makefile src/Makefile include/Makefile include/fastjet/Makefile include/fastjet/internal/Makefile )

dnl-------------------------------------------------------------------
dnl give a nice summary of the config
dnl-------------------------------------------------------------------
dnl echo ""
dnl echo "Configuration summary:"
dnl echo "----------------------"
dnl echo "  Installation directory     "${prefix}
dnl echo "  Shared libraries           "${enable_shared}
dnl echo "  Static libraries           "${enable_static}
dnl echo "  Debug flag                 "$ENABLE_DEBUG_FLAG
dnl echo "  CGAL support               "${ENABLE_CGAL_FLAG}
dnl AC_PLUGIN_SUMMARY
dnl echo "  Monolithic plugins lib     "$monolithic

CONFIG_SUMMARY=""
CONFIG_SUMMARY+="Configuration summary:\n"
CONFIG_SUMMARY+="----------------------\n"
if [[ "x${prefix}" == "xNONE" ]] ; then
CONFIG_SUMMARY+="  Installation directory     /usr/local\n"
else
CONFIG_SUMMARY+="  Installation directory     "${prefix}"\n"
fi
CONFIG_SUMMARY+="  Shared libraries           "${enable_shared}"\n"
CONFIG_SUMMARY+="  Static libraries           "${enable_static}"\n"
CONFIG_SUMMARY+="  Debug flag                 "$ENABLE_DEBUG_FLAG"\n"
CONFIG_SUMMARY+="  CGAL support               "${ENABLE_CGAL_FLAG}"\n"
AC_PLUGIN_SUMMARY([yes])
CONFIG_SUMMARY+="  Monolithic plugins lib     "$monolithic"\n"
AC_SUBST(CONFIG_SUMMARY)


CONFIGURE_INVOCATION="$0 $ac_configure_args"
AC_SUBST(CONFIGURE_INVOCATION)

dnl-------------------------------------------------------------------
dnl finish with a nice message
dnl-------------------------------------------------------------------
AC_OUTPUT
printf "$CONFIG_SUMMARY"
echo "---------------------------------------------------"
echo "        IMPORTANT NOTICE TO FASTJET USERS          "
echo "                                                   "
echo " When using the FastJet library, you can retrieve  "
echo " compilation and link flags using fastjet-config.  "
echo " Type 'fastjet-config --help' for more information "
echo "---------------------------------------------------"

