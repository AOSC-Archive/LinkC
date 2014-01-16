UTOMAKE_OPTIONS = foreign -Wall -Werror

noinst_LIBRARIES = libList.a

libList_a_SOURCES = list.c		\
		conn_list.c	\
		$(top_srcdir)/include/list.h

AM_CPPFLAGS = -D_LINUX -DONLY_EPOLL -D_SERVER
libList_a_CPPFLAGS = $(AM_CPPFLAGS) -I$(top_srcdir)/include/
