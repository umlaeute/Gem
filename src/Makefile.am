## for now this is just a dummy Automake-Makefile
AUTOMAKE_OPTIONS = foreign
ACLOCAL_AMFLAGS = -I m4

SUBDIRS=Base RTE

lib_LTLIBRARIES = Gem.la

nodist_EXTRA_Gem_la_SOURCES = dummy.cxx

Gem_la_LDFLAGS = -module -avoid-version -shared @ARCH_FLAGS@ @PD_LDFLAGS@

Gem_la_LIBADD = Base/libBase.la RTE/libRTE.la


libtool: $(LIBTOOL_DEPS)
	$(SHELL) ./config.status --recheck
