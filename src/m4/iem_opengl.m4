# SYNOPSIS
#
#   IEM_OPENGL
#
# DESCRIPTION
#
#  some checks for OpenGL-related stuff
#  basically just a wrapper around Braden McDaniel's ax_check_gl* family (as 
#  found in Gnu's autoconf-archive) with additionally setting am-conditionals HAVE_GL* 
#
# LICENSE
#
#   Copyright (c) 2011 IOhannes m zmölnig <zmoelnig@iem.at>
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


AC_DEFUN([IEM_CHECK_GL],
[AC_REQUIRE([AX_CHECK_GL]),
AM_CONDITIONAL(HAVE_GL, [test "x$no_gl" != "xyes"])
])

AC_DEFUN([IEM_CHECK_GLU],
[AC_REQUIRE([AX_CHECK_GLU]),
AM_CONDITIONAL(HAVE_GLU, [test "x$no_glu" != "xyes"])
])


AC_DEFUN([IEM_CHECK_GLUT],
[AC_REQUIRE([AX_CHECK_GLUT]),
AM_CONDITIONAL(HAVE_GLUT, [test "x$no_glut" != "xyes"])
])

