/*
 *  pix_record.cpp
 *
 */
#include "Gem/GemConfig.h"
#include "pix_record.h"

#include "Gem/State.h"
#include "Gem/Exception.h"

#include "plugins/PluginFactory.h"

#include <map>
#include <algorithm>

CPPEXTERN_NEW_WITH_GIMME(pix_record);

struct pix_record :: PIMPL
{
  CPPExtern*parent;
  PIMPL(CPPExtern*_parent)
    :parent(_parent)
  {};
  ~PIMPL(void) {};

  static gem::any atom2any(t_atom*ap)
  {
    gem::any result;
    if(ap) {
      switch(ap->a_type) {
      case A_FLOAT:
        result=atom_getfloat(ap);
        break;
      case A_SYMBOL:
        result=std::string(atom_getsymbol(ap)->s_name);
        break;
      default:
        result=ap->a_w.w_gpointer;
      }
    }
    return result;
  }
  void addProperties(gem::Properties&props, int argc, t_atom*argv)
  {
    if(!argc) {
      return;
    }

    if(argv->a_type != A_SYMBOL) {
      pd_error(parent, "no key given...");
      return;
    }
    std::string key=std::string(atom_getsymbol(argv)->s_name);
    std::vector<gem::any> values;
    argc--;
    argv++;
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
    case 0: {
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
pix_record :: pix_record(int argc, t_atom *argv) :
  m_banged(false), m_automatic(true),
  m_framesOut(gem::RTE::Outlet(this)), m_infoOut(gem::RTE::Outlet(this)),
  m_currentFrame(-1),
  m_maxFrames(0),
  m_recording(false),
  m_handle(NULL),
  m_pimpl(new PIMPL(this))
{
  if (argc != 0) {
    error("ignoring arguments");
  }

  m_handle=gem::plugins::record::getInstance();
  getCodecList();
}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
pix_record :: ~pix_record()
{
  if(m_handle) {
    delete m_handle;
  }
  if(m_pimpl) {
    delete m_pimpl;
  }
}

//
// stops recording into the movie
//
void pix_record :: startRecording()
{
  if(!m_handle) {
    return;
  }

  if(m_filename.empty()) {
    error("start recording requested with no prior open");
    return;
  }

  // find a handle for the current settings (filename, codec, props)
  /* const std::string codec=m_codec; */
  stopRecording();

  m_currentFrame = 0;
  // do not re-set the codec, if there is no need...
  /* m_handle->setCodec(codec); */
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
  if(!m_handle) {
    return;
  }

  if(m_recording) {
    m_handle->stop();
    m_currentFrame = 0;
    m_framesOut.send(m_currentFrame);
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
  if(!m_handle || !m_recording) {
    return;
  }

  //check if state exists
  if(!state) {
    return;
  }
  pixBlock*img=NULL;
  state->get(GemState::_PIX, img);

  if(!img || !img->image.data) {
    return;
  }

  if(m_banged||m_automatic) {
    //      if(m_maxFrames != 0 && m_currentFrame >= m_maxFrames) m_recordStop = 1;
    bool success=m_handle->write(&img->image);
    m_banged=false;

    if(success) {
      m_currentFrame++;
      m_framesOut.send(m_currentFrame);
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
  if(!m_handle) {
    return;
  }

  gem::Properties props;
  if(!m_handle->enumProperties(props)) {
    return;
  }

  std::vector<gem::any>data;
  gem::any value;
  std::vector<std::string>keys=props.keys();
  data.clear();
  data.push_back(value=(int)keys.size());
  m_infoOut.send("numprops", data);


  for(unsigned int i=0; i<keys.size(); i++) {
    int ac=2;
    std::string key=keys[i];
    data.clear();
    data.push_back(value=key);
    switch(props.type(key)) {
    case gem::Properties::NONE:
      data.push_back(value=std::string("Bang"));
      break;
    case gem::Properties::DOUBLE: {
      double d=-1;
      data.push_back(value=std::string("Float"));
      /* LATER: get and show ranges */
      if(props.get(key, d)) {
        data.push_back(value=d);
      }
    }
    break;
    case gem::Properties::STRING: {
      data.push_back(value=std::string("Symbol"));
      std::string s;
      if(props.get(key, s)) {
        data.push_back(value=s);
      }
    }
    break;
    default:
      data.push_back(value=std::string("unknown"));
      break;
    }
    m_infoOut.send("property", data);
  }
}
void pix_record :: setPropertiesMess(t_symbol*s, int argc, t_atom*argv)
{
  m_pimpl->addProperties(m_props, argc, argv);
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
  if(!m_handle) {
    return;
  }

  if(!m_handle->dialog()) {
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
  } else {
    stopRecording();
  }
}

/////////////////////////////////////////////////////////
// spits out a list of installed codecs and stores them
//
/////////////////////////////////////////////////////////
void pix_record :: getCodecList()
{
  if(!m_handle) {
    return;
  }

  std::vector<std::string>codecs=m_handle->getCodecs();

  for(unsigned int i=0; i<codecs.size(); i++) {
    const std::string codecname=codecs[i];
    const std::string descr=m_handle->getCodecDescription(codecname);
    std::vector<gem::any>data;
    gem::any value;
    t_atom ap[3];

    verbose(2, "codec%d: '%s': %s", i, codecname.c_str(),
            (descr.empty()?"":descr.c_str()));
    data.push_back(value=i);
    data.push_back(value=codecname);
    data.push_back(value=descr);
    m_infoOut.send("codec", data);
  }
}


/////////////////////////////////////////////////////////
// deals with the name of a codec
//
/////////////////////////////////////////////////////////
void pix_record :: codecMess(t_atom *argv)
{
  if(!m_handle) {
    return;
  }

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
    if(id>0 && ((unsigned int)id)<codecs.size()) {
      sid=codecs[id];
    } else {
      error("invalid codec# %d (0..%d)", id, codecs.size());
      return;
    }
  }
  if(m_handle->setCodec(sid)) {
    m_codec=sid;
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
   * this would allow us to use this object for streaming, virtual output devices,...
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
  CPPEXTERN_MSG0(classPtr, "codeclist", getCodecList);
  class_addmethod(classPtr,
                  reinterpret_cast<t_method>(&pix_record::codecMessCallback),
                  gensym("codec"), A_GIMME, A_NULL);

  CPPEXTERN_MSG0(classPtr, "dialog", dialogMess);
  CPPEXTERN_MSG0(classPtr, "proplist", enumPropertiesMess);
  CPPEXTERN_MSG0(classPtr, "enumProps", enumPropertiesMess);
  CPPEXTERN_MSG (classPtr, "set", setPropertiesMess);

  CPPEXTERN_MSG0(classPtr, "clearProps", clearPropertiesMess);
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

void pix_record :: codecMessCallback(void *data, t_symbol* s, int argc,
                                     t_atom *argv)
{
  if(argc) {
    GetMyClass(data)->codecMess(argv);
  }
}
