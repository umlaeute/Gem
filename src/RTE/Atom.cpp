////////////////////////////////////////////////////////
//
// GEM - Graphics Environment for Multimedia
//
// zmoelnig@iem.at
//
// Implementation file
//
//    Copyright (c) 1997-2000 Mark Danks.
//    Copyright (c) 2001-2011 IOhannes m zmölnig. forum::für::umläute. IEM. zmoelnig@iem.at
//    Copyright (c) 2002 James Tittle & Chris Clepper
//    For information on usage and redistribution, and for a DISCLAIMER OF ALL
//    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.
//
// a wrapper for accessing the RTE's arrays
// currently only numeric arrays
//
/////////////////////////////////////////////////////////

#include "RTE/Atom.h"
#include "Gem/RTE.h"

class gem::RTE::Atom::PIMPL
{
public:
  t_atom atom;

  PIMPL(void)
  {
    atom.a_type = A_NULL;
  }
  ~PIMPL(void)
  {
  }
};

gem::RTE::Atom :: Atom(void)
  : m_pimpl(new PIMPL)
{
}

gem::RTE::Atom :: Atom(const gem::RTE::Atom&a)
  : m_pimpl(new PIMPL)
{
  m_pimpl->atom=a.m_pimpl->atom;
}
gem::RTE::Atom :: Atom(const std::string&name)
  : m_pimpl(new PIMPL)
{
  SETSYMBOL(&m_pimpl->atom, gensym(name.c_str()));
}
gem::RTE::Atom :: Atom(double f)
  : m_pimpl(new PIMPL)
{
  SETFLOAT(&m_pimpl->atom, f);
}
gem::RTE::Atom :: Atom(const t_atom*a)
  : m_pimpl(new PIMPL)
{
  if(a)
    m_pimpl->atom=*a;
  else
  SETFLOAT(&m_pimpl->atom, 0);
}
gem::RTE::Atom :: Atom(const t_symbol*name)
  : m_pimpl(new PIMPL)
{
  SETSYMBOL(&m_pimpl->atom, (t_symbol*)name);
}


gem::RTE::Atom :: ~Atom(void)
{
  delete m_pimpl;
}

gem::RTE::Atom&gem::RTE::Atom::operator=(const Atom&a)
{
  m_pimpl->atom = a.m_pimpl->atom;
  return (*this);
}
gem::RTE::Atom&gem::RTE::Atom::operator=(const t_atom*a)
{
  m_pimpl->atom = *a;
  return (*this);
}
gem::RTE::Atom&gem::RTE::Atom::operator=(const std::string&name)
{
  SETSYMBOL(&m_pimpl->atom, gensym(name.c_str()));
  return (*this);
}
gem::RTE::Atom&gem::RTE::Atom::operator=(const t_symbol*name)
{
  SETSYMBOL(&m_pimpl->atom, (t_symbol*)name);
  return (*this);
}
gem::RTE::Atom&gem::RTE::Atom::operator=(double f)
{
  SETFLOAT(&m_pimpl->atom, f);
  return (*this);
}

std::string gem::RTE::Atom::getString(void) const
{
  if(A_NULL == m_pimpl->atom.a_type)
    return "";
  return std::string(atom_getsymbol(&m_pimpl->atom)->s_name);
}
t_symbol* gem::RTE::Atom::getSymbol(void) const
{
  if(A_NULL == m_pimpl->atom.a_type)
    return gensym("");
  return atom_getsymbol(&m_pimpl->atom);
}
double gem::RTE::Atom::getFloat(void) const
{
  return atom_getfloat(&m_pimpl->atom);
}
const t_atom*gem::RTE::Atom::getRTEAtom(void) const
{
  return&m_pimpl->atom;
}

namespace {
bool do_checkSignature(int argc, const t_atom*argv, int numtypes, const int*types)
{
  if(argc>numtypes)
    return false;

  int i;
  for(i=0; i<argc; i++) {
    if (argv[i].a_type == types[i])
      continue;

    switch (argv[i].a_type) {
    case A_FLOAT:
    case A_DEFFLOAT:
      switch (types[i]) {
      case A_FLOAT:
      case A_DEFFLOAT:
        break;
      default: return false;
      }
      break;
    case A_SYMBOL:
    case A_DEFSYMBOL:
      switch (types[i]) {
      case A_SYMBOL:
      case A_DEFSYMBOL:
        break;
      default: return false;
      }
      break;
    default:
      if (argv[i].a_type != types[i])
        return false;
    }
  }
  /* for the remaining expected args, there are no atoms.
   * so we must expect *default* values
   */
  for(i=argc; i<numtypes; i++) {
    switch(types[i]) {
    case A_DEFFLOAT:
    case A_DEFSYMBOL:
      break;
    default:
      return false;
    }
  }
  return true;
}
const char*arg2name(int argtype) {
  const char*result = "?";
    switch(argtype) {
    case A_NULL: result="(none)";  break;
    case A_FLOAT: result="float";  break;
    case A_SYMBOL: result="symbol";  break;
    case A_POINTER: result="pointer";  break;
    case A_SEMI: result=";";  break;
    case A_COMMA: result=",";  break;
    case A_DEFFLOAT: result="[float]";  break;
    case A_DEFSYM: result="[symbol]";  break;
    case A_DOLLAR: result="$arg";  break;
    case A_DOLLSYM: result="$arg-sym";  break;
    case A_GIMME: result="*";  break;
    case A_CANT: result="(X)";  break;
    default: break;
    }
  return gensym(result)->s_name;
}

};

bool gem::RTE::Atom::checkSignature(int argc, const t_atom*argv, int numtypes, const int*types, const char*name) {
  if(!numtypes)
    return true;
  bool result = do_checkSignature(argc, argv, numtypes, types);
  if(name && !result) {
    pd_error(0, "[%s] wrong arguments", name);

    startpost("received:");
    for(int i=0; i<argc; i++) startpost(" %s", arg2name(argv[i].a_type));
    endpost();

    startpost("expected:");
    for(int i=0; i<numtypes; i++) startpost(" %s", arg2name(types[i]));
    endpost();
  }
  return result;
}
