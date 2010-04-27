/*
 *  pix_record.cpp
 *
 */
#include "Base/GemConfig.h"
#include "pix_record.h"

#include "Base/GemState.h"

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
  m_banged(false), m_automatic(true),
  m_recordStart(0), m_recordStop(0), 
  m_outNumFrames(NULL), m_outInfo(NULL),
  m_currentFrame(-1),
  m_minFrames(0), m_maxFrames(0),
  m_numCodecs(0),
  m_handle(NULL)
{
  int xoff = 0, yoff = 0;
  int width = 0, height = 0;
  if (argc == 4) {
    xoff = atom_getint(&argv[0]);
    yoff = atom_getint(&argv[1]);
    width = atom_getint(&argv[2]);
    height = atom_getint(&argv[3]);
  } else if (argc == 2) {
    width = atom_getint(&argv[0]);
    height = atom_getint(&argv[1]);
  } else if (argc != 0){
    error("needs 0, 2, or 4 values");
    xoff = yoff = 0;
    width = height = 128;
  }

  gem::PluginFactory<gem::record>::loadPlugins("record");

  m_outNumFrames = outlet_new(this->x_obj, 0);
  m_outInfo      = outlet_new(this->x_obj, 0);

  m_handle=gem::PluginFactory<gem::record>::getInstance("QT");

  if(!m_handle)
    m_handle=gem::PluginFactory<gem::record>::getInstance("QT4L");
 
  if(NULL==m_handle) {
    std::vector<std::string>ids=gem::PluginFactory<gem::record>::getIDs();
    int i=0;
    for(i=0; i<ids.size(); i++) {
      m_handle=gem::PluginFactory<gem::record>::getInstance(ids[i]);
      if(m_handle)break;
    }
  }

  if(!m_handle) {
    error("no recording backends found!");
  }
}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
pix_record :: ~pix_record()
{
  if(m_handle)delete m_handle;
  outlet_free(m_outNumFrames);
  outlet_free(m_outInfo);
}

//
// stops recording into the movie
//
void pix_record :: stopRecording()
{
  if(m_handle){
    m_handle->close();
    m_currentFrame = 0; //reset the frame counter?
    outlet_float(m_outNumFrames,m_currentFrame);
    verbose(1, "movie written");
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
	  m_handle->m_recordStart=m_recordStart;
	  m_handle->m_recordStop=m_recordStop;
	  m_handle->m_maxFrames = m_maxFrames;
	  m_handle->m_minFrames = m_minFrames;
	  
	  if(m_maxFrames != 0 && m_currentFrame >= m_maxFrames) m_recordStop = 1;
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
      error("could not set new dimensions");
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
      error("could not set offset");
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
      error("unable to open settings dialog");
    }
  }
}
/////////////////////////////////////////////////////////
// dialogMess
//
/////////////////////////////////////////////////////////
void pix_record :: recordMess(bool on)
{
  if (on) {
    m_recordStart=1;
    m_recordStop=0;
    verbose(1, "recording on!");
  }else{
    m_recordStart=0;
    m_recordStop=1;
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
      t_atom ap[2];
	  const char*codecname=m_handle->getCodecName(i);
	  const char*descr=m_handle->getCodecDescription(i);
	  if(codecname) {
		  post("codec%d: '%s': %s", i, codecname, (descr?descr:""));
       SETFLOAT (ap+0, static_cast<t_float>(i));
       SETSYMBOL(ap+1, gensym(codecname));
       outlet_anything(m_outInfo, gensym("codec"), 2, ap);
	  }
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
    if    (A_SYMBOL==argv->a_type)err=m_handle->setCodec(atom_getsymbol(argv)->s_name);
    else if(A_FLOAT==argv->a_type)err=m_handle->setCodec(atom_getint(argv));

    if(!err)error("unable to set the codec");
  }
}

void pix_record :: fileMess(int argc, t_atom *argv)
{
  /* LATER let the record()-handles chose whether they accept an open request
   * and then try other handles (if available)
   * this would allow to use this object for streaming, virtual output devices,...
   */
  if(m_handle&&argc){
  
    int err=0;
#ifndef HAVE_QUICKTIME
    // ??? but why:
    m_handle->close();
#endif
    switch(argc){
    case 1:
      err=m_handle->open((const char*)atom_getsymbol(argv)->s_name);
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
  class_addmethod(classPtr, reinterpret_cast<t_method>(&pix_record::fileMessCallback),
		  gensym("file"), A_GIMME, A_NULL);
  class_addmethod(classPtr, reinterpret_cast<t_method>(&pix_record::autoMessCallback),
		  gensym("auto"), A_FLOAT, A_NULL);
  class_addbang(classPtr, reinterpret_cast<t_method>(&pix_record::bangMessCallback));

  class_addmethod(classPtr, reinterpret_cast<t_method>(&pix_record::recordMessCallback),
		  gensym("record"), A_FLOAT, A_NULL);

  class_addmethod(classPtr, reinterpret_cast<t_method>(&pix_record::sizeMessCallback),
		  gensym("size"), A_FLOAT, A_FLOAT, A_NULL);
  class_addmethod(classPtr, reinterpret_cast<t_method>(&pix_record::sizeMessCallback),
		  gensym("dimen"), A_FLOAT, A_FLOAT, A_NULL);
  class_addmethod(classPtr, reinterpret_cast<t_method>(&pix_record::posMessCallback),
		  gensym("vert_pos"), A_FLOAT, A_FLOAT, A_NULL);
  class_addmethod(classPtr, reinterpret_cast<t_method>(&pix_record::posMessCallback),
		  gensym("offset"), A_FLOAT, A_FLOAT, A_NULL);

  class_addmethod(classPtr, reinterpret_cast<t_method>(&pix_record::dialogMessCallback),
		  gensym("dialog"),  A_NULL);
  class_addmethod(classPtr, reinterpret_cast<t_method>(&pix_record::codeclistMessCallback),
		  gensym("codeclist"),  A_NULL);
  class_addmethod(classPtr, reinterpret_cast<t_method>(&pix_record::codecMessCallback),
		  gensym("codec"), A_GIMME, A_NULL);
  class_addmethod(classPtr, reinterpret_cast<t_method>(&pix_record::minMessCallback),
		  gensym("min_frames"), A_FLOAT, A_NULL);
	class_addmethod(classPtr, reinterpret_cast<t_method>(&pix_record::maxMessCallback),
		  gensym("max_frames"), A_FLOAT, A_NULL);
}

void pix_record :: fileMessCallback(void *data, t_symbol *s, int argc, t_atom *argv)
{
  GetMyClass(data)->fileMess(argc, argv);
}
void pix_record :: autoMessCallback(void *data, t_floatarg on)
{
  bool onb=static_cast<bool>(on);
  GetMyClass(data)->m_automatic=onb;
}
void pix_record :: bangMessCallback(void *data)
{
  GetMyClass(data)->m_banged=true;
}

void pix_record :: sizeMessCallback(void *data, t_floatarg width, t_floatarg height)
{
  GetMyClass(data)->sizeMess(static_cast<int>(width), static_cast<int>(height));
}
void pix_record :: posMessCallback(void *data, t_floatarg x, t_floatarg y)
{
  GetMyClass(data)->posMess(static_cast<int>(x), static_cast<int>(y));
}

void pix_record :: recordMessCallback(void *data, t_floatarg on)
{
  GetMyClass(data)->recordMess(!(!static_cast<int>(on)));
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

void pix_record :: minMessCallback(void *data, t_floatarg min)
{
  GetMyClass(data)->m_minFrames=(static_cast<int>(min));
}

void pix_record :: maxMessCallback(void *data, t_floatarg max)
{
  GetMyClass(data)->m_maxFrames=(static_cast<int>(max));
}
