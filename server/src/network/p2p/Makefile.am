UTOMAKE_OPTIONS = foreign -Wall -Werror

noinst_LIBRARIES = libp2p.a

libp2p_a_SOURCES = p2p_helper.c				\
		$(top_srcdir)/include/linkc_db.h	\
		$(top_srcdir)/include/linkc_types.h	\
		$(top_srcdir)/include/linkc_network_protocol.h	\
		$(top_srcdir)/include/linkc_network.h	\
		$(top_srcdir)/include/p2p_helper.h	\
		$(top_srcdir)/include/def.h

INCLUDES=-I$(top_srcdir)/include/
AM_CXXFLAGS = -D_LINUX -DONLY_EPOLL -D_SERVER
