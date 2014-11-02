#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([csl], [0.9])
AC_CONFIG_SRCDIR([src/CslApp.cpp])
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_MACRO_DIR([m4])

AM_INIT_AUTOMAKE([-Wno-portability])

# i18n
IT_PROG_INTLTOOL([0.50.0])

GETTEXT_PACKAGE=AC_PACKAGE_NAME
AC_DEFINE_UNQUOTED(GETTEXT_PACKAGE, ["$GETTEXT_PACKAGE"], [Define to the Gettext package name])
AC_SUBST([GETTEXT_PACKAGE])


# Add some compiler flags
DEFAULT_FLAGS="-fsigned-char -Wall -fno-strict-aliasing"
CFLAGS="$CFLAGS $DEFAULT_FLAGS"
CXXFLAGS="$CXXFLAGS $DEFAULT_FLAGS"


# Checks for programs.
AC_LANG_CPLUSPLUS
AC_PROG_CXX
AC_PROG_CC
AC_PROG_INSTALL
AC_PROG_MAKE_SET
AC_PROG_LIBTOOL

# Checks for header files.
AC_CHECK_HEADERS([arpa/inet.h fcntl.h netdb.h netinet/in.h stdint.h stdlib.h string.h sys/socket.h sys/time.h unistd.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_CHECK_HEADER_STDBOOL
AC_C_INLINE
AC_TYPE_OFF_T
AC_TYPE_SIZE_T

# Checks for library functions.
AC_FUNC_ERROR_AT_LINE
AC_FUNC_MALLOC
AC_FUNC_MMAP
AC_FUNC_REALLOC
AC_CHECK_FUNCS([gethostbyname gethostname gettimeofday inet_ntoa localtime_r memmove memset munmap select socket strcasecmp strchr strdup strerror strpbrk strrchr strspn strstr])


# Checks for libraries.

# wxWidgets
AM_OPTIONS_WXCONFIG
reqwx=2.9.5
AM_PATH_WXCONFIG($reqwx, wxWin=1,,[std,aui])
if test "$wxWin" != 1; then
	AC_MSG_ERROR([
		wxWidgets must be installed on your system.
 
		Please check that wx-config is in path, the directory
		where wxWidgets libraries are installed (returned by
		'wx-config --libs' or 'wx-config --static --libs' command)
		is in LD_LIBRARY_PATH or equivalent variable and
		wxWidgets version is $reqwx or above.
		])
fi
 
CPPFLAGS="$CPPFLAGS $WX_CPPFLAGS"
CXXFLAGS="$CXXFLAGS $WX_CXXFLAGS_ONLY"
wxtype=`echo $WX_CXXFLAGS | sed 's/\(.*\)\(ansi\|unicode\)\(.*\)/\2/'`
wxversion="$WX_VERSION"
AC_SUBST(WX_LIBS)


# Optional speech-dispatcher
use_speechd="yes"
AC_ARG_ENABLE(speechd,AC_HELP_STRING([--disable-speechd],
                                     [disable speech-dispatcher support]),
              use_speechd=$enableval)
if test "x$use_speechd" = "xyes"; then
  AC_CHECK_HEADERS(libspeechd.h,SPEECHD_LIBS="-lspeechd",use_speechd="no")
fi
AC_SUBST(SPEECHD_LIBS)


# Optional libircclient
use_internal_libircclient="yes"
AC_CHECK_HEADERS(libircclient.h,use_internal_libircclient="no")
if test "x$use_internal_libircclient" = "xyes"; then
  LIBIRCCLIENT_LIBS="irc/libircclient/src/libircclient.a"
else
  LIBIRCCLIENT_LIBS="-llibircclient"
fi
AM_CONDITIONAL(with_internal_libircclient,test "x$use_internal_libircclient" = "xyes")
AC_SUBST(LIBIRCCLIENT_LIBS)


# Optional libGeoIP
use_internal_geoip="yes"
AC_CHECK_HEADERS(GeoIP.h,AC_CHECK_LIB(GeoIP,GeoIP_new,[use_internal_geoip="no"]))
if test "x$use_internal_geoip" = "xyes"; then
  GEOIP_LIBS="libGeoIP/libGeoIP.a"
else
  GEOIP_LIBS="-lGeoIP"
fi
AM_CONDITIONAL(with_internal_geoip,test "x$use_internal_geoip" = "xyes")
AC_SUBST(GEOIP_LIBS)


# Optional GeoIP Database
GEOIPDATADIR=""
use_internal_geoip_db="yes"
AC_ARG_WITH(geoip-datadir,AC_HELP_STRING([--with-geoip-datadir=DIR],
                                         [Use the given path for GeoIP database lookup]),
[
  if test "$withval" != "yes" -a "$withval" != ""; then
    use_internal_geoip_db="no"
    GEOIPDATADIR=$withval
    AC_DEFINE([CSL_EXTERNAL_GEOIP_DATABASE],[1],[use external GeoIP database])
  fi
])
AM_CONDITIONAL(with_internal_geoipdatabase,test "x$use_internal_geoip_db" = "xyes")
AC_DEFINE_UNQUOTED([GEOIPDATADIR],["$GEOIPDATADIR"],[GeoIP database path])


# Build the map config tool
cslmapcfgtool=yes
AC_ARG_WITH(cslmapcfgtool,AC_HELP_STRING([--with-cslmapcfgtool],
                                         [build the map config tool]),
            cslmapcfgtool=$withval
)
AM_CONDITIONAL(build_cslmapcfgtool,test "x$cslmapcfgtool" = "xyes")


# Enable precompiled headers
AC_MSG_CHECKING([whether to use precompiled headers])
use_pch=""
AC_ARG_ENABLE(pch,AC_HELP_STRING([--enable-pch],[build using precompiled headers]),
              use_pch="$enableval")
if test "x$enable_dependency_tracking" = "xno"; then
  use_pch="no"
fi
if test -z "$use_pch"; then
  if test "X$GCC" = Xyes; then
    if gcc_version=`$CC -dumpversion` > /dev/null 2>&1; then
      major=`echo $gcc_version | cut -d. -f1`
      minor=`echo $gcc_version | sed "s/@<:@-,a-z,A-Z@:>@.*//" | cut -d. -f2`
      if test -z "$major" || test -z "$minor"; then
        use_pch=no
      elif test "$major" -ge 4; then
        use_pch=yes
      else
        use_pch=no
      fi
    else
      use_pch=no
    fi
  else
    use_pch=no
  fi
fi
if test "x$use_pch" = "xyes"; then
  AC_MSG_RESULT([yes])
else
  AC_MSG_RESULT([no])
fi
AM_CONDITIONAL(ENABLE_PCH,test "x$use_pch" = "xyes")


AC_CONFIG_FILES([Makefile
                 data/Makefile
                 data/maps/Makefile
                 data/maps/RedEclipse/256/Makefile
                 data/maps/RedEclipse/Makefile
                 data/maps/Sauerbraten/254/Makefile
                 data/maps/Sauerbraten/256/Makefile
                 data/maps/Sauerbraten/Makefile
                 src/Makefile
                 src/engine/Makefile
                 src/irc/Makefile
                 src/irc/libircclient/Makefile
                 src/irc/libircclient/include/Makefile
                 src/irc/libircclient/src/Makefile
                 src/libGeoIP/Makefile
                 src/pch/Makefile
                 po/Makefile.in])
AC_OUTPUT

echo
echo "--------------------------------------------------"
echo "$PACKAGE $VERSION"

echo
echo "wxWidgets version.............. : $wxversion ($wxtype)"
echo "Build using precompiled headers : $use_pch"
echo
echo "Speech-dispatcher support...... : $use_speechd"
echo
echo "Need internal libGeoIP......... : $use_internal_geoip"
echo "Need internal GeoIP database... : $use_internal_geoip_db"
echo
echo "Need internal libircclient..... : $use_internal_libircclient"
echo
echo "Build map config tool.......... : $cslmapcfgtool"
echo "--------------------------------------------------"
echo

