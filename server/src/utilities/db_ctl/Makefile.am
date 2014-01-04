UTOMAKE_OPTIONS = foreign -Wall -Werror

noinst_LIBRARIES = libdb.a

libdb_a_SOURCES = linkc_db.c				\
		$(top_srcdir)/include/linkc_db.h	\
		$(top_srcdir)/include/linkc_types.h	\
		$(top_srcdir)/include/def.h

INCLUDES=-I$(top_srcdir)/include/
AM_CXXFLAGS = -D_LINUX -DONLY_EPOLL -D_SERVER
