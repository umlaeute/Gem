/* ------------------------------------------------------------------
 * GEM - Graphics Environment for Multimedia
 *
 * SPDX-FileCopyrightText: © 2011, IOhannes m zmölnig and the GEM contributors
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * ------------------------------------------------------------------
 */

/*-----------------------------------------------------------------

  access outlets of the RTE

  -----------------------------------------------------------------*/
#ifndef _INCLUDE__GEM_RTE_OUTLET_H_
#define _INCLUDE__GEM_RTE_OUTLET_H_

#include "Gem/ExportDef.h"
#include <vector>
#include <string>
#include "Utils/any.h"

class CPPExtern;
namespace gem
{
namespace RTE
{
class GEM_EXTERN Outlet
{
private:
  class PIMPL;
  PIMPL*m_pimpl;

public:
  Outlet(CPPExtern*parent);
  Outlet(const Outlet&);

  virtual ~Outlet(void);

  void send(void);   // bang
  void send(double); // float
  void send(std::string, std::vector<gem::any>data);

  Outlet& operator=(const Outlet&);
};
};
};


#endif /* header file */
