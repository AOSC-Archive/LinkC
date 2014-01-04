UTOMAKE_OPTIONS = foreign -Wall -Werror

noinst_LIBRARIES = libnetwork.a

libnetwork_a_SOURCES = package_control.c	\
		basic_network_io.c	\
		send_friends_data.c	\
		$(top_srcdir)/include/linkc_network_protocol.h \
		$(top_srcdir)/include/linkc_types.h	\
		$(top_srcdir)/include/def.h		\
		$(top_srcdir)/include/linkc_network.h

INCLUDES=-I$(top_srcdir)/include/
AM_CXXFLAGS = -D_LINUX -DONLY_EPOLL -D_SERVER
