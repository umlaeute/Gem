////////////////////////////////////////////////////////
//
// GEM - Graphics Environment for Multimedia
//
// cclepp@pop.artic.edu
// tigital@users.sourceforge.net
//
// Implementation file
//
//    Copyright (c) 1997-1999 Mark Danks.
//    Copyright (c) Günther Geiger.
//    Copyright (c) 2001-2002 IOhannes m zmoelnig. forum::für::umläute
//    Copyright (c) 2002 James Tittle & Chris Clepper
//
//    For information on usage and redistribution, and for a DISCLAIMER OF ALL
//    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.
//
/////////////////////////////////////////////////////////

#include "yuv_dual.h"

#include "GemCache.h"

/////////////////////////////////////////////////////////
//
// yuv_dual
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
yuv_dual :: yuv_dual()
    	       : m_cacheRight(NULL), m_pixRightValid(-1)
{
post("yuv_dual:constructed");
    m_inlet = inlet_new(this->x_obj, &this->x_obj->ob_pd, gensym("gem_state"), gensym("gem_right"));
    memset(&m_pixRight, 0, sizeof(m_pixRight));
}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
yuv_dual :: ~yuv_dual()
{
    inlet_free(m_inlet);
}

/////////////////////////////////////////////////////////
// processImage
//
/////////////////////////////////////////////////////////
void yuv_dual :: processImage(imageStruct &image)
{
    post("yuv_dual: no RGB support :-P");
}

/////////////////////////////////////////////////////////
// do the YUV processing here
//
/////////////////////////////////////////////////////////
void yuv_dual :: processYUVImage(imageStruct &image)
{
   //post("yuv_dual:processGrayImage");
   if (!m_pixRight) return;
    
    if (image.xsize != m_pixRight->image.xsize ||
    	image.ysize != m_pixRight->image.ysize)
    {
    	error("GEM: yuv_dual: two images do not have equal dimensions");
    	m_pixRight = NULL;
    	return;
    }
   processDualYUV(image, m_pixRight->image);

}

/////////////////////////////////////////////////////////
// processBothGray
//
/////////////////////////////////////////////////////////
void yuv_dual :: processDualYUV(imageStruct &, imageStruct &)
{
	error("GEM: yuv_dual: DualYUV");
}

/////////////////////////////////////////////////////////
// processLeftGray
//
/////////////////////////////////////////////////////////
void yuv_dual :: processLeftYUV(imageStruct &, imageStruct &)
{
	error("GEM: yuv_dual: LeftYUV");
}

/////////////////////////////////////////////////////////
// processRightGray
//
/////////////////////////////////////////////////////////
void yuv_dual :: processRightYUV(imageStruct &, imageStruct &)
{
	error("GEM: yuv_dual: RightYUV");
}

/////////////////////////////////////////////////////////
// postrender
//
/////////////////////////////////////////////////////////
void yuv_dual :: postrender(GemState *state)
{
  if (org_pixRightValid != m_pixRightValid)setPixModified();

  org_pixRightValid = m_pixRightValid;

  m_pixRightValid = 0;
}

/////////////////////////////////////////////////////////
// stopRendering
//
/////////////////////////////////////////////////////////
void yuv_dual :: stopRendering()
{
    //m_cacheRight = NULL;
    //m_pixRight = NULL;
    m_pixRightValid = 0;
}

/////////////////////////////////////////////////////////
// rightRender
//
/////////////////////////////////////////////////////////
void yuv_dual :: rightRender(GemState *statePtr)
{
/*    m_pixRight = statePtr->image;
    if (!statePtr->image) return;
    if (statePtr->image->newimage) setPixModified();
*/
  if (!statePtr || !statePtr->image) {
    m_pixRightValid = 0;
    m_pixRight = 0;
    return;
  }
  
  m_pixRightValid = 1;
  m_pixRight = statePtr->image;
 
  if (statePtr->image->newimage)setPixModified(); // force the left arm to create a new image
}

/////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////
void yuv_dual :: obj_setupCallback(t_class *classPtr)
{
    class_addmethod(classPtr, (t_method)&yuv_dual::gem_rightMessCallback,
    	    //gensym("gem_right"), A_POINTER, A_POINTER, A_NULL);
            gensym("gem_right"), A_GIMME, A_NULL);
}
void yuv_dual :: gem_rightMessCallback(void *data, t_symbol *s, int argc, t_atom *argv)
{
    if (argc==1 && argv->a_type==A_FLOAT){
    } else if (argc==2 && argv->a_type==A_POINTER && (argv+1)->a_type==A_POINTER){
    GetMyClass(data)->m_cacheRight = (GemCache*)argv->a_w.w_gpointer;
    GetMyClass(data)->rightRender((GemState *)(argv+1)->a_w.w_gpointer);
  } else error("GEM: wrong righthand arguments....");
}

/*void yuv_dual :: rightRenderCallback(GemBase *data, GemState *state)
{
    ((yuv_dual *)data)->rightRender(state);
}
void yuv_dual :: rightPostrenderCallback(GemBase *data, GemState *state)
{
    ((yuv_dual *)data)->rightPostrender(state);
}
*/