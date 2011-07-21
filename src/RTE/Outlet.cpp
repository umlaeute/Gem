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
// a wrapper for accessing the RTE's outlets
//
/////////////////////////////////////////////////////////
#include "RTE/Outlet.h"
#include "Base/CPPExtern.h"
#include "Gem/RTE.h"


class gem::RTE::Outlet::PIMPL {
public:
  t_outlet*outlet;
  PIMPL(CPPExtern*parent) : outlet(NULL) {
    outlet=outlet_new(parent->x_obj, 0);
  }
  ~PIMPL(void) {
    if(outlet)outlet_free(outlet);
    outlet=NULL;
  }
  static bool any2atom(const gem::any value, t_atom&atom) {
    const std::type_info*typ=&value.get_type();

    double d=0;
    std::string s;
    bool issymbol=false;


#define ISTYPE(type) (*typ == typeid(type))
    if (value.empty()) {
      return false;
    } else if(ISTYPE(char)) {
      issymbol=false;
      d=any_cast<char>(value);
    } else if(ISTYPE(unsigned char)) {
      issymbol=false;
      d=any_cast<unsigned char>(value);

    } else if(ISTYPE(short)) {
      issymbol=false;
      d=any_cast<short>(value);
    } else if(ISTYPE(unsigned short)) {
      issymbol=false;
      d=any_cast<unsigned short>(value);

    } else if(ISTYPE(int)) {
      issymbol=false;
      d=any_cast<int>(value);
    } else if(ISTYPE(unsigned int)) {
      issymbol=false;
      d=any_cast<unsigned int>(value);
#if 0
      /* "long"s cannot be stored in "double"s without loosing precision... */
    } else if(ISTYPE(long)) {
      issymbol=false;
      d=any_cast<long>(value);
    } else if(ISTYPE(unsigned long)) {
      issymbol=false;
      d=any_cast<unsigned long>(value);
#endif
    } else if(ISTYPE(float)) {
      issymbol=false;
      d=any_cast<float>(value);
    } else if(ISTYPE(double)) {
      issymbol=false;
      d=any_cast<double>(value);

    } else if(ISTYPE(char*)) {
      issymbol=true;
      s=std::string(any_cast<char*>(value));
#ifdef SETSYMBOL
      /* only use this, if compiled with Pd... */
    } else if(ISTYPE(t_symbol*)) {
      issymbol=true;
      s=std::string((any_cast<t_symbol*>(value))->s_name);
#endif
    } else if(ISTYPE(std::string)) {
      issymbol=true;
      s=any_cast<std::string>(value);
    } else
      return false;

    if(issymbol) {
      SETSYMBOL(&atom, gensym(s.c_str()));
    } else {
      SETFLOAT(&atom, d);
    }
    return true;
  }

};


gem::RTE::Outlet :: Outlet(CPPExtern*parent)
  : m_pimpl(new PIMPL(parent))
{

}

gem::RTE::Outlet :: ~Outlet(void)
{
  delete m_pimpl;
} 

void gem::RTE::Outlet :: send(void) {
  outlet_bang(m_pimpl->outlet);
}
void gem::RTE::Outlet :: send(double f) {
  outlet_float(m_pimpl->outlet, f);
}
void gem::RTE::Outlet :: send(std::string selector, std::vector<gem::any>data) {
  t_atom*atomlist=new t_atom[data.size()];
  unsigned int count=0;
  unsigned int i;
  for(i=0; i<data.size(); i++) {
    if(m_pimpl->any2atom(data[i], atomlist[i]))
      count++;
  }
  
  outlet_anything(m_pimpl->outlet, gensym(selector.c_str()), count, atomlist);
  delete[]atomlist;
}
