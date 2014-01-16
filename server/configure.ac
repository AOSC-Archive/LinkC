#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ(2.69)
AC_INIT(Linkc_Server, 1.0, 564691478@qq.com)
AM_INIT_AUTOMAKE(1.0)
#AC_CONFIG_SRCDIR(src/main.c)
#AC_CONFIG_HEADERS(config.h)

# Checks for programs.
AC_PROG_CC

# Checks for libraries.
# FIXME: Replace `main' with a function in `-lpthread':
AC_CHECK_LIB(pthread, main)
# FIXME: Replace `main' with a function in `-lsqlite3':
AC_CHECK_LIB(sqlite3, main)

# Checks for header files.
AC_CHECK_HEADERS(arpa/inet.h malloc.h netinet/in.h stdlib.h string.h sys/socket.h unistd.h)

# Checks for typedefs, structures, and compiler characteristics.
AC_CHECK_HEADER_STDBOOL
AC_TYPE_INT16_T
AC_TYPE_PID_T
AC_TYPE_SIZE_T
AC_TYPE_UINT16_T
AC_TYPE_UINT32_T
AC_TYPE_UINT8_T

AC_PROG_RANLIB

# Checks for library functions.
AC_FUNC_FORK
AC_FUNC_MALLOC
AC_CHECK_FUNCS(inet_ntoa memset socket)

AC_CONFIG_FILES(Makefile
                 src/Makefile
                 src/network/Makefile
                 src/network/p2p/Makefile
                 src/network/network_protocol/Makefile
                 src/utilities/Makefile
                 src/utilities/db_ctl/Makefile
                 src/utilities/list/Makefile
                 src/utilities/user/Makefile)
AC_OUTPUT
