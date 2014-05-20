dnl CHECK CGAL BEGIN
dnl This script takes too arguments, the action on success and the action on failure.
dnl It first checks if a --with-cgalmakefile) value is passed.
dnl If that fails, it checks if the CGAL_MAKEFILE environment variable
dnl is defined. If that fails too, it seaches for CGAL in the standard places.
dnl
dnl If a Makefile is specified and not found, an error is returned
dnl If the standard Makefile is not found, then one check for the 
dnl   --with-cgaldir presence and check if CGAL can be found in standard places
dnl 
dnl Note that in the last case, we use a predefined set of compiler and linker
dnl arguments, a method that may fail.
dnl
dnl CGAL_CXXFLAGS, CGAL_CPPFLAGS, CGAL_LDFLAGS and CGAL_LIBS are all defined.
AC_DEFUN([ACX_CHECK_CGAL],
[
dnl Start by looking if a Makefile is given through --with-cgalmakefile
acx_cgal_found=no
acx_cgal_default_makefile=no
AC_ARG_WITH(cgalmakefile,
            [AC_HELP_STRING([--with-cgalmakefile=makefile], [Use the following CGAL makefile (CGAL <= 3.3.x)])])

dnl ckeck if a directory is specified for CGAL
dnl Note: this is of no effect if a Makefile is used
AC_ARG_WITH(cgaldir,
            [AC_HELP_STRING([--with-cgaldir=dir], [Assume the given directory for CGAL (CGAL >= 3.4)])])

dnl define CGAL_MAKEFILE to be 
dnl  1. the value given to --with-cgalmakefile
dnl  2. the environment var
dnl  3. the default location: /usr/share/cgal/cgal.mk
dnl in that order 
dnl Note: using --with-cgalmakefile=no disables CGAL. In our case this is redundant as 
dnl       we already have a --enable-cgal option, but we keep it as it was in the
dnl       original code
case $with_cgalmakefile in
    yes | "")
        dnl check if a env. var is defined
	if test x${CGAL_MAKEFILE} = x ; then
            # if no installation dir is explicitly specified, try a default makefile
	    if test \! -z "$with_cgaldir"; then
	        CGAL_MAKEFILE=""
	    else 
	        CGAL_MAKEFILE="/usr/share/cgal/cgal.mk" 
	    fi
	fi
	acx_cgal_default_makefile=yes;
	;;
    no) acx_cgal_found=disable ;;
    -* | */* | *.a | *.so | *.so.* | *.o) CGAL_MAKEFILE="$with_cgalmakefile" ;;
    *) CGAL_MAKEFILE="$with_cgalmakefile" ;;
esac

dnl check for the presence of the CGAL Makefile
dnl if present, get the compilation and linker flags there
if test "$acx_cgal_found" == no; then
    AC_MSG_CHECKING(CGAL_MAKEFILE)

    dnl check that a Makefile has been specified
    if test \! -z "$CGAL_MAKEFILE"; then

        dnl make sure the file exists
	if test -e "$CGAL_MAKEFILE"; then
 	    dnl to get the arguments, we'll just write a temporary Makefile
	    dnl that "echo"s the CGAL_CXXFLAGS and CGAL_LDFLAGS variables upon request
	    tname=`mktemp /tmp/cgal_makefile_dsrXXXXXX`
    
cat > $tname << _ACEOF
include $CGAL_MAKEFILE

cppflags:
	@echo \$(CGAL_CXXFLAGS)

cxxflags:
	@echo
ldflags:
	@echo \$(CGAL_LDFLAGS)
_ACEOF

	    dnl get the compiler and preprocessor info
	    CGAL_CPPFLAGS="`make -s -f $tname cppflags`"
	    CGAL_CXXFLAGS="`make -s -f $tname cxxflags`"
	    
	    dnl for the libs, put the -l directives in CGAL_LIBS and the others in 
	    dnl CGAL_LDFLAGS
	    CGAL_LDFLAGST="`make -s -f $tname ldflags`"
	    for i in $CGAL_LDFLAGST; do
		if test `echo $i| grep -c ^-l`; then 
		    CGAL_LIBS="$CGAL_LIBS $i"
		else
		    CGAL_LDFLAGS="$CGAL_LDFLAGS $i"
		fi
	    done

	    dnl remove the temporary Makefile and tell we succeeded
	    rm -f $tname
	    AC_MSG_RESULT(yes)
	    acx_cgal_found=yes
        else
	    dnl the Makefile does not exists
            dnl We only need to return an error if the Makefile was not the default one
	    AC_MSG_RESULT(inexistent)
	    if test "$acx_cgal_default_makefile" == no; then
 	    	AC_ERROR([CGAL_MAKEFILE defined as $CGAL_MAKEFILE, but the makefile does not exist.])
	    fi
	fi
    else 
	dnl the Makefile is invalid
	AC_MSG_RESULT(not specified)
        dnl dnl We only need to return an error if the Makefile was not the default one
	dnl if test "$acx_cgal_default_makefile" == no; then
 	dnl     AC_ERROR([CGAL_MAKEFILE defined as $CGAL_MAKEFILE, but the makefile does not exist.])
	dnl fi
    fi
fi


dnl if CGAL hasn't been found yet and the Makefile search didn't return an error message
dnl we check if we can get the arguments ourselves. This method leave the option to specify
dnl a CGAL directory using the --with-cgaldir directive
dnl If no dir are specified, CGLA will be searched in standard places
if test "$acx_cgal_found" == no; then
   dnl check support for -frounding-math for the C++ compiler
   ADDITIONAL_CGAL_FLAGS=""
   AC_LANG_PUSH(C++)
   AX_CHECK_COMPILER_FLAGS([-frounding-math],[ADDITIONAL_CGAL_FLAGS=${ADDITIONAL_CGAL_FLAGS}" -frounding-math"])
   AC_LANG_POP(C++)


    dnl First check if an installation dir has been supplied.
    dnl The current flags have to be saved temporarily
    save_LIBS="$LIBS"
    save_LDFLAGS="$LDFLAGS"
    save_CXXFLAGS="$CXXFLAGS"
    save_CPPFLAGS="$CPPFLAGS"

    CGAL_CXXFLAGS=""
    CGAL_CPPFLAGS=""
    CGAL_LDFLAGS=""
    CGAL_LIBS=""

    dnl check if a directory has bene specified
    dnl in that case, we need to add a -I and -L arg to CXXFLAGS and LDFLAGS
    dnl Note that the headers will be searched fo in ${cgaldir}/include
    dnl and the libs in ${cgaldir}/libs
    if test \! -z "$with_cgaldir"; then
        AC_MSG_CHECKING(CGAL in ${with_cgaldir})
	LDFLAGS="${LDFLAGS} -L${with_cgaldir}/lib -Wl,-rpath,${with_cgaldir}/lib"
	CXXFLAGS="${CXXFLAGS} -I${with_cgaldir}/include $ADDITIONAL_CGAL_FLAGS"
	CPPFLAGS="${CPPFLAGS} -I${with_cgaldir}/include $ADDITIONAL_CGAL_FLAGS"

	CGAL_CPPFLAGS="-I${with_cgaldir}/include"
	CGAL_CXXFLAGS="-I${with_cgaldir}/include"
	CGAL_LIBS="-L${with_cgaldir}/lib -Wl,-rpath,${with_cgaldir}/lib"
    fi

    CXXFLAGS=${CXXFLAGS}" $ADDITIONAL_CGAL_FLAGS"
    CPPFLAGS=${CPPFLAGS}" $ADDITIONAL_CGAL_FLAGS"

    dnl search the CGAL headers
    dnl
    dnl set temporarily the check language to C++
    dnl The default choice otherwise in C whih is usually fine apart from 
    dnl the tests related to CGAL which have c-like header names and fail
    dnl with gcc. The switch to g++ allows CGAL to be detected if it is
    dnl not detected through a Makefile.
    dnl
    dnl Note that one can temporarily change lang. By using
    dnl   AC_LANG_PUSH(language)
    dnl followed by
    dnl   AC_LANG_POP(language)
    AC_LANG_PUSH(C++)
    AC_CHECK_HEADER([CGAL/Exact_predicates_inexact_constructions_kernel.h], [cgal_have_header=yes], [cgal_have_header=no])
    AC_LANG_POP(C++)

    dnl if the headers have been found, check for the libs
    if test "$cgal_have_header" == yes; then
	CGAL_CPPFLAGS="${CGAL_CPPFLAGS} $ADDITIONAL_CGAL_FLAGS"
	CGAL_CXXFLAGS="${CGAL_CXXFLAGS} $ADDITIONAL_CGAL_FLAGS"
	AC_LANG_PUSH(C++)
	AC_CHECK_LIB(CGAL, main, cgal_have_lib=yes, cgal_have_lib=no)
	AC_LANG_POP(C++)
	if test "$cgal_have_lib" == yes; then
	    CGAL_LIBS=${CGAL_LIBS}" -lCGAL"
	    dnl AC_CHECK_LIB(mpfr, main, [CGAL_LIBS="$CGAL_LIBS -lmpfr"])
	    dnl AC_CHECK_LIB(gmp, main, [CGAL_LIBS="$CGAL_LIBS -lgmp"])
	    dnl AC_CHECK_LIB(gmpxx, main, [CGAL_LIBS="$CGAL_LIBS -lgmpxx"])
	    dnl AC_CHECK_LIB(CGALcore++, main, [CGAL_LIBS="$CGAL_LIBS -lCGALcore++"])

	    dnl we can finally claim we've found CGAL!
	    acx_cgal_found=yes
	fi
    fi 

    dnl if the lib has not been found, reset the saved vars to their original values
    LIBS="$save_LIBS"
    LDFLAGS="$save_LDFLAGS"
    CXXFLAGS="$save_CXXFLAGS"
    CPPFLAGS="$save_CPPFLAGS"

    if test "$acx_cgal_found" == no; then 
       CGAL_CPPFLAGS=""
       CGAL_CXXFLAGS=""
       CGAL_LDFLAGS=""
       CGAL_LIBS=""
    fi
fi

AC_MSG_CHECKING(CGAL)
if test "$acx_cgal_found" == yes; then 
	AC_MSG_RESULT(yes);
	$1
else
	AC_MSG_RESULT(no);
	$2
fi])


dnl CHECK CGAL END
