////////////////////////////////////////////////////////
//
// GEM - Graphics Environment for Multimedia
//
// zmoelnig@iem.kug.ac.at
//
// Implementation file
//
//    Copyright (c) 1997-1999 Mark Danks.
//    Copyright (c) G�nther Geiger.
//    Copyright (c) 2001-2002 IOhannes m zmoelnig. forum::f�r::uml�ute. IEM
//    For information on usage and redistribution, and for a DISCLAIMER OF ALL
//    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.
//
/////////////////////////////////////////////////////////

#include "spline_path.h"

#include "Base/GemFuncUtil.h"

CPPEXTERN_NEW_WITH_GIMME(spline_path)

/////////////////////////////////////////////////////////
//
// separator
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
spline_path :: spline_path(int argc, t_atom *argv)
             : GemPathBase(argc, argv)
{ }

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
spline_path :: ~spline_path()
{ }

/////////////////////////////////////////////////////////
// floatMess
//
/////////////////////////////////////////////////////////
void spline_path :: floatMess(float val)
{
    if (!m_array)
    {
        error("GEM: spline_path: no array");
        return;
    }

    int size;
    t_float *vec;
    if (!garray_getfloatarray(m_array, &size, &vec))
        return;

    if (size % m_numDimens)
    {
        error("GEM: spline_path: size is not a mod of dimensions");
        return;
    }

    float output[64];
    splineFunc(val, output, m_numDimens, size / m_numDimens, vec);

	t_atom argv[64];
    for (int i = 0; i < m_numDimens; i++)
        SETFLOAT((&argv[i]), output[i]);

	outlet_list(m_out1, &s_list, m_numDimens, argv);
}

/////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////
void spline_path :: obj_setupCallback(t_class *)
{ }
