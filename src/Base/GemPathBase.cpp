////////////////////////////////////////////////////////
//
// GEM - Graphics Environment for Multimedia
//
// zmoelnig@iem.kug.ac.at
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
  : m_numDimens(1), m_array(NULL),
    m_out1(NULL)
{
  m_out1 = outlet_new(this->x_obj, 0);

  if (argc >= 2)
    openMess(atom_getsymbol(&argv[1]));
  if (argc >= 1) {
      m_numDimens = (int)atom_getfloat(&argv[0]);
      if (m_numDimens < 1) m_numDimens = 1;
      if (m_numDimens > 64) {
	  error("too many dimensions, must be below 64");
	  m_numDimens = 64;
	}
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
void GemPathBase :: openMess(t_symbol *arrayname)
{
  m_array = (t_garray *)pd_findbyclass(arrayname, garray_class);
  if (!m_array) {
      error("unable to find array %s", arrayname->s_name);
      return;
    }
}
void GemPathBase :: floatMess(t_float val)
{
    if (!m_array) {
        error("no array");
        return;
    }

    int size;
    t_word *vec;
    if (!garray_getfloatwords(m_array, &size, &vec))
        return;

    if (size % m_numDimens) {
      error("size is not a mod of dimensions");
      return;
    }

    t_float output[64];
    lookupFunc(val, output, m_numDimens, size / m_numDimens, &(vec->w_float));

    t_atom argv[64];
    for (int i = 0; i < m_numDimens; i++)
        SETFLOAT((&argv[i]), output[i]);

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
