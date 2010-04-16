AC_PREREQ(2.57)
AC_INIT([pix_artoolkit],[0.1],[zmoelnig@iem.at])

AM_INIT_AUTOMAKE([foreign])


AC_LANG(C++)

GEM_CHECK_RTE


AC_SUBST(EXTENSION)
EXTENSION=$GEM_RTE_EXTENSION

#linking on linux
GEM_CHECK_LDFLAGS([-shared -Wl,--export-dynamic])
# linking on os-X
GEM_CHECK_LDFLAGS([-bundle -mmacosx-version-min=10.3 -undefined dynamic_lookup],
  ,
  GEM_CHECK_LDFLAGS([-dynamiclib -mmacosx-version-min=10.3 -undefined dynamic_lookup],
    ,
	  GEM_CHECK_LDFLAGS([-bundle -undefined suppress -flat_namespace])
    )
  )



GEM_CHECK_LIB(artoolkit,AR, arInitCparam,,,,ARToolKit)
AC_CHECK_HEADERS([AR/ar.h])
AC_CHECK_LIB(AR, arInitCparam)



AC_CONFIG_HEADER([config.h])
AC_CONFIG_FILES([Make.config AMakefile])
AC_OUTPUT
