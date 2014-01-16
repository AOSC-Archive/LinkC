UTOMAKE_OPTIONS = foreign -Wall -Werror

noinst_LIBRARIES = libLinkCProtocol.a

libLinkCProtocol_a_SOURCES = package_control.c	\
		send_friends_data.c	\
		basic_network_io.c	\
		$(top_srcdir)/include/linkc_network_protocol.h \
		$(top_srcdir)/include/linkc_types.h	\
		$(top_srcdir)/include/def.h		\
		$(top_srcdir)/include/linkc_network.h

AM_CPPFLAGS = -D_LINUX -DONLY_EPOLL -D_SERVER
libLinkCProtocol_a_CPPFLAGS = $(AM_CPPFLAGS) -I$(top_srcdir)/include/
