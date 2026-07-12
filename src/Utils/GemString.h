/* ------------------------------------------------------------------
 * GEM - Graphics Environment for Multimedia
 *
 * SPDX-FileCopyrightText: © 2013, IOhannes m zmölnig and the GEM contributors
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * ------------------------------------------------------------------
 */

/*-----------------------------------------------------------------

  bidirectional text support

-----------------------------------------------------------------*/

#ifndef _INCLUDE__GEM_GEM_STRING_H_
#define _INCLUDE__GEM_GEM_STRING_H_

#include <string>
#include "Gem/ExportDef.h"

class CPPExtern;
namespace gem
{

namespace string
{

/* convert a string to it's visual representation (UTF-8) */
GEM_EXTERN std::wstring getVisualLine(const std::string&);
GEM_EXTERN std::wstring getVisualLine(const std::wstring&);

/* convert a UTF-8 string to wchar */
// throws 'int', holding the position of the char that couldn't be converted
GEM_EXTERN std::wstring toWstring(const char*str);
GEM_EXTERN std::wstring toWstring(const std::string&str);
};
};

#endif /* _INCLUDE__GEM_GEM_BIDI_H_ */
