#
# SYNOPSIS
#
#   generic M4 macros
#
# LICENSE
#
#   Copyright (c) 2014 IOhannes m zmölnig, iem
#
#   This program is free software; you can redistribute it and/or modify it
#   under the terms of the GNU General Public License as published by the
#   Free Software Foundation; either version 2 of the License, or (at your
#   option) any later version.
#
#   This program is distributed in the hope that it will be useful, but
#   WITHOUT ANY WARRANTY; without even the implied warranty of
#   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General
#   Public License for more details.
#
#   You should have received a copy of the GNU General Public License along
#   with this program. If not, see <http://www.gnu.org/licenses/>.
#
#   As a special exception, the respective Autoconf Macro's copyright owner
#   gives unlimited permission to copy, distribute and modify the configure
#   scripts that are the output of Autoconf when processing the Macro. You
#   need not follow the terms of the GNU General Public License when using
#   or distributing such scripts, even though portions of the text of the
#   Macro appear in them. The GNU General Public License (GPL) does govern
#   all other use of the material that constitutes the Autoconf Macro.
#
#   This special exception to the GPL applies to versions of the Autoconf
#   Macro released by the Autoconf Archive. When you make and distribute a
#   modified version of the Autoconf Macro, you may extend this special
#   exception to the GPL to apply to your modified version as well.

#serial 16



#
# SYNOPSIS
#
#   IEM_CHECK_INCLUDES_DEFAULT
#
# DESCRIPTION
#
#   Extend the default-includes (AC_INCLUDES_DEFAULT)
#
#   Given a list of headers to check, this macro adds each each header to the
#   AC_INCLUDES_DEFAULT (surrounded by a protecting #ifdef).
#   If the header is actually present on the system, a corresponding HAVE_...
#   is defined.
#
#   Example:
#
#   IEM_CHECK_INCLUDES_DEFAULT([windows.h])
#
# will define HAVE_WINDOWS_H (if the header is found, e.g. on w32 systems) and
# add the following to your headers:
#     #if defined(HAVE_WINDOWS_H)
#     # include <windows.h>
#     #endif
AC_DEFUN([IEM_CHECK_INCLUDES_DEFAULT], [
   for __iem_check_includes_default_hdr in $1; do
       AC_CHECK_HEADERS([${__iem_check_includes_default_hdr}])
       ac_includes_default="${ac_includes_default}
[#]ifdef AS_TR_CPP([HAVE_${__iem_check_includes_default_hdr}])
[#] include <${__iem_check_includes_default_hdr}>
[#]endif"
   done
])

#
# SYNOPSIS
#
#   IEM_CHECK_FUNCS(function..., [action-if-found], [action-if-not-found], [additional-cflags], [additional-libs])
#
# DESCRIPTION
#
#    check for functions
#
#    For each function enumerated in the blank-or-newline-separated argument list,
#    define HAVE_function (in all capitals) if it is available.
#    if additional-cflags is given, it is temporarily appended to the CFLAGS.
#    if additional-libs is given, it is temporarily appended to the LIBS
#
#   Example:
#
#   IEM_CHECK_FUNCS([foo bar], [], [], [-DHAVE_FOO=1], [-lfoo])
#
AC_DEFUN([IEM_CHECK_FUNCS], [
 tmp_iemcheckfuncs_cflags="$CFLAGS"
 tmp_iemcheckfuncs_libs="$LIBS"
 CFLAGS="$CFLAGS $4"
 LIBS="$LIBS $5"
 AC_CHECK_FUNCS([$1], [$2], [$3])

 CFLAGS="${tmp_iemcheckfuncs_cflags}"
 LIBS="${tmp_iemcheckfuncs_libs}"
])
