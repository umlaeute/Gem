////////////////////////////////////////////////////////
//
// GEM - Graphics Environment for Multimedia
//
// zmoelnig@iem.at
//
// Implementation file
//
//    Copyright (c) 1997-1999 Mark Danks.
//    Copyright (c) Günther Geiger.
//    Copyright (c) 2001-2011 IOhannes m zmölnig. forum::für::umläute. IEM. zmoelnig@iem.at
//
//    For information on usage and redistribution, and for a DISCLAIMER OF ALL
//    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.
//
/////////////////////////////////////////////////////////

#include "GemPathBase.h"
#include "Utils/Functions.h"

// CPPEXTERN_NEW_WITH_GIMME(GemPathBase);

/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
GemPathBase :: GemPathBase(int argc, t_atom *argv)
  : m_numDimens(1), m_arrayname(NULL),
    m_out1(NULL)
{
  m_out1 = outlet_new(this->x_obj, 0);

  switch(argc) {
  case 0:
    break;
  default:
    openMess(atom_getsymbol(&argv[1]));
    /* fall throught */
  case 1:
    m_numDimens = (int)atom_getfloat(&argv[0]);
    if (m_numDimens < 1) {
      m_numDimens = 1;
    }
    if (m_numDimens > 64) {
      error("too many dimensions, must be below 64");
      m_numDimens = 64;
    }
    break;
  }
}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
GemPathBase :: ~GemPathBase()
{
  outlet_free(m_out1);
}

/////////////////////////////////////////////////////////
// openMess
//
/////////////////////////////////////////////////////////
void GemPathBase :: openMess(t_symbol* arrayname)
{
  m_arrayname = arrayname;
  m_warnedNonExistent = false;
}
void GemPathBase :: floatMess(t_float val)
{
  const char*arrayname = m_arrayname?(m_arrayname->s_name):0;
  if(!arrayname)
    return;

  if(!m_array.name(arrayname)) {
    if(!m_warnedNonExistent)
      error("no array '%s'", arrayname);
    m_warnedNonExistent = true;
    return;
  }
  m_warnedNonExistent = false;

  if (m_array.size() % m_numDimens) {
    error("size %d is not a multiple of dimensions %d", m_array.size(), m_numDimens);
    return;
  }

  t_float output[64];
  lookupFunc(val, output, m_numDimens, m_array);

  t_atom argv[64];
  for (int i = 0; i < m_numDimens; i++) {
    SETFLOAT((&argv[i]), output[i]);
  }

  outlet_list(m_out1, &s_list, m_numDimens, argv);
}

/////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////
void GemPathBase :: obj_setupCallback(t_class *classPtr)
{
  CPPEXTERN_MSG1(classPtr, "open", openMess, t_symbol*);
  CPPEXTERN_MSG1(classPtr, "float", floatMess, t_float);
}
