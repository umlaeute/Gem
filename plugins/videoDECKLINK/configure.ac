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

AC_LANG(C++)

GEM_CHECK_FAT(GEM_ARCH)
GEM_CHECK_EXTERNAL
GEM_CHECK_THREADS

system=""
AS_CASE([${host}]
        , [*-darwin*], [system=Darwin]
        , [*-linux*], [system=Linux]
        , [*mingw*], [system=Windows]
        )

AS_IF([test "x${with_DeckLink}" = "xlocal"],[
 sdkdir=${srcdir}/SDK/${system}/
 AS_IF([test -e ${sdkdir}/DeckLinkAPI.h], [
 with_DeckLink=yes
 with_DeckLink_CFLAGS="-I${sdkdir}"
 AC_MSG_NOTICE([using embedded copy of DeckLink-SDK in ${sdkdir}])
],[
 AC_MSG_WARN([requested to use embedded copy of DeckLink-SDK, but none found])
 with_DeckLink=no
]
)])

dnl GEM_CHECK_DECKLINK
GEM_CHECK_LIB([DeckLink],[], [DeckLinkAPI.h],[CreateDeckLinkIteratorInstance], ,,,[video input; use 'local' to use an embedded copy pf the SDK])
AS_IF([test "x${PKG_DECKLINK_LIBS}" != "x" ],[],[
 AC_DEFINE_UNQUOTED(AS_TR_CPP(MUST_DLOPEN_DECKLINKAPI),[1], [Define to 1 to open DeckLinkAPI via dlopen()])
 ])

AC_OUTPUT
