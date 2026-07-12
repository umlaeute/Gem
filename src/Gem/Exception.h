/* ------------------------------------------------------------------
 * GEM - Graphics Environment for Multimedia
 *
 * SPDX-FileCopyrightText: © 2009, IOhannes m zmölnig and the GEM contributors
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * ------------------------------------------------------------------
 */

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
    GemException

    an exception class...

DESCRIPTION

    this is a class, we can throw on creation,
    to make sure that the pd-object can not be created

-----------------------------------------------------------------*/

#ifndef _INCLUDE__GEM_GEM_EXCEPTION_H_
#define _INCLUDE__GEM_GEM_EXCEPTION_H_

#include "Gem/ExportDef.h"
#include <stdexcept>


typedef struct _text t_object;

class GEM_EXTERN GemException : public std::runtime_error
{
public:
  GemException(void);
  explicit GemException(const char*error);
  explicit GemException(const std::string&error);
  virtual void report(const char*origin=0) const;
};

namespace gem
{
GEM_EXTERN void catchGemException(const char*name=NULL,
                                  const t_object*obj=NULL);
};


#endif /* _INCLUDE__GEM_GEM_EXCEPTION_H_ */
