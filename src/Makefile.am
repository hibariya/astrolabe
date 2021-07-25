bin_PROGRAMS = astrolabe

astrolabe_SOURCES = main.c

astrolabe_CFLAGS = -I$(top_srcdir)/include
astrolabe_LDADD = $(WORDNET_LIB) -L$(top_srcdir)/lib -lastrolabe

MAINTAINERCLEANFILES = main Makefile.in

maintainer-clean-local:
	-rm -rf .deps
