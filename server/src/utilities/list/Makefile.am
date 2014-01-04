UTOMAKE_OPTIONS = foreign -Wall -Werror

noinst_LIBRARIES = liblist.a

liblist_a_SOURCES = list.c		\
		conn_list.c	\
		$(top_srcdir)/include/list.h

INCLUDES=-I$(top_srcdir)/include/
AM_CXXFLAGS = -D_LINUX -DONLY_EPOLL -D_SERVER
