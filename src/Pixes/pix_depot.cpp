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
#include "pix_depot.h"
#include "pix_put.h"
#include "pix_get.h"
#include <string.h>
#include <stdio.h>

/////////////////////////////////////////////////////////
//
// pix_depot
//
/////////////////////////////////////////////////////////

CPPEXTERN_NEW_WITH_TWO_ARGS(pix_depot, t_symbol*,A_DEFSYM,t_float,A_FLOAT)

/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
pix_depot :: pix_depot(t_symbol *s,t_float f=100.0)
{
  if (s==&s_){
    static int depotcounter=0;
    char cbuf[16];
    sprintf(cbuf, "pix_depot_%6d", depotcounter++);
    s=gensym(cbuf);
  }

  if (f<0)f=DEFAULT_NUM_FRAMES;
  m_bindname = s;
  m_numframes = (int)f;
  m_buffer = new imageStruct[m_numframes];
  
  pd_bind(&this->x_obj->ob_pd, m_bindname); 
  outlet_new(this->x_obj, &s_float);
}
/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
pix_depot :: ~pix_depot()
{
  delete [] m_buffer;
  pd_unbind(&this->x_obj->ob_pd, m_bindname);
}
/////////////////////////////////////////////////////////
// allocateMess
//
/////////////////////////////////////////////////////////
void pix_depot :: allocateMess(int x, int y, int c)
{
  int i = m_numframes;
  while(i--){
    m_buffer[i].xsize=x;
    m_buffer[i].ysize=y;
    m_buffer[i].csize=c;
    m_buffer[i].format=0;
    m_buffer[i].reallocate();
  }    
}
/////////////////////////////////////////////////////////
// allocateMess
//
/////////////////////////////////////////////////////////
void pix_depot :: bangMess()
{
  outlet_float(this->x_obj->ob_outlet, m_numframes);
}
bool pix_depot :: putMess(imageStruct*img,int pos){
  if (pos<0 || pos>=m_numframes)return false;
  img->copy2Image(m_buffer+pos);
  return true;
}
imageStruct*pix_depot :: getMess(int pos){
  if (pos<0 || pos>=m_numframes)return 0;
  return (m_buffer+pos);
}

/////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////
void pix_depot :: obj_setupCallback(t_class *classPtr)
{
  class_addmethod(classPtr, (t_method)&pix_depot::allocateMessCallback,
  		  gensym("allocate"), A_DEFFLOAT, A_DEFFLOAT, A_DEFFLOAT, A_NULL);
  class_addbang(classPtr, (t_method)&pix_depot::bangMessCallback);
}
void pix_depot :: allocateMessCallback(void *data, t_floatarg x, t_floatarg y, t_floatarg c=4)
{
  if (x<1 || y<1 || c<0){
    error("pix_depot: init-specs out of range");
    return;
  }
  if (c==0)c=4;
  GetMyClass(data)->allocateMess((int)x, (int)y, (int)c);
}
void pix_depot :: bangMessCallback(void *data)
{
  GetMyClass(data)->bangMess();
}

/////////////////////////////////////////////////////////
//
// pix_put
//
/////////////////////////////////////////////////////////

CPPEXTERN_NEW_WITH_ONE_ARG(pix_put, t_symbol*,A_DEFSYM)

/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
pix_put :: pix_put(t_symbol *s) : m_frame(0), m_lastframe(-1) {
  setMess(s);
  inlet_new(this->x_obj, &this->x_obj->ob_pd, gensym("float"), gensym("frame"));
}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
pix_put :: ~pix_put(){

}

/////////////////////////////////////////////////////////
// setMess
//
/////////////////////////////////////////////////////////
void pix_put :: setMess(t_symbol*s){
  if (s!=&s_){
    m_bindname = s;
  }
}
/////////////////////////////////////////////////////////
// setMess
//
/////////////////////////////////////////////////////////
void pix_put :: frameMess(int f){
  if (f<0){
    error("frame# must not be less than zero (%d)", f);
  }
  m_frame=f;
}

void pix_put :: render(GemState*state){
  if (m_frame<0)return;
  if (state && state->image && &state->image->image){
    if (state->image->newimage || m_frame!=m_lastframe){
      pix_depot *depot=(pix_depot *)((Obj_header*)pd_findbyclass(m_bindname, pix_depot_class))->data;
      if (depot){
	depot->putMess(&state->image->image,m_lastframe=m_frame);
	m_frame=-1;
      }
    }
  }
}

/////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////
void pix_put :: obj_setupCallback(t_class *classPtr)
{
  class_addmethod(classPtr, (t_method)&pix_put::setMessCallback,
  		  gensym("set"), A_SYMBOL, A_NULL);
  class_addmethod(classPtr, (t_method)&pix_put::frameMessCallback,
  		  gensym("frame"), A_FLOAT, A_NULL);
}
void pix_put :: setMessCallback(void *data, t_symbol*s)
{
  GetMyClass(data)->setMess(s);
}
void pix_put :: frameMessCallback(void *data, t_floatarg f)
{
  GetMyClass(data)->frameMess((int)f);
}



/////////////////////////////////////////////////////////
//
// pix_get
//
/////////////////////////////////////////////////////////

CPPEXTERN_NEW_WITH_ONE_ARG(pix_get, t_symbol*,A_DEFSYM)

/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
pix_get :: pix_get(t_symbol *s) : m_frame(0) {
  setMess(s);
  inlet_new(this->x_obj, &this->x_obj->ob_pd, gensym("float"), gensym("frame"));
}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
pix_get :: ~pix_get(){

}

/////////////////////////////////////////////////////////
// setMess
//
/////////////////////////////////////////////////////////
void pix_get :: setMess(t_symbol*s){
  if (s!=&s_){
    m_bindname = s;
  }
}
/////////////////////////////////////////////////////////
// setMess
//
/////////////////////////////////////////////////////////
void pix_get :: frameMess(int f){
  if (f<0){
    error("frame# must not be less than zero (%d)", f);
  }
  m_frame=f;
}

void pix_get :: render(GemState*state){
  if (!state)return;
  orgPixBlock = state->image;
  if (state->image && state->image->newimage) {
    cachedPixBlock.newimage = state->image->newimage;
    state->image->image.copy2ImageStruct(&cachedPixBlock.image);
    state->image = &cachedPixBlock;
  }
  cachedPixBlock.newimage=1;
  state->image = &cachedPixBlock;
  pix_depot *depot=(pix_depot *)((Obj_header*)pd_findbyclass(m_bindname, pix_depot_class))->data;
  if (depot){
    imageStruct *img=depot->getMess(m_frame);
    if (img && img->data)img->copy2ImageStruct(&state->image->image);
    else state->image=0;
  }
}

/////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////
void pix_get :: obj_setupCallback(t_class *classPtr)
{
  class_addmethod(classPtr, (t_method)&pix_get::setMessCallback,
  		  gensym("set"), A_SYMBOL, A_NULL);
  class_addmethod(classPtr, (t_method)&pix_get::frameMessCallback,
  		  gensym("frame"), A_FLOAT, A_NULL);
}
void pix_get :: setMessCallback(void *data, t_symbol*s)
{
  GetMyClass(data)->setMess(s);
}
void pix_get :: frameMessCallback(void *data, t_floatarg f)
{
  GetMyClass(data)->frameMess((int)f);
}
