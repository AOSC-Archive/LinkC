UTOMAKE_OPTIONS = foreign -Wall -Werror

noinst_LIBRARIES = libUser.a

libUser_a_SOURCES = basic_user_control.c		\
		$(top_srcdir)/include/linkc_db.h	\
		$(top_srcdir)/include/linkc_types.h	\
		$(top_srcdir)/include/def.h

AM_CPPFLAGS = -D_LINUX -DONLY_EPOLL -D_SERVER
libUser_a_CPPFLAGS = $(AM_CPPFLAGS) -I$(top_srcdir)/include/
