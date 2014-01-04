UTOMAKE_OPTIONS = foreign -Wall -Werror

noinst_LIBRARIES = libuser.a

libuser_a_SOURCES = basic_user_control.c			\
		$(top_srcdir)/include/linkc_db.h	\
		$(top_srcdir)/include/linkc_types.h	\
		$(top_srcdir)/include/def.h

INCLUDES=-I$(top_srcdir)/include/
AM_CXXFLAGS = -D_LINUX -DONLY_EPOLL -D_SERVER
