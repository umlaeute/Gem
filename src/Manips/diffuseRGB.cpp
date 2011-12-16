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
//    For information on usage and redistribution, and for a DISCLAIMER OF ALL
//    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.
//
/////////////////////////////////////////////////////////

#include "diffuseRGB.h"
#include "Gem/Exception.h"

CPPEXTERN_NEW_WITH_GIMME(diffuseRGB);

/////////////////////////////////////////////////////////
//
// diffuseRGB
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
diffuseRGB :: diffuseRGB(int argc, t_atom *argv)
{
    if (argc == 4)
    {
        m_vector[0] = atom_getfloat(&argv[0]);
        m_vector[1] = atom_getfloat(&argv[1]);
        m_vector[2] = atom_getfloat(&argv[2]);
        m_vector[3] = atom_getfloat(&argv[3]);
    }
    else if (argc == 3)
    {
        m_vector[0] = atom_getfloat(&argv[0]);
        m_vector[1] = atom_getfloat(&argv[1]);
        m_vector[2] = atom_getfloat(&argv[2]);
        m_vector[3] = 1.0f;
    }
    else if (argc == 0)
    {
        m_vector[0] = 0.8f;
        m_vector[1] = 0.8f;
        m_vector[2] = 0.8f;
        m_vector[3] = 1.0f;
    }
    else
      {
        throw(GemException("needs 0, 3 or 4 arguments"));
      }

    // create the new inlets
    inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("rVal"));
    inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("gVal"));
    inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("bVal"));
    inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("aVal"));
}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
diffuseRGB :: ~diffuseRGB()
{ }

/////////////////////////////////////////////////////////
// postrender
//
/////////////////////////////////////////////////////////
void diffuseRGB :: postrender(GemState *)
{
	glEnable(GL_COLOR_MATERIAL);
}

/////////////////////////////////////////////////////////
// render
//
/////////////////////////////////////////////////////////
void diffuseRGB :: render(GemState *)
{
	glDisable(GL_COLOR_MATERIAL);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, m_vector);
}

/////////////////////////////////////////////////////////
// rMess
//
/////////////////////////////////////////////////////////
void diffuseRGB :: rMess(float val)
{
    m_vector[0] = val;
    setModified();
}

/////////////////////////////////////////////////////////
// gMess
//
/////////////////////////////////////////////////////////
void diffuseRGB :: gMess(float val)
{
    m_vector[1] = val;
    setModified();
}

/////////////////////////////////////////////////////////
// bMess
//
/////////////////////////////////////////////////////////
void diffuseRGB :: bMess(float val)
{
    m_vector[2] = val;
    setModified();
}

/////////////////////////////////////////////////////////
// aMess
//
/////////////////////////////////////////////////////////
void diffuseRGB :: aMess(float val)
{
    m_vector[3] = val;
    setModified();
}

/////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////
void diffuseRGB :: obj_setupCallback(t_class *classPtr)
{
  CPPEXTERN_MSG1(classPtr, "rVal", rMess, float);
  CPPEXTERN_MSG1(classPtr, "gVal", gMess, float);
  CPPEXTERN_MSG1(classPtr, "bVal", bMess, float);
  CPPEXTERN_MSG1(classPtr, "aVal", aMess, float);
}
