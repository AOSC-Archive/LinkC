UTOMAKE_OPTIONS = foreign -Wall -Werror

SUBDIRS = src

bin_PROGRAMS = Server

Server_SOURCES = src/main.c

Server_LDADD = $(top_builddir)/src/network/p2p/libp2p.a				\
		$(top_builddir)/src/network/libconnect.a				\
		$(top_builddir)/src/utilities/db_ctl/libdb.a			\
		$(top_builddir)/src/utilities/list/liblist.a			\
		$(top_builddir)/src/utilities/network_protocol/libnetwork.a	\
		$(top_builddir)/src/utilities/user/libuser.a			\
		-lsqlite3 -lpthread
INCLUDES=-I$(top_srcdir)/include/
