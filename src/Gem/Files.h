/* ------------------------------------------------------------------
 * GEM - Graphics Environment for Multimedia
 *
 * SPDX-FileCopyrightText: © 2010, IOhannes m zmölnig and the GEM contributors
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * ------------------------------------------------------------------
 */

/*-----------------------------------------------------------------

  file handling with Gem

-----------------------------------------------------------------*/

#ifndef _INCLUDE__GEM_GEM_FILES_H_
#define _INCLUDE__GEM_GEM_FILES_H_

#include <string>
#include <vector>
#include "Gem/ExportDef.h"

class CPPExtern;
namespace gem
{

namespace files
{

GEM_EXTERN std::vector<std::string>getFilenameListing(
  const std::string&pattern);
GEM_EXTERN std::string expandEnv(const std::string&,
                                 bool bashfilename=false);

GEM_EXTERN std::string getExtension(const std::string&filename,
                                    bool make_lowercase=false);


GEM_EXTERN std::string getFullpath(const std::string&,
                                   const CPPExtern*obj=NULL);

GEM_EXTERN void close(int fd);
};
};

#endif /* _INCLUDE__GEM_GEM_FILES_H_ */
