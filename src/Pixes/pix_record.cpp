/*
 *  pix_record.cpp
 *
 */
#include "Gem/GemConfig.h"
#include "pix_record.h"

#include "Gem/State.h"
#include "Gem/Exception.h"

#include "RTE/MessageCallbacks.h"
#include "plugins/PluginFactory.h"

#include <map>

CPPEXTERN_NEW_WITH_GIMME(pix_record);

class pix_record :: PIMPL {
public:
  PIMPL(void) {};
  ~PIMPL(void) {};

  struct codechandle {
    codechandle(gem::plugins::record*h, const std::string c):handle(h), codec(c) {}
    
    gem::plugins::record*handle;
    std::string codec;
  };
  std::map<std::string, std::vector<codechandle> >m_codechandle;
  std::vector<std::string>m_codecs;

  void addCodecHandle(gem::plugins::record*handle, const std::string codec) {
#ifdef __GNUC__
# warning better handling of duplicate codecs
#endif
    /* FIXME: we should generate a unique codec-ID, e.g. "<handlename>:<codec>" */
    m_codechandle[codec].push_back(codechandle(handle, codec));
    m_codecs.push_back(codec);
  }
  void clearCodecHandle(void) {
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
  m_outNumFrames(NULL), m_outInfo(NULL),
  m_currentFrame(-1),
  m_maxFrames(0),
  m_recording(false),
  m_handle(NULL),
  m_pimpl(new PIMPL())
{
  if (argc != 0){
    error("ignoring arugments");
  }
  m_outNumFrames = outlet_new(this->x_obj, 0);
  m_outInfo      = outlet_new(this->x_obj, 0);


  m_handle=gem::plugins::record::getInstance();
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
      gem::plugins::record         *handle=NULL;
      startpost("backend #%d='%s'\t", m_allhandles.size(), key.c_str());
      try {
        handle=gem::PluginFactory<gem::plugins::record>::getInstance(key); 
      } catch (GemException&ex) {
        startpost("(%s) ", ex.what());
        handle=NULL;
      }
      if(NULL==handle) { 
        post("<--- DISABLED");
        break;
      }
      endpost();

      m_ids.push_back(key);
      m_allhandles.push_back(handle);
      count++;
      verbose(2, "added backend#%d '%s' @ 0x%x", m_allhandles.size()-1, key.c_str(), handle);
    }
  }

  return (count>0);
}

//
// stops recording into the movie
//
void pix_record :: startRecording()
{
  if(!m_handle)return;

  if(m_filename.empty()) {
    error("start recording requested with no prior open");
    return;
  }

  // find a handle for the current settings (filename, codec, props)
  const std::string codec=m_codec;
  stopRecording();


  m_currentFrame = 0;
  unsigned int i=0;
  m_handle->setCodec(codec);
  if(m_handle->start(m_filename, m_props)) {
    m_filename=std::string("");
    m_recording=true;
  } else {
    post("unable to open '%s'", m_filename.c_str());
  }
}

//
// stops recording into the movie
//
void pix_record :: stopRecording()
{
  if(!m_handle)return;

  if(m_recording) {
    m_handle->stop();
    m_currentFrame = 0;
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
  if(!m_handle)return;

  //check if state exists
  if(!state)return;
  pixBlock*img=NULL;
  state->get(GemState::_PIX, img);

  if(!img || !img->image.data){
    return;
  }
  
  if(m_banged||m_automatic){
    //      if(m_maxFrames != 0 && m_currentFrame >= m_maxFrames) m_recordStop = 1;
    bool success=m_handle->write(&img->image);
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
  if(!m_handle)return;

  gem::Properties props;
  if(!m_handle->enumProperties(props))
    return;

  int ac=0;
  t_atom ap[3];
  std::vector<std::string>keys=props.keys();

  SETFLOAT(ap+0, keys.size());
  outlet_anything(m_outInfo, gensym("numprops"), 1, ap);

  unsigned int i=0;
  for(i=0; i<keys.size(); i++) {
    ac=2;
    std::string key=keys[i];
    SETSYMBOL(ap+0, gensym(key.c_str()));
    switch(props.type(key)) {
    case gem::Properties::NONE:
      SETSYMBOL(ap+1, gensym("Bang"));
      break;
    case gem::Properties::DOUBLE: {
      double d=-1;
      SETSYMBOL(ap+1, gensym("Float"));
      /* LATER: get and show ranges */
      if(props.get(key, d)) {
	ac=3;
	SETFLOAT(ap+2, d);
      }
    }
      break;
    case gem::Properties::STRING: {
      SETSYMBOL(ap+1, gensym("Symbol"));
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
void pix_record :: setPropertiesMess(t_symbol*s, int argc, t_atom*argv)
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
  if(!m_handle)return;

  if(!m_handle->dialog()){
    error("unable to open settings dialog");
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
  if(!m_handle)return;

  std::vector<std::string>codecs=m_handle->getCodecs();

  unsigned int i;
  for(i=0; i<codecs.size(); i++) {
    const std::string codecname=codecs[i];
    const std::string descr=m_handle->getCodecDescription(codecname);
    t_atom ap[3];

    verbose(2, "codec%d: '%s': %s", i, codecname.c_str(), (descr.empty()?"":descr.c_str()));
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
  if(!m_handle)return;

#ifdef __GNUC__
#warning codecMess is a mess
#endif
  /*
   * allow setting of codec without handle
   */

  /*
   * codecMess should do the following:
   *  find "valid" handles (those that support the given codec)
   *  copy all valid handles from m_allhandles to m_handles
   *  query all valid handles for settable properties
   *
   * if a special codec is given (e.g. none at all), all handles are valid
   */

  std::string sid;

  if (A_SYMBOL==argv->a_type) {
    sid=std::string(atom_getsymbol(argv)->s_name);
  } else if (A_FLOAT==argv->a_type) {
    int id=atom_getint(argv);
    std::vector<std::string>codecs=m_handle->getCodecs();
    if(id>0 && id<codecs.size())
      sid=codecs[id];
    else {
      error("invalid codec# %d (0..%d)", id, codecs.size());
      return;
    }
  }
  if(m_handle->setCodec(sid)) {
    verbose(1, "successfully set codec '%s'", sid.c_str());
  } else {
    error("couldn't find a valid backend for codec '%s'", sid.c_str());
    return;
  }
  enumPropertiesMess();
}

void pix_record :: fileMess(t_symbol*s, int argc, t_atom *argv)
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
  CPPEXTERN_MSG (classPtr, "file", fileMess);

  CPPEXTERN_MSG1(classPtr, "auto", autoMess, bool);
  CPPEXTERN_MSG0(classPtr, "bang", bangMess);
  CPPEXTERN_MSG1(classPtr, "record", recordMess, bool);
  CPPEXTERN_MSG0(classPtr, "dialog", dialogMess);
  CPPEXTERN_MSG0(classPtr, "codeclist", getCodecList);
  class_addmethod(classPtr, reinterpret_cast<t_method>(&pix_record::codecMessCallback),
		  gensym("codec"), A_GIMME, A_NULL);

  CPPEXTERN_MSG0(classPtr, "proplist", enumPropertiesMess);
  CPPEXTERN_MSG (classPtr, "set", fileMess);

  CPPEXTERN_MSG0(classPtr, "clearprops", clearPropertiesMess);
}

void pix_record :: bangMess(void)
{
  m_banged=true;
}
void pix_record :: autoMess(bool on)
{
  m_automatic=on;
}

void pix_record :: codecMessCallback(void *data, t_symbol *s, int argc, t_atom *argv)
{
  if(argc)
    GetMyClass(data)->codecMess(argv);
}
