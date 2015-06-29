AC_PREREQ(2.60)
AC_INIT([videoDECKLINK],[0.1],[zmoelnig@iem.at], [], [http://gem.iem.at])

AC_CONFIG_AUX_DIR([.])
AC_CONFIG_MACRO_DIR([../../m4])

AM_INIT_AUTOMAKE([foreign])

AC_CONFIG_SRCDIR([videoDECKLINK.cpp])
AC_CONFIG_HEADER([config.h])
AC_CONFIG_FILES([Makefile])

AC_PROG_CXX
AC_PROG_LIBTOOL
#AM_SANITY_CHECK

GEM_CHECK_RTE

AC_LANG(C++)


dnl GEM_CHECK_DECKLINK
GEM_CHECK_LIB([DeckLink],[], [DeckLinkAPI.h],[CreateDeckLinkIteratorInstance], ,,,[video input])
AS_IF([test "x${PKG_DECKLINK_LIBS}" != "x" ],[],[
 AC_DEFINE_UNQUOTED(AS_TR_CPP(MUST_DLOPEN_DECKLINKAPI),[1], [Define to 1 to open DeckLinkAPI via dlopen()])
 ])

AC_OUTPUT
