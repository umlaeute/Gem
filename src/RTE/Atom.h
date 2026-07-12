/* ------------------------------------------------------------------
 * GEM - Graphics Environment for Multimedia
 *
 * SPDX-FileCopyrightText: © 2023, IOhannes m zmölnig and the GEM contributors
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * ------------------------------------------------------------------
 */

/*-----------------------------------------------------------------

  access atoms of the RTE

  -----------------------------------------------------------------*/
#ifndef _INCLUDE__GEM_RTE_ATOM_H_
#define _INCLUDE__GEM_RTE_ATOM_H_


#include "Gem/ExportDef.h"
#include <string>

struct _atom;
struct _symbol;
namespace gem
{
namespace RTE
{
class GEM_EXTERN Atom
{
private:
  class PIMPL;
  PIMPL*m_pimpl;

public:
  Atom(void);
  Atom(const gem::RTE::Atom&a);
  Atom(const std::string&name);
  Atom(double);
  Atom(const struct _atom*a);
  Atom(const struct _symbol*name);

  virtual ~Atom(void);

  virtual Atom&operator=(const Atom&);
  virtual Atom&operator=(const struct _atom*);
  virtual Atom&operator=(const std::string&);
  virtual Atom&operator=(const struct _symbol*);
  virtual Atom&operator=(double);

  virtual std::string getString(void) const;
  virtual struct _symbol*getSymbol(void) const;
  virtual double getFloat(void) const;
  virtual const struct _atom*getRTEAtom(void) const;

  virtual operator std::string(void)
  {
    return getString();
  }

  virtual operator struct _symbol*(void)
  {
    return getSymbol();
  }

  virtual operator const struct _atom*(void)
  {
    return getRTEAtom();
  }

  virtual operator double(void)
  {
    return getFloat();
  }

  static bool checkSignature(int argc, const _atom*argv, int numtypes, const int*types, const char*name);

};
};
};
#endif /* _INCLUDE__GEM_RTE_ATOM_H_ */
