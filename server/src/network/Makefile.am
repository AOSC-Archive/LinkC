UTOMAKE_OPTIONS = foreign -Wall -Werror

SUBDIRS = p2p network_protocol

noinst_LIBRARIES = libConnect.a

libConnect_a_SOURCES = init_network.c			\
		keep_connect.c				\
		$(top_srcdir)/include/linkc_db.h	\
		$(top_srcdir)/include/linkc_types.h	\
		$(top_srcdir)/include/linkc_network_protocol.h	\
		$(top_srcdir)/include/linkc_network.h	\
		$(top_srcdir)/include/linkc_user.h	\
		$(top_srcdir)/include/def.h

AM_CPPFLAGS = -D_LINUX -DONLY_EPOLL -D_SERVER
libConnect_a_CPPFLAGS = $(AM_CPPFLAGS) -I$(top_srcdir)/include/
