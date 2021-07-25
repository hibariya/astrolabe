lib_LTLIBRARIES = libastrolabe.la

libastrolabe_la_SOURCES = astrolabe.c

libastrolabe_la_CFLAGS = -I$(top_srcdir)/include
libastrolabe_la_LIBADD = $(WORDNET_LIB)

MAINTAINERCLEANFILES = Makefile.in

maintainer-clean-local:
	-rm -rf .deps
