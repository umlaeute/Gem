/* ------------------------------------------------------------------
 * GEM - Graphics Environment for Multimedia
 *
 * SPDX-FileCopyrightText: © 2010, IOhannes m zmölnig and the GEM contributors
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * ------------------------------------------------------------------
 */

/*-----------------------------------------------------------------

  access arrays of the RTE

  -----------------------------------------------------------------*/
#ifndef _INCLUDE__GEM_RTE_ARRAY_H_
#define _INCLUDE__GEM_RTE_ARRAY_H_


#include "Gem/ExportDef.h"
#include <string>
#include "Gem/RTE.h"

namespace gem
{
namespace RTE
{
class GEM_EXTERN Array
{
private:
  class PIMPL;
  PIMPL*m_pimpl;

public:
  Array(void);
  Array(const gem::RTE::Array&a);
  Array(const std::string&name);

  virtual ~Array(void);

  /* check whether we hold a valid reference to an array */
  virtual bool isValid();

  /* reference another array */
  virtual bool name(const std::string&s);
  virtual const std::string&name(void) const;

  virtual bool resize(const size_t newsize);
  virtual size_t size(void) const;

  virtual t_float&operator[](const unsigned int&index);
  virtual const t_float&operator[](const unsigned int&index) const;

  virtual void set(const t_float f);

  virtual Array&operator=(const Array&);
};
};
};
#endif /* _INCLUDE__GEM_RTE_ARRAY_H_ */
