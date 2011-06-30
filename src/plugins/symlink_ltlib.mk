# convenience links: symlink the plugin-binary into top_builddir
# include this makefile snippet in your plugin's Makefile.am to automatically
# create symlinks to all pkglib_LTLIBRARIES binaries into your top builddir


.PHONY: convenience-link

convenience-link: $(pkglib_LTLIBRARIES)
	  @for soname in `$(EGREP) "^dlname=" $^ | $(SED) -e "s|^dlname='\(.*\)'|\1|"`; do  \
            echo "$$soname: creating convenience link"; \
	    rm -f $(top_builddir)/$$soname && \
	    test -e $(abs_builddir)/.libs/$$soname && \
	    $(LN_S) $(abs_builddir)/.libs/$$soname $(top_builddir)/$$soname; \
	  done 

all-local:: convenience-link

