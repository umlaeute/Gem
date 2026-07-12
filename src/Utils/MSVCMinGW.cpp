////////////////////////////////////////////////////////
//
// GEM - Graphics Environment for Multimedia
//
// Implementation file
//
// SPDX-FileCopyrightText: © 2011, IOhannes m zmölnig and the GEM contributors
// SPDX-License-Identifier: GPL-2.0-or-later
//
////////////////////////////////////////////////////////
/* stubs for compiling with MinGW
 *
 * seems to be needed since MinGW provides ISO-C99 compatible printf() implementation
 * which older MSVCrt does not provide yet
 */
#include "Gem/ExportDef.h"

extern "C"  {
  GEM_EXTERN
  int _get_output_format( void )
  {
    return 1;
  }
}
