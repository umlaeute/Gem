////////////////////////////////////////////////////////
//
// GEM - Graphics Environment for Multimedia
//
// zmoelnig@iem.at
//
// Implementation file
//
//    Copyright (c) 1997-2000 Mark Danks.
//    Copyright (c) 2001-2002 IOhannes m zmoelnig. forum::für::umläute. IEM
//    Copyright (c) 2002 James Tittle & Chris Clepper
//    For information on usage and redistribution, and for a DISCLAIMER OF ALL
//    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.
//
// a wrapper for accessing the RTE's arrays
// currently only numeric arrays
//
/////////////////////////////////////////////////////////

#include "RTE/Symbol.h"
#include "Gem/RTE.h"


class gem::RTE::Symbol::PIMPL {
public:
  const t_symbol*sym;
  
  PIMPL(void) : sym(NULL)
  {
  }
  ~PIMPL(void) {
  }
};

gem::RTE::Symbol :: Symbol(void)
  : m_pimpl(new PIMPL)
{
}

gem::RTE::Symbol :: Symbol(const gem::RTE::Symbol&Sym)
  : m_pimpl(new PIMPL)
{
  m_pimpl->sym=Sym.m_pimpl->sym;
}
gem::RTE::Symbol :: Symbol(const std::string&name) 
  : m_pimpl(new PIMPL)
{
  m_pimpl->sym=gensym(name.c_str());
}
gem::RTE::Symbol :: Symbol(const t_symbol*name) 
  : m_pimpl(new PIMPL)
{
  m_pimpl->sym=name;
}

gem::RTE::Symbol :: ~Symbol(void)
{
  m_pimpl->sym=NULL;
  delete m_pimpl;
}

gem::RTE::Symbol&gem::RTE::Symbol::operator=(const std::string&name) {
  m_pimpl->sym=gensym(name.c_str());
}
gem::RTE::Symbol&gem::RTE::Symbol::operator=(const gem::RTE::Symbol&Sym) {
  m_pimpl->sym=Sym.m_pimpl->sym;
}
gem::RTE::Symbol&gem::RTE::Symbol::operator=(const t_symbol*name) {
  m_pimpl->sym=name;
}
gem::RTE::Symbol&gem::RTE::Symbol::setSymbol(const unsigned int argc, const t_atom*argv) {
  //  m_pimpl->sym=gensym(name.c_str());
}
