/*-----------------------------------------------------------------
LOG
    GEM - Graphics Environment for Multimedia

        - bidirectional text support

    Copyright (c) 2010-2011 IOhannes m zmölnig. forum::für::umläute. IEM. zmoelnig@iem.at
    For information on usage and redistribution, and for a DISCLAIMER OF ALL
    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

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
