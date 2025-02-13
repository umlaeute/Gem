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

#include "Gem/RTE.h"
#include "RTE/Array.h"
#include "RTE/RTE.h"


class gem::RTE::Array::PIMPL
{
public:
  t_float f;
  t_garray*A;
  t_word*pointer;
  size_t length;

  std::string name;

  PIMPL(void) :
    f(0.),
    A(NULL),
    pointer(NULL),
    length(0),
    name(std::string())
  {
  }
  ~PIMPL(void)
  {
  }

  bool check(void)
  {
    pointer=NULL;
    length=0;
    A=NULL;

    A = (t_garray *)pd_findbyclass(gensym(name.c_str()), garray_class);
    if(!A) {
      return false;
    }

    int size=0;
    t_word *words;
    if(!garray_getfloatwords(A, &size, &words)) {
      return false;
    }

    length=size;
    pointer=words;

    return true;
  }

  bool setName(const std::string&s)
  {
    name=s;
    return check();
  }

  inline t_float&get(const unsigned int&index)
  {
    if(pointer) {
      return pointer[index].w_float;
    }

    else {
      return f;
    }
  }
};


gem::RTE::Array :: Array(void)
  : m_pimpl(new PIMPL)
{


}

gem::RTE::Array :: Array(const gem::RTE::Array&a)
  : m_pimpl(new PIMPL)
{
  m_pimpl->setName(a.m_pimpl->name);
}
gem::RTE::Array :: Array(const std::string&name)
  : m_pimpl(new PIMPL)
{
  m_pimpl->setName(name);
}

gem::RTE::Array :: ~Array(void)
{
  delete m_pimpl;
}
gem::RTE::Array& gem::RTE::Array :: operator=(const gem::RTE::Array&org)
{
  m_pimpl->setName(org.m_pimpl->name);
  return (*this);
}


bool gem::RTE::Array :: isValid(void)
{
  return m_pimpl->check();
}

bool gem::RTE::Array :: name(const std::string&s)
{
  return m_pimpl->setName(s);
}
const std::string gem::RTE::Array :: name(void)
{
  return m_pimpl->name;
}
typedef void (*rte_resize_t)(t_garray *x, long n);
bool gem::RTE::Array :: resize(const size_t newsize)
{
  if(m_pimpl->A) {
    static rte_resize_t rte_resize=NULL;
    static bool rte_resize_checked=false;
    if(false==rte_resize_checked) {
      gem::RTE::RTE*rte=gem::RTE::RTE::getRuntimeEnvironment();
      if(rte) {
        rte_resize=(rte_resize_t)rte->getFunction("garray_resize_long");
      }
    }
    rte_resize_checked=true;
    if(rte_resize) {
      rte_resize(m_pimpl->A, newsize);
    } else {
#ifdef __GNUC__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#elif defined _MSC_VER
#pragma warning( push )
#pragma warning( disable : 4996 )
#endif
      garray_resize(m_pimpl->A, newsize);
#ifdef __GNUC__
#pragma GCC diagnostic pop
#elif defined _MSC_VER
#pragma warning( pop )
#endif
    }

    return (size()==newsize);
  }

  return false;
}
size_t gem::RTE::Array :: size(void)
{
  m_pimpl->check();
  return m_pimpl->length;
}

t_float&gem::RTE::Array :: operator[](const unsigned int&index)
{
  return m_pimpl->get(index);
}

void gem::RTE::Array :: set(const t_float f)
{
  if(!m_pimpl->check()) {
    return;
  }

  t_word*wp=m_pimpl->pointer;
  unsigned int i;
  for(i=0; i<m_pimpl->length; i++) {
    wp->w_float=f;
    wp++;
  }

}
