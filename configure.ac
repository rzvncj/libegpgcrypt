
AC_INIT([libegpgcrypt],[0.1])
AC_CONFIG_SRCDIR(lib/init.cpp)
AC_CONFIG_HEADERS(include/config.h)
AC_CONFIG_MACRO_DIR([m4])
LT_INIT

AC_PROG_CXX
AX_CXX_COMPILE_STDCXX_17([noext], [mandatory])

AM_PATH_GPG_ERROR(1.35, [], AC_MSG_ERROR([Could not find libgpg-error]))
AM_PATH_GPGME(1.3.0, [], AC_MSG_ERROR([Could not find GPGME]))

CXXFLAGS="$CXXFLAGS -W -Wall -pthread $GPGME_CFLAGS $GPG_ERROR_CFLAGS"
CFLAGS="$CFLAGS -W -Wall -pthread $GPGME_CFLAGS $GPG_ERROR_CFLAGS"
LIBS="$LIBS $GPGME_LIBS"

AC_ARG_ENABLE(debug,
	      [  --enable-debug          compile with gdb debug information.],
	      CXXFLAGS="$CXXFLAGS -g")

AC_ARG_ENABLE(optimize,
	      [  --enable-optimize       optimize compiled code (-O2).],
	      CXXFLAGS="$CXXFLAGS -O2")

AM_INIT_AUTOMAKE
AC_PROG_INSTALL

AC_CONFIG_FILES([Makefile lib/Makefile tests/Makefile include/Makefile])
AC_OUTPUT

