////////////////////////////////////////////////////////
//
// GEM - Graphics Environment for Multimedia
//
// zmoelnig@iem.kug.ac.at
//
// Implementation file
//
//    Copyright (c) 2002 IOhannes m zmoelnig. forum::für::umläute. IEM
//    this file has been generated automatically
//
//    For information on usage and redistribution, and for a DISCLAIMER OF ALL
//    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.
//
/////////////////////////////////////////////////////////

#include "GEMglEdgeFlag.h"

CPPEXTERN_NEW_WITH_ONE_ARG (GEMglEdgeFlag , t_floatarg, A_DEFFLOAT)

/////////////////////////////////////////////////////////
//
// GEMglEdgeFlag
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
GEMglEdgeFlag :: GEMglEdgeFlag(t_floatarg arg1=0) :
             		flag((GLboolean)arg1)
{
	m_inlet[0] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float, gensym("flag"));
}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
GEMglEdgeFlag :: ~GEMglEdgeFlag(){
inlet_free(m_inlet[0]);
}
/////////////////////////////////////////////////////////
// Render
//
/////////////////////////////////////////////////////////
void GEMglEdgeFlag :: render(GemState *state)
{ glEdgeFlag(flag); }


/////////////////////////////////////////////////////////
// set my variables
/////////////////////////////////////////////////////////

void GEMglEdgeFlag :: flagMess (int arg1) {
	flag = (GLboolean)arg1;
	setModified();
}



/////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////

void GEMglEdgeFlag :: obj_setupCallback(t_class *classPtr) {
        class_addcreator((t_newmethod)_classGEMglEdgeFlag,gensym("glEdgeFlag"),A_NULL);

	class_addmethod(classPtr, (t_method)&GEMglEdgeFlag::flagMessCallback, gensym("flag"), A_NULL);
}


void GEMglEdgeFlag :: flagMessCallback (   void* data, t_floatarg    arg0) {
	GetMyClass(data)->flagMess ( (t_int)    arg0);
}
