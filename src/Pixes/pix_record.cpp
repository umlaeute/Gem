/*
 *  pix_record.cpp
 *
 */

#include "pix_record.h"

#include "Pixes/recordQT.h"
#include "Pixes/recordQT4L.h"

CPPEXTERN_NEW_WITH_GIMME(pix_record)

/////////////////////////////////////////////////////////
//
// pix_record
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
pix_record :: pix_record(int argc, t_atom *argv):
  m_recordStart(0), m_recordStop(0), 
  m_automatic(false), m_banged(false),
  m_currentFrame(-1),
  m_handle(NULL)
{
  int m_xoff = 0, m_yoff = 0;
  int m_width = 0, m_height = 0;
  if (argc == 4) {
    m_xoff = (int)atom_getfloat(&argv[0]);
    m_yoff = (int)atom_getfloat(&argv[1]);
    m_width = (int)atom_getfloat(&argv[2]);
    m_height = (int)atom_getfloat(&argv[3]);
  } else if (argc == 2) {
    m_width = (int)atom_getfloat(&argv[0]);
    m_height = (int)atom_getfloat(&argv[1]);
  } else if (argc != 0){
    error("GEM: pix_record: needs 0, 2, or 4 values");
    m_xoff = m_yoff = 0;
    m_width = m_height = 128;
	 
  }

  inlet_new(this->x_obj, &this->x_obj->ob_pd, gensym("list"), gensym("vert_pos"));
  inlet_new(this->x_obj, &this->x_obj->ob_pd, gensym("list"), gensym("size"));
  
  m_outNumFrames = outlet_new(this->x_obj, 0);

  m_automatic = false;
  m_banged = false;

#ifdef HAVE_QUICKTIME
  m_handle=new recordQT(m_xoff, m_yoff, m_width, m_height);
#elif defined HAVE_LIBQUICKTIME
  m_handle=new recordQT4L(m_xoff, m_yoff, m_width, m_height);
#endif
}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
pix_record :: ~pix_record()
{
  if(m_handle)delete m_handle;
}

//
// stops recording into the movie
//
void pix_record :: stopRecording()
{
  if(m_handle){
    m_handle->close();
    m_currentFrame = 0; //reset the frame counter?
    //outlet_float(m_outNumFrames,m_currentFrame);
    post("pix_record : movie written");
  }

}


/////////////////////////////////////////////////////////
// render
//
/////////////////////////////////////////////////////////
void pix_record :: render(GemState *state)
{
  //check if state exists
  if(!state || !state->image)return;
  if(!m_handle)return;

  if(m_recordStart){
    m_recordStop=0;
    if(m_banged||m_automatic){
      m_currentFrame=m_handle->putFrame(&state->image->image);
      m_banged=false;
      if(m_currentFrame<0)m_recordStop=1;
      outlet_float(m_outNumFrames,m_currentFrame);
    }
  }

  if(m_recordStop){
    m_recordStart=0;
    stopRecording();
    m_recordStop=0;
  }
}


/////////////////////////////////////////////////////////
// sizeMess
//
/////////////////////////////////////////////////////////
void pix_record :: sizeMess(int width, int height)
{
  if(m_handle){
    if(!m_handle->size(width, height)){
      error("[pix_record]: could not set new dimensions");
    }
  }
}

/////////////////////////////////////////////////////////
// posMess
//
/////////////////////////////////////////////////////////
void pix_record :: posMess(int x, int y)
{
  if(m_handle){
    if(!m_handle->position(x, y)){
      error("[pix_record]: could not set offset");
    }
  }
}

/////////////////////////////////////////////////////////
// dialogMess
//
/////////////////////////////////////////////////////////
void pix_record :: dialogMess()
{
  if(m_handle){
    if(!m_handle->dialog()){
      error("[pix_record]: unable to open settings dialog");
    }
  }
}

/////////////////////////////////////////////////////////
// spits out a list of installed codecs and stores them
//
/////////////////////////////////////////////////////////
void pix_record :: getCodecList()
{
  /* LATER think about sending this to an outlet instead of the console */
  if(m_handle){
    int i=0;
    int count=m_handle->getNumCodecs();
    for(i=0; i<count; i++){
      post("[pix_record]: codec%d: '%s': %s", i, m_handle->getCodecName(i), m_handle->getCodecDescription(i));
    }
  }
}


/////////////////////////////////////////////////////////
// deals with the name of a codec
//
/////////////////////////////////////////////////////////
void pix_record :: codecMess(t_atom *argv)
{
  if(m_handle){
    int err=0;
    post("codec-mess");
    if    (A_SYMBOL==argv->a_type)err=m_handle->setCodec(atom_getsymbol(argv)->s_name);
    else if(A_FLOAT==argv->a_type)err=m_handle->setCodec(atom_getint(argv));

    if(!err)error("[pix_record]: unable to set the codec");
  }
}

void pix_record :: fileMess(int argc, t_atom *argv)
{
  /* LATER let the record()-handles chose whether they accept a handle
   * and then try other handles (if available)
   * this would allow to use this object for streaming, virtual output devices,...
   */
  if(m_handle&&argc){
    int err=0;
    m_handle->close();
    switch(argc){
    case 1:
      err=m_handle->open(atom_getsymbol(argv)->s_name);
      break;
    }
  }
}

/////////////////////////////////////////////////////////
// static member functions
//
/////////////////////////////////////////////////////////
void pix_record :: obj_setupCallback(t_class *classPtr)
{
  class_addmethod(classPtr, (t_method)&pix_record::fileMessCallback,
		  gensym("file"), A_GIMME, A_NULL);
  class_addmethod(classPtr, (t_method)&pix_record::autoMessCallback,
		  gensym("auto"), A_FLOAT, A_NULL);
  class_addbang(classPtr, (t_method)&pix_record::bangMessCallback);

  class_addmethod(classPtr, (t_method)&pix_record::sizeMessCallback,
		  gensym("size"), A_FLOAT, A_FLOAT, A_NULL);
  class_addmethod(classPtr, (t_method)&pix_record::posMessCallback,
		  gensym("vert_pos"), A_FLOAT, A_FLOAT, A_NULL);
  class_addmethod(classPtr, (t_method)&pix_record::recordMessCallback,
		  gensym("record"), A_FLOAT, A_NULL);
  class_addmethod(classPtr, (t_method)&pix_record::dialogMessCallback,
		  gensym("dialog"),  A_NULL);
  class_addmethod(classPtr, (t_method)&pix_record::codeclistMessCallback,
		  gensym("codeclist"),  A_NULL);
  class_addmethod(classPtr, (t_method)&pix_record::codecMessCallback,
		  gensym("codec"), A_GIMME, A_NULL);
}

void pix_record :: fileMessCallback(void *data, t_symbol *s, int argc, t_atom *argv)
{
  GetMyClass(data)->fileMess(argc, argv);
}
void pix_record :: autoMessCallback(void *data, t_floatarg on)
{
  GetMyClass(data)->m_automatic=(on!=0);
}
void pix_record :: bangMessCallback(void *data)
{
  GetMyClass(data)->m_banged=true;
}

void pix_record :: sizeMessCallback(void *data, t_floatarg width, t_floatarg height)
{
  GetMyClass(data)->sizeMess((int)width, (int)height);
}
void pix_record :: posMessCallback(void *data, t_floatarg x, t_floatarg y)
{
  GetMyClass(data)->posMess((int)x, (int)y);
}

void pix_record :: recordMessCallback(void *data, t_floatarg on)
{
  if (!(!(int)on)) {
    GetMyClass(data)->m_recordStart=1;
    GetMyClass(data)->m_recordStop=0;
    post("pix_record: recording on!");
  }else{
    GetMyClass(data)->m_recordStart=0;
    GetMyClass(data)->m_recordStop=1;
  }
}

void pix_record :: dialogMessCallback(void *data)
{
  GetMyClass(data)->dialogMess();
}

void pix_record :: codeclistMessCallback(void *data)
{
  GetMyClass(data)->getCodecList();
}

void pix_record :: codecMessCallback(void *data, t_symbol *s, int argc, t_atom *argv)
{
  if(argc)
    GetMyClass(data)->codecMess(argv);
}
