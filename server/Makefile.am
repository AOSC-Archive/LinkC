UTOMAKE_OPTIONS = foreign -Wall -Werror

SUBDIRS = src

bin_PROGRAMS = Server

Server_SOURCES = src/main.c

Server_LDADD = $(top_builddir)/src/network/p2p/libP2P.a				\
		$(top_builddir)/src/network/libConnect.a			\
		$(top_builddir)/src/utilities/db_ctl/libDb.a			\
		$(top_builddir)/src/utilities/list/libList.a			\
		$(top_builddir)/src/network/network_protocol/libLinkCProtocol.a	\
		$(top_builddir)/src/utilities/user/libUser.a			\
		-lsqlite3 -lpthread
AM_CPPFLAGS = -D_LINUX -DONLY_EPOLL -D_SERVER
Server_CPPFLAGS = $(AM_CPPFLAGS) -I$(top_srcdir)/include/
