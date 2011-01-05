/*
 *  pix_record.cpp
 *
 */
#include "Base/GemConfig.h"
#include "pix_record.h"

#include "Base/GemState.h"
#include "Base/GemException.h"

CPPEXTERN_NEW_WITH_GIMME(pix_record)

struct pix_record :: PIMPL {
  PIMPL(void) {};
  ~PIMPL(void) {};

  struct codechandle {
    codechandle(gem::record*h, const std::string c):handle(h), codec(c) {}

    gem::record*handle;
    const std::string codec;
  };
  std::map<std::string, codechandle*>m_codechandle;
  std::vector<std::string>m_codecs;

  void addCodecHandle(gem::record*handle, const std::string codec) {
    codechandle*ch=new codechandle(handle, codec);
    if(m_codechandle[codec]==NULL) {
      m_codechandle[codec]=ch;
    } else {
#warning handle duplicate codecs
    }
    m_codecs.push_back(codec);
  }
  void clearCodecHandle(void) {
#warning memleak codechandle

    m_codecs.clear();
    m_codechandle.clear();
  }



  static gem::any atom2any(t_atom*ap) {
    gem::any result;
    if(ap) {
      switch(ap->a_type) {
      case A_FLOAT:
	result=atom_getfloat(ap);
	break;
      case A_SYMBOL:
	result=atom_getsymbol(ap)->s_name;
	break;
      default:
	result=ap->a_w.w_gpointer;
      }
    }
    return result;
  }
  static void addProperties(gem::Properties&props, int argc, t_atom*argv)
  {
    if(!argc)return;
    
    if(argv->a_type != A_SYMBOL) {
      ::error("no key given...");
      return;
    }
    std::string key=std::string(atom_getsymbol(argv)->s_name);
    std::vector<gem::any> values;
    argc--; argv++;
    while(argc-->0) {
      values.push_back(atom2any(argv++));
    }
    switch(values.size()) {
    default:
      props.set(key, values);
      break;
    case 1:
      props.set(key, values[0]);
      break;
    case 0: 
      {
	gem::any dummy;
	props.set(key, dummy);
      }
      break;
    }
  }
  
};

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
  m_recording(false),
  m_handle(NULL),
  m_driver(-1),
  m_pimpl(new PIMPL())
{
  if (argc != 0){
    error("ignoring arugments");
  }
  m_outNumFrames = outlet_new(this->x_obj, 0);
  m_outInfo      = outlet_new(this->x_obj, 0);


  gem::PluginFactory<gem::record>::loadPlugins("record");
  std::vector<std::string>ids=gem::PluginFactory<gem::record>::getIDs();
  addHandle(ids, "QT");
  addHandle(ids, "QT4L");
  addHandle(ids);


  if(m_handles.size()>0) {
    m_driver=-1;
  } else {
    error("no video backends found!");
  }

  getCodecList();
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

  if(m_pimpl)delete m_pimpl;
}


/////////////////////////////////////////////////////////
// add backends
//
/////////////////////////////////////////////////////////
bool pix_record :: addHandle( std::vector<std::string>available, std::string ID)
{
  unsigned int i=0;
  int count=0;

  std::vector<std::string>id;
  if(!ID.empty()) {
    // if requested 'cid' is in 'available' add it to the list of 'id's
    if(std::find(available.begin(), available.end(), ID)!=available.end()) {
      id.push_back(ID);
    } else {
      // request for an unavailable ID
      verbose(2, "backend '%s' unavailable", ID.c_str());
      return false;
    }
  } else {
    // no 'ID' given: add all available IDs
    id=available;
  }

  for(i=0; i<id.size(); i++) {
    std::string key=id[i];
    verbose(2, "trying to add '%s' as backend", key.c_str());
    if(std::find(m_ids.begin(), m_ids.end(), key)==m_ids.end()) {
      // not yet added, do so now!
      gem::record         *handle=NULL;
      startpost("backend #%d='%s'\t", m_handles.size(), key.c_str());
      try {
	handle=gem::PluginFactory<gem::record>::getInstance(key); 
      } catch (GemException ex) {
      }
      if(NULL==handle) { 
	post("<--- DISABLED");
	break;
      }
#if 0
      std::vector<std::string>devs=handle->provides();
      if(devs.size()>0) {
	startpost(": ");
	unsigned int i=0;
	for(i=0; i<devs.size(); i++) {
	  startpost("%s ", devs[i].c_str());
	}
      }
#endif
      endpost();

      m_ids.push_back(key);
      m_handles.push_back(handle);
      count++;
      verbose(2, "added backend#%d '%s' @ 0x%x", m_handles.size()-1, key.c_str(), handle);
    }
  }

  return (count>0);
}


//
// stops recording into the movie
//
void pix_record :: startRecording()
{
  gem::record *handle=NULL;
  // find a handle for the current settings (filename, codec, props)
  const int count=m_handles.size();
  const std::string codec=m_codec;

  if(m_handle) {
    stopRecording();
  }
  if(m_filename.empty()) {
    error("start recording requested with no prior open");
    return;
  }

  if(m_driver<0) {
    int i=0;
    for(i=0; i<count; i++) {
      // check whether the handle supports the requested codec
      handle=m_handles[i];
      if(!handle->setCodec(codec))
	continue;
      if(handle->start(m_filename, m_props)) {
	m_handle=handle;
	post("open successfull...");
      } else {
	handle=NULL;
      }
      
    }
  } else if (m_driver<count) {
    handle=m_handles[m_driver];
    if(!handle->setCodec(codec)) {
      error("requested driver cannot handle codec '%s'", codec.c_str());
      return;
    }
    if(handle->start(m_filename, m_props)) {
      m_handle=handle;
      post("open successfull...");
    } else {
      error("requested driver could not open '%s' for writing..", m_filename.c_str());
      handle=NULL;
    }
  }

  if(m_handle) {
    m_filename=std::string("");
    m_recording=true;
  }
}

//
// stops recording into the movie
//
void pix_record :: stopRecording()
{
  if(m_recording) {
    m_handle->stop();
    m_currentFrame = 0; //reset the frame counter?
    outlet_float(m_outNumFrames,m_currentFrame);
    verbose(1, "movie written");
  }

  m_recording=false;
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
  
  if(m_banged||m_automatic){
#if 0
    m_handle->m_recordStart=m_recordStart;
    m_handle->m_recordStop=m_recordStop;
#endif  

    //      if(m_maxFrames != 0 && m_currentFrame >= m_maxFrames) m_recordStop = 1;
    bool success=m_handle->write(&state->image->image);
    m_banged=false;

    if(success) {
      m_currentFrame++;
      outlet_float(m_outNumFrames,m_currentFrame);
    } else {
      stopRecording();
    }
  }
}

/////////////////////////////////////////////////////////
// Properties
//
/////////////////////////////////////////////////////////
void pix_record :: enumPropertiesMess()
{
  post("no property enumeration yet: %x", m_handle);
  if(m_handle) {
    gem::Properties props;
    if(!m_handle->enumProperties(props))
      return;

    int ac=0;
    t_atom ap[3];
    std::vector<std::string>keys=props.keys();

    SETFLOAT(ap+0, keys.size());
    outlet_anything(m_outInfo, gensym("numprops"), 1, ap);

    int i=0;
    for(i=0; i<keys.size(); i++) {
      ac=2;
      std::string key=keys[i];
      SETSYMBOL(ap+0, gensym(key.c_str()));
      switch(props.type(key)) {
      case gem::Properties::NONE:
        SETSYMBOL(ap+1, gensym("bang"));
        break;
      case gem::Properties::DOUBLE: {
        double d=-1;
        SETSYMBOL(ap+1, gensym("float"));
        /* LATER: get and show ranges */
        if(props.get(key, d)) {
          ac=3;
          SETFLOAT(ap+2, d);
        }
      }
        break;
      case gem::Properties::STRING: {
        SETSYMBOL(ap+1, gensym("symbol"));
        std::string s;
        if(props.get(key, s)) {
          ac=3;
          SETSYMBOL(ap+2, gensym(s.c_str()));
        }
      }
        break;
      default:
        SETSYMBOL(ap+1, gensym("unknown"));
        break;
      }
      outlet_anything(m_outInfo, gensym("property"), ac, ap);
    }
  }
}
void pix_record :: setPropertiesMess(int argc, t_atom*argv)
{
  PIMPL::addProperties(m_props, argc, argv);
}

void pix_record :: clearPropertiesMess()
{
  m_props.clear();
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
// recordMess
//
/////////////////////////////////////////////////////////
void pix_record :: recordMess(bool on)
{
  if (on) {
    startRecording();
  }else{
    stopRecording();
  }
}

/////////////////////////////////////////////////////////
// spits out a list of installed codecs and stores them
//
/////////////////////////////////////////////////////////
void pix_record :: getCodecList()
{
  m_pimpl->clearCodecHandle();
  int i=0;
  for(i=0; i<m_handles.size(); i++) {
    std::vector<std::string>c=m_handles[i]->getCodecs();
    int j;
    for(j=0; j<c.size(); j++) {
      m_pimpl->addCodecHandle(m_handles[i], c[j]);
    }
  }
  for(i=0; i<m_pimpl->m_codecs.size(); i++) {
    const std::string id=m_pimpl->m_codecs[i];
    gem::record*handle=m_pimpl->m_codechandle[id]->handle;

    const std::string codecname=m_pimpl->m_codechandle[id]->codec;
    const std::string descr=handle->getCodecDescription(codecname);
    t_atom ap[3];

    //post("codec%d: '%s': %s", i, codecname.c_str(), (descr.empty()?"":descr.c_str()));
    SETFLOAT (ap+0, static_cast<t_float>(i));
    SETSYMBOL(ap+1, gensym(codecname.c_str()));
    SETSYMBOL(ap+2, gensym(descr.c_str()));
    outlet_anything(m_outInfo, gensym("codec"), 3, ap);
  }
}


/////////////////////////////////////////////////////////
// deals with the name of a codec
//
/////////////////////////////////////////////////////////
void pix_record :: codecMess(t_atom *argv)
{
#warning allow setting of codec without handle
  if(m_handle){
    m_handle->stop();
    m_handle=NULL;
  }
  
  std::string sid;

  if (A_SYMBOL==argv->a_type) {
    sid=std::string(atom_getsymbol(argv)->s_name);
  } else if(A_FLOAT==argv->a_type){
    /* maintain a list of all codecs and resolve using that */
    int id=atom_getint(argv);
    if((id>=0) && (id<m_pimpl->m_codecs.size())) {
      sid=m_pimpl->m_codecs[id];
    }
  }

  if(m_pimpl->m_codechandle[sid]) {
    gem::record*handle=m_pimpl->m_codechandle[sid]->handle;
    std::string codec =m_pimpl->m_codechandle[sid]->codec;
    
    if(handle->setCodec(codec)) {
      m_handle=handle;
      m_codec=codec;
      enumPropertiesMess();
    } else {
      error("unable to set the codec");
    }
  } else {
    post("unknown codec '%s", sid.c_str());
  }
}

void pix_record :: fileMess(int argc, t_atom *argv)
{
  /* LATER let the record()-handles chose whether they accept an open request
   * and then try other handles (if available)
   * this would allow to use this object for streaming, virtual output devices,...
   */
  if(argc) {
    m_filename=std::string(atom_getsymbol(argv)->s_name);
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

  class_addmethod(classPtr, reinterpret_cast<t_method>(&pix_record::dialogMessCallback),
		  gensym("dialog"),  A_NULL);
  class_addmethod(classPtr, reinterpret_cast<t_method>(&pix_record::codeclistMessCallback),
		  gensym("codeclist"),  A_NULL);
  class_addmethod(classPtr, reinterpret_cast<t_method>(&pix_record::codecMessCallback),
		  gensym("codec"), A_GIMME, A_NULL);


  class_addmethod(classPtr, 
		  reinterpret_cast<t_method>(&pix_record::enumPropertiesMessCallback),
		  gensym("proplist"), A_NULL);
  class_addmethod(classPtr, 
		  reinterpret_cast<t_method>(&pix_record::setPropertiesMessCallback),
		  gensym("set"), A_GIMME, A_NULL);
  class_addmethod(classPtr, 
		  reinterpret_cast<t_method>(&pix_record::clearPropertiesMessCallback),
		  gensym("clearprops"), A_NULL);


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


void pix_record :: enumPropertiesMessCallback(void *data)
{
  GetMyClass(data)->enumPropertiesMess();
}
void pix_record :: setPropertiesMessCallback(void *data, t_symbol *s, int argc, t_atom *argv)
{
  GetMyClass(data)->setPropertiesMess(argc, argv);
}
void pix_record :: clearPropertiesMessCallback(void *data)
{
  GetMyClass(data)->clearPropertiesMess();
}
