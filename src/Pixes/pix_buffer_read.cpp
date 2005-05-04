////////////////////////////////////////////////////////
//
// GEM - Graphics Environment for Multimedia
//
// zmoelnig@iem.kug.ac.at
//
// Implementation file
//
//    Copyleft  (l) 2002 IOhannes m zmölnig
//    For information on usage and redistribution, and for a DISCLAIMER OF ALL
//    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.
//
/////////////////////////////////////////////////////////

#include "pix_buffer.h"
#include "pix_buffer_read.h"

/////////////////////////////////////////////////////////
//
// pix_buffer_read
//
/////////////////////////////////////////////////////////

CPPEXTERN_NEW_WITH_ONE_ARG(pix_buffer_read, t_symbol*,A_DEFSYM)


/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
pix_buffer_read :: pix_buffer_read(t_symbol *s) : m_frame(0), m_bindname(NULL) {
  setMess(s);
  inlet_new(this->x_obj, &this->x_obj->ob_pd, gensym("float"), gensym("frame"));
}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
pix_buffer_read :: ~pix_buffer_read(){

}

/////////////////////////////////////////////////////////
// setMess
//
/////////////////////////////////////////////////////////
void pix_buffer_read :: setMess(t_symbol*s){
  if (s!=&s_){
    m_bindname = s;
  }
}
/////////////////////////////////////////////////////////
// frameMess
//
/////////////////////////////////////////////////////////
void pix_buffer_read :: frameMess(t_float f){
  if (f<0.){
    error("frame# must not be less than zero (%f)", f);
  }
  m_frame=f;
}
/////////////////////////////////////////////////////////
// autoMess
//   specify an incrementor to proceed to the next image
//
/////////////////////////////////////////////////////////
void pix_buffer_read :: autoMess(t_float f){
  m_auto=f;
}
/////////////////////////////////////////////////////////
// loopMess
//   should we loop when reaching the end of the buffer ?
//
/////////////////////////////////////////////////////////
void pix_buffer_read :: loopMess(int i){
  m_loop=(i!=0);
}
/////////////////////////////////////////////////////////
// get the image at <m_frame> from the buffer
// and insert it into the gemchain
/////////////////////////////////////////////////////////
void pix_buffer_read :: render(GemState*state){
  if (!state)return;
  orgPixBlock = state->image;
  if (state->image && state->image->newimage) {
    cachedPixBlock.newimage = state->image->newimage;
    state->image->image.copy2ImageStruct(&cachedPixBlock.image);
    state->image = &cachedPixBlock;
  }
  cachedPixBlock.newimage=1;
  state->image = &cachedPixBlock;
  if(m_bindname==NULL || m_bindname->s_name==NULL)return;
  Obj_header*ohead=(Obj_header*)pd_findbyclass(m_bindname, pix_buffer_class);
  if(ohead==NULL){
    post("pix_buffer_read: couldn't find pix_buffer '%s'", m_bindname->s_name);
    return;
  }
  pix_buffer *buffer=(pix_buffer *)(ohead)->data;


  if (buffer){
    imageStruct *img=NULL;
    if(m_loop){
      int numFrames=buffer->numFrames();
      m_frame=fmod(m_frame, numFrames);
      if(m_frame<0.f)m_frame+=numFrames;
    }
    img=buffer->getMess((int)m_frame);
    if (img && img->data)img->copy2ImageStruct(&state->image->image);
    else state->image=0;

    m_frame+=m_auto;
  }
}

/////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////
void pix_buffer_read :: obj_setupCallback(t_class *classPtr)
{
  class_addcreator((t_newmethod)_classpix_buffer_read,gensym("pix_get"),A_DEFSYM,A_NULL);
  class_addmethod(classPtr, (t_method)&pix_buffer_read::setMessCallback,
  		  gensym("set"), A_SYMBOL, A_NULL);
  class_addmethod(classPtr, (t_method)&pix_buffer_read::frameMessCallback,
  		  gensym("frame"), A_FLOAT, A_NULL);
  class_addmethod(classPtr, (t_method)&pix_buffer_read::autoMessCallback,
  		  gensym("auto"), A_FLOAT, A_NULL);
  class_addmethod(classPtr, (t_method)&pix_buffer_read::loopMessCallback,
  		  gensym("loop"), A_FLOAT, A_NULL);
}
void pix_buffer_read :: setMessCallback(void *data, t_symbol*s)
{
  GetMyClass(data)->setMess(s);
}
void pix_buffer_read :: frameMessCallback(void *data, t_floatarg f)
{
  GetMyClass(data)->frameMess((int)f);
}
void pix_buffer_read :: autoMessCallback(void *data, t_floatarg f)
{
  GetMyClass(data)->autoMess(f);
}
void pix_buffer_read :: loopMessCallback(void *data, t_floatarg f)
{
  GetMyClass(data)->loopMess(f>0.f);
}
