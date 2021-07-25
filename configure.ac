AC_INIT([Astrolabe], [0.0])
AM_INIT_AUTOMAKE([-Wall -Werror foreign])

AC_PROG_CC

AC_CHECK_LIB([wordnet], [wninit], [WORDNET_LIB=-lwordnet], [
  AC_CHECK_LIB([WN], [wninit], [WORDNET_LIB=-lWN], [AC_MSG_ERROR([Missing library: wordnet])])
])
AC_SUBST(WORDNET_LIB)

AM_PROG_AR

AC_CHECK_HEADERS([stdlib.h])
AC_CHECK_HEADERS([string.h])
AC_CHECK_HEADERS([wn.h])

LT_INIT([shared disable-static])

AC_CONFIG_FILES([Makefile include/Makefile lib/Makefile src/Makefile])
AC_OUTPUT
