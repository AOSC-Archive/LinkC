UTOMAKE_OPTIONS = foreign -Wall -Werror

noinst_LIBRARIES = libP2P.a

libP2P_a_SOURCES = p2p_helper.c				\
		$(top_srcdir)/include/linkc_db.h	\
		$(top_srcdir)/include/linkc_types.h	\
		$(top_srcdir)/include/linkc_network_protocol.h	\
		$(top_srcdir)/include/linkc_network.h	\
		$(top_srcdir)/include/p2p_helper.h	\
		$(top_srcdir)/include/def.h

AM_CPPFLAGS = -D_LINUX -DONLY_EPOLL -D_SERVER
libP2P_a_CPPFLAGS = $(AM_CPPFLAGS) -I$(top_srcdir)/include/
