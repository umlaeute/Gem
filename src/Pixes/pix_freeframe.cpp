////////////////////////////////////////////////////////
//
// GEM - Graphics Environment for Multimedia
//
// zmoelnig@iem.at
//
// Implementation file
//
//    Copyright (c) 1997-2000 Mark Danks.
//    Copyright (c) 2001-2011 IOhannes m zmölnig. forum::für::umläute. IEM. zmoelnig@iem.at
//    Copyright (c) 2002 James Tittle & Chris Clepper
//    For information on usage and redistribution, and for a DISCLAIMER OF ALL
//    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.
//
// this is based on EffecTV by Fukuchi Kentarou
// * Copyright (C) 2001 FUKUCHI Kentarou
//
/////////////////////////////////////////////////////////

#include <iostream>

#include "pix_freeframe.h"
#include "Gem/Exception.h"
#include "Gem/Loaders.h"

#include "Gem/Properties.h"

#ifndef DONT_WANT_FREEFRAME

#include <stdio.h>
#ifdef _WIN32
# include <io.h>
# include <windows.h>
# define snprintf _snprintf
# define close _close
#else /* !_WIN32 */
# define DL_OPEN 1
#endif /* _WIN32 */

/*
 * Apple used to use CFBundle's to load FF plugins
 * currently this only crashes (on OSX-10.4 and OSX-10.5)
 * we therefore use dlopen() on OSX as well
 */
#if defined __APPLE__ && 0
# define FREEFRAME_USE_BUNDLELOADER
# include <CoreFoundation/CoreFoundation.h>
#endif /* __APPLE__ */

#ifdef DL_OPEN
# include <dlfcn.h>
# include <unistd.h>
#endif /* DL_OPEN */

#include <string.h>


#ifndef HAVE_STRNLEN
#define strnlen ff_strnlen
static size_t ff_strnlen(const char* str, size_t maxlen)
{
  size_t len=0;
  if(NULL==str) {
    return len;
  }
  while(*str++ && len<maxlen) {
    len++;
  }

  return len;
}
#endif


class pix_freeframe::FFPlugin
{
public:
  static std::string nchar2str(const char*str, const unsigned int len)
  {
    std::string result;
    size_t l=strnlen(str, len);
    result.assign(str, l);
    return result;
  }
  static std::string nchar2str(void*str, const unsigned int len)
  {
    return nchar2str(reinterpret_cast<const char*>(str), len);
  }

private:
  class FFInstance
  {
    FFInstanceID    m_instance;
    FF_Main_FuncPtr m_plugin;
    static inline FFUInt32 csize2depth(unsigned int csize)
    {
      switch(csize) {
      case(4):
        return FF_CAP_32BITVIDEO;
      case(3):
        return FF_CAP_24BITVIDEO;
      }
      return 0;
    }
    static inline FFUInt32 updown2orientation(bool updown)
    {
      return (updown?1:2);
    }

    unsigned int m_width, m_height, m_depth, m_orientation;

    void create(VideoInfoStruct&vis)
    {
      if(!m_plugin) {
        throw(GemException("no plugin loaded"));
      }
      FFMixed input;
      input.PointerValue = &vis;
      FFMixed result = m_plugin(FF_INSTANTIATE, input, m_instance);
      if(FF_FAIL==result.UIntValue) {
        throw(GemException("couldn't instaniate"));
      }

      m_instance = result.PointerValue;

      if(NULL==m_instance) {
        throw(GemException("could not instaniate"));
      }

      m_width =vis.FrameWidth;
      m_height=vis.FrameHeight;
      m_orientation=vis.Orientation;
      m_depth =vis.BitDepth;

    }

  public:
    FFInstance(FF_Main_FuncPtr plugin, VideoInfoStruct&vis)
      : m_instance(NULL)
      , m_plugin(plugin)
    {
      create(vis);
    }
    FFInstance(FF_Main_FuncPtr plugin, imageStruct&image)
      : m_instance(NULL)
      , m_plugin(plugin)
    {
      VideoInfoStruct vis;
      vis.FrameWidth =image.xsize;
      vis.FrameHeight=image.ysize;
      vis.BitDepth   = csize2depth(image.csize);
      if(0==vis.BitDepth) {
        throw(GemException("unsupported colorspace"));
      }
      vis.Orientation=updown2orientation(image.upsidedown);
      create(vis);
    }
    virtual ~FFInstance(void)
    {
      call(FF_DEINSTANTIATE);
    }

    FFMixed call(FFUInt32 funcode, FFMixed value)
    {
      FFMixed result;
      result=m_plugin(funcode, value, m_instance);
      return result;
    }
    FFMixed call(FFUInt32 funcode, FFUInt32 value)
    {
      FFMixed mixed;
      mixed.UIntValue=value;
      return call(funcode, mixed);
    }
    FFMixed call(FFUInt32 funcode, FFFloat32 value)
    {
      FFMixed mixed;
      mixed.FloatValue=value;
      return call(funcode, mixed);
    }
    FFMixed call(FFUInt32 funcode, void* value)
    {
      FFMixed mixed;
      mixed.PointerValue=value;
      return call(funcode, mixed);
    }
    FFMixed call(FFUInt32 funcode)
    {
      FFMixed mixed;
      mixed.PointerValue=0;
      return call(funcode, mixed);
    }

    bool checkDimen(const imageStruct&img)
    {
      return (
               (img.xsize == m_width ) &&
               (img.ysize == m_height) &&
               (csize2depth       (img.csize     ) == m_depth ) &&
               (updown2orientation(img.upsidedown) == m_orientation)
             );
    }



  };

  std::string m_name;
  FF_Main_FuncPtr m_plugin;
  FFInstance     *m_instance;

  std::string m_id;

  std::string m_description;
  std::string m_about;

  gem::Properties m_parameter;
  std::vector<std::string>m_parameterNames;
  bool m_rgba;
  unsigned int m_type;
  unsigned int m_majorVersion, m_minorVersion;
  bool m_cancopy;

#ifdef DL_OPEN
  void           *m_dlhandle;
#endif
#ifdef _WIN32
  HINSTANCE       m_w32handle;
#endif

  FFMixed callInstance(FFUInt32 funcode, FFMixed value)
  {
    if(!m_instance) {
      FFMixed result;
      result.UIntValue=FF_FAIL;
      return result;
    }
    return m_instance->call(funcode, value);
  }
  FFMixed callInstance(FFUInt32 funcode, FFUInt32 value)
  {
    if(!m_instance) {
      FFMixed result;
      result.UIntValue=FF_FAIL;
      return result;
    }
    return m_instance->call(funcode, value);
  }
  FFMixed callInstance(FFUInt32 funcode, FFFloat32 value)
  {
    if(!m_instance) {
      FFMixed result;
      result.UIntValue=FF_FAIL;
      return result;
    }
    return m_instance->call(funcode, value);
  }
  FFMixed callInstance(FFUInt32 funcode, void* value)
  {
    if(!m_instance) {
      FFMixed result;
      result.UIntValue=FF_FAIL;
      return result;
    }
    return m_instance->call(funcode, value);
  }
  FFMixed callInstance(FFUInt32 funcode)
  {
    if(!m_instance) {
      FFMixed result;
      result.UIntValue=FF_FAIL;
      return result;
    }
    return m_instance->call(funcode);
  }

  FFMixed call(FFUInt32 funcode, FFMixed value, FFInstanceID id)
  {
    return m_plugin(funcode, value, id);
  }

  FFMixed callGlobal(FFUInt32 funcode, FFMixed value)
  {
    return call(funcode, value, NULL);
  }
  FFMixed callGlobal(FFUInt32 funcode, FFUInt32 value)
  {
    FFMixed mixed;
    mixed.UIntValue=value;
    return callGlobal(funcode, mixed);
  }
  FFMixed callGlobal(FFUInt32 funcode, FFFloat32 value)
  {
    FFMixed mixed;
    mixed.FloatValue=value;
    return callGlobal(funcode, mixed);
  }
  FFMixed callGlobal(FFUInt32 funcode, void* value)
  {
    FFMixed mixed;
    mixed.PointerValue=value;
    return callGlobal(funcode, mixed);
  }
  FFMixed callGlobal(FFUInt32 funcode)
  {
    FFMixed invalue;
    invalue.PointerValue=0;
    return call(funcode, invalue, NULL);
  }

  void close(void)
  {
    if(NULL==m_plugin) {
      return;
    }

    if(m_instance) {
      deinstantiate_();
    }

    deinitialize_();

#ifdef DL_OPEN
    if(m_dlhandle) {
      dlclose(m_dlhandle);
    }
    m_dlhandle=NULL;
#endif
#ifdef _WIN32
    if(m_w32handle) {
      FreeLibrary(m_w32handle);
    }
    m_w32handle=NULL;
#endif
  }

  bool open(std::string name, const t_canvas*canvas)
  {
    bool loud=false;
    const char*hookname="plugMain";
    if(name.empty()) {
      return false;
    }

    if(m_plugin) {
      close();
    }

    FF_Main_FuncPtr plugmain = NULL;

    char buf[MAXPDSTRING];
    char buf2[MAXPDSTRING];
    char *bufptr=NULL;

    const char *extension=
#ifdef _WIN32
      ".dll";
#elif defined __APPLE__
      "";
#else
      ".so";
#endif

#ifdef __APPLE__
    char buf3[MAXPDSTRING];
# ifdef DL_OPEN
    snprintf(buf3, MAXPDSTRING, "%s.frf/Contents/MacOS/%s", name.c_str(),
             name.c_str());
# else
    // this can never work...
    snprintf(buf3, MAXPDSTRING, "%s.frf/%s", name.c_str(), name.c_str());
# endif
    buf3[MAXPDSTRING-1]=0;
    name=buf3;
#endif

    int fd=-1;
    if ((fd=canvas_open(const_cast<t_canvas*>(canvas), name.c_str(), extension,
                        buf2, &bufptr, MAXPDSTRING, 1))>=0) {
      ::close(fd);
#if defined FREEFRAME_USE_BUNDLELOADER && 0
      snprintf(buf, MAXPDSTRING, "%s", buf2);
#else
      snprintf(buf, MAXPDSTRING, "%s/%s", buf2, bufptr);
#endif
      buf[MAXPDSTRING-1]=0;
    } else {
      if(canvas) {
        canvas_makefilename(const_cast<t_canvas*>(canvas),
                            const_cast<char*>(name.c_str()), buf, MAXPDSTRING);
      } else {
        if(loud) {
          ::pd_error(0, "pix_freeframe[%s]: unfindeable", name.c_str());
        }
        return false;
      }
    }
    name=buf;
    std::string libname = name;

    if(loud) {
      ::post("trying to load %s", buf);
    }

#ifdef DL_OPEN
    if(!plugmain) {
      if(loud) {
        ::post("dlopen %s", libname.c_str());
      }
      m_dlhandle=dlopen(libname.c_str(), RTLD_NOW);
      if(!m_dlhandle) {
        if(loud) {
          ::pd_error(0, "pix_freeframe[%s]: %s", libname.c_str(), dlerror());
        }
      } else {
        dlerror();
        plugmain = reinterpret_cast<FF_Main_FuncPtr>(dlsym(m_dlhandle, hookname));
      }
    }
#endif
#if defined FREEFRAME_USE_BUNDLELOADER
    if(!plugmain) {
      CFURLRef bundleURL = NULL;
      CFBundleRef theBundle = NULL;
      CFStringRef plugin = CFStringCreateWithCString(NULL,
                                                     libname.c_str(), kCFStringEncodingMacRoman);

      bundleURL = CFURLCreateWithFileSystemPath( kCFAllocatorSystemDefault,
                                                 plugin,
                                                 kCFURLPOSIXPathStyle,
                                                 true );
      theBundle = CFBundleCreate( kCFAllocatorSystemDefault, bundleURL );

      // Get a pointer to the function.
      if (theBundle) {
        plugmain = reinterpret_cast<FF_Main_FuncPtr>
          (CFBundleGetFunctionPointerForName(
                                             theBundle, CFSTR("plugMain") )
           );
      } else {
        if(loud) {
          ::post("%s: couldn't load", libname.c_str());
        }
      }
      if(bundleURL != NULL) {
        CFRelease( bundleURL );
      }
      if(theBundle != NULL) {
        CFRelease( theBundle );
      }
      if(plugin != NULL) {
        CFRelease( plugin );
      }
    }
#endif
#if defined _WIN32
    if(!plugmain) {
      char buffer[MAXPDSTRING];
      sys_bashfilename(libname.c_str(), buffer);
      libname=buffer;
      m_w32handle = LoadLibrary(libname.c_str());
      if (!m_w32handle) {
        if(loud) {
          ::post("%s: couldn't load", libname.c_str());
        }
      } else {
        plugmain = reinterpret_cast<FF_Main_FuncPtr>(GetProcAddress(m_w32handle,
                   hookname));
      }
    }
#endif

    m_plugin=plugmain;
    return (NULL!=m_plugin);
  }


  /* GLOBAL
      0  getInfo         none    Pointer to a PluginInfoStruct
      1  initialise      none    Success/error code
      2  deInitialise    none    Success/error code
      4  getNumParameters        none    NumParameters
      5  getParameterName        ParameterNumber         Pointer to ParameterName
      6  getParameterDefault     ParameterNumber         ParameterDefaultValue
      10         getPluginCaps   PluginCapsIndex         Supported/unsupported/value
      13         getExtendedInfo         none    Pointer to PluginExtendedInfoStruct
      15         getParameterType        ParameterNumber         ParameterType
   */

  bool getInfo_(void)
  {
    FFMixed result=callGlobal(FF_GETINFO);
    if(FF_FAIL==result.UIntValue) {
      std::cerr << "getInfo failed" << std::endl;
      return false;
    }
    PluginInfoStruct*pis=reinterpret_cast<PluginInfoStruct*>
                         (result.PointerValue);
    m_name = nchar2str(pis->PluginName, 16);
    m_id = nchar2str(pis->PluginUniqueID, 4);
    m_type = pis->PluginType;

    std::cerr <<
              "FF-API: "<<pis->APIMajorVersion<<"."<<pis->APIMinorVersion<<std::endl;

    return true;
  }
  PluginExtendedInfoStruct*getExtendedInfo_(void)
  {
    FFMixed result=callGlobal(FF_GETEXTENDEDINFO);
    m_description=m_about="";
    m_majorVersion = m_minorVersion = 0;
    if(FF_FAIL==result.UIntValue) {
      return NULL;
    }
    PluginExtendedInfoStruct*pis=reinterpret_cast<PluginExtendedInfoStruct*>
                                 (result.PointerValue);
    m_description=pis->Description;
    m_about = pis->About;
    m_majorVersion = pis -> PluginMajorVersion;
    m_minorVersion = pis -> PluginMinorVersion;
    return pis;
  }

  bool initialize_(void)
  {
    FFMixed result=callGlobal(FF_INITIALISE);
    return (FF_SUCCESS==result.UIntValue);
  }
  bool deinitialize_(void)
  {
    FFMixed result=callGlobal(FF_DEINITIALISE);
    m_plugin=NULL;

    m_name=m_id=m_description=m_about="";

    return (FF_SUCCESS==result.UIntValue);
  }
  FFUInt32 getNumParameters_(void)
  {
    FFMixed result=callGlobal(FF_GETNUMPARAMETERS);
    if(FF_FAIL == result.UIntValue) {
      return 0;
    }
    return result.UIntValue;
  }
  std::string getParameterName_(FFUInt32 ParameterNumber)
  {
    FFMixed result=callGlobal(FF_GETPARAMETERNAME, ParameterNumber);
    std::string name = nchar2str(result.PointerValue, 16);
    return name;
  }
  FFMixed getParameterDefault_(FFUInt32 ParameterNumber)
  {
    FFMixed result=callGlobal(FF_GETPARAMETERDEFAULT, ParameterNumber);
    return result;
  }
  FFUInt32 getParameterType_(FFUInt32 ParameterNumber)
  {
    FFMixed result=callGlobal(FF_GETPARAMETERTYPE, ParameterNumber);
    return result.UIntValue;
  }


  FFUInt32 getPluginCaps_(FFUInt32 PluginCapsIndex)
  {
    FFMixed result=callGlobal(FF_GETPLUGINCAPS, PluginCapsIndex);
    return result.UIntValue;
  }

  /* INSTANCE SPECIFIC
    3   processFrame    Pointer to a frame of video     Success/error code
    7   getParameterDisplay     ParameterNumber         Pointer to ParameterDisplayValue
    8   setParameter    Pointer to SetParameterStruct   Success/error code
    9   getParameter    ParameterNumber         ParameterValue
    11  instantiate     Pointer to VideoInfoStruct      InstanceIdentifier
    12  deInstantiate   none    Success/error code
    14  processFrameCopy        Pointer to ProcessFrameCopyStruct       Success/error code
    16  getInputStatus  InputChannel    InputStatus
  */
  bool processFrame_(imageStruct&img)
  {
    // return true;
    FFMixed input;
    input.PointerValue = img.data;
    FFMixed result = callInstance(FF_PROCESSFRAME, input);
    return (FF_SUCCESS == result.UIntValue);
  }
  std::string getParameterDisplay_(FFUInt32 ParameterNumber)
  {
    std::string name;
    FFMixed result = callInstance(FF_GETPARAMETERDISPLAY, ParameterNumber);
    if(result.UIntValue != FF_FAIL) {
      name=nchar2str(result.PointerValue, 16);
    }

    return name;
  }
  bool setParameter_(FFUInt32 ParameterNumber, FFMixed ParameterValue)
  {
    SetParameterStruct sps = { ParameterNumber, ParameterValue};
    FFMixed input;
    input.PointerValue = &sps;
    FFMixed result = callInstance(FF_SETPARAMETER, input);

    return (FF_SUCCESS == result.UIntValue);
  }
  FFMixed getParameter_(FFUInt32 ParameterNumber)
  {
    FFMixed result = callInstance(FF_GETPARAMETER, ParameterNumber);
    return result;
  }
  bool instantiate_(VideoInfoStruct&vis)
  {
    if(m_instance) {
      deinstantiate_();
    }

    try {
      m_instance = new FFInstance(m_plugin, vis);
    } catch (GemException&x) {
      x.report("pix_freeframe");
      m_instance=NULL;
      return false;
    }

    return true;
  }
  bool instantiate_(imageStruct&img)
  {
    if(m_instance) {
      deinstantiate_();
    }

    try {
      m_instance = new FFInstance(m_plugin, img);

    } catch (GemException&x) {
      x.report("pix_freeframe");
      m_instance=NULL;
      return false;
    }

    return true;
  }
  bool deinstantiate_(void)
  {
    if(m_instance) {
      delete m_instance;
    }
    m_instance=NULL;
    return true;
  }
  bool processFrameCopy_(ProcessFrameCopyStruct&pfcs)
  {
    return false;
  }
  bool getInputStatus_(FFUInt32 InputChannel)
  {
    FFMixed result = callInstance(FF_GETINPUTSTATUS, InputChannel);
    return (0!=result.UIntValue);
  }

  bool initParameters_(void)
  {
    m_parameterNames.clear();
    m_parameter.clear();
    unsigned int count=getNumParameters_();
    unsigned int i;

    m_parameterNames.push_back(""); // dummy parameter
    for(i=0; i<count; i++) {
      std::string name=getParameterName_(i);
      FFUInt32 type = getParameterType_ (i);
      FFMixed def = getParameterDefault_(i);

      gem::any val;
      switch(type) {
      case FF_TYPE_EVENT:
        //?
        break;
      case FF_TYPE_TEXT:
        val = std::string(reinterpret_cast<const char*>(def.PointerValue));
        break;
      default:
        val = def.FloatValue;
      }
      std::cerr << "param#"<<i<<": "<<name<<std::endl;
      m_parameterNames.push_back(name);
      m_parameter.set(name, val);
    }
    return true;
  }

  bool init_(void)
  {
    if(!initialize_()) {
      ::pd_error(0, "pix_freeframe[%s]: does not look like a FreeFrame plugin", m_name.c_str());
      return false;
    }

    bool rgb = (FF_SUPPORTED==getPluginCaps_( FF_CAP_24BITVIDEO ));
    bool rgba= (FF_SUPPORTED==getPluginCaps_( FF_CAP_32BITVIDEO ));
    if(rgb || rgba) {
      m_rgba=rgba;
    } else {
      ::pd_error(0, "pix_freeframe[%s]: unsupported plugin type", m_name.c_str());
      return false;
    }
    m_cancopy =  (FF_SUPPORTED==getPluginCaps_( FF_CAP_PROCESSFRAMECOPY ));

    getInfo_();
    getExtendedInfo_();

    initParameters_();
    return true;
  }


public:
  FFPlugin(const std::string&name, const t_canvas*canvas=NULL)
    : m_name(name)
    , m_plugin(NULL)
    , m_instance(NULL)
    , m_rgba(false)
    , m_type(FF_EFFECT)
    , m_majorVersion(0)
    , m_minorVersion(0)
    , m_cancopy(false)
#ifdef DL_OPEN
    , m_dlhandle(NULL)
#endif
#ifdef _WIN32
    , m_w32handle(NULL)
#endif
  {
    if(!open(name, canvas)) {
      throw(GemException(std::string("unable to open '"+name+"'")));
    }

    if(!init_()) {
      throw(GemException(std::string("unable to initialize '"+name+"'")));
    }
  }
  virtual ~FFPlugin(void)
  {
    close();
  }

  unsigned int GLformat()
  {
    unsigned int format = (m_rgba?GEM_RGBA:GEM_RGB);
    return format;
  }

  FFUInt32 getNumParameters(void)
  {
    //    return getNumParameters_();
    return m_parameterNames.size();
  }
  std::string getParameterName(FFUInt32 ParameterNumber)
  {
    if(ParameterNumber<m_parameterNames.size()) {
      return m_parameterNames[ParameterNumber];
    }
    return std::string();
    //    return getParameterName_(ParameterNumber);
  }
  FFMixed getParameterDefault(FFUInt32 ParameterNumber)
  {
    return getParameterDefault_(ParameterNumber);
  }
  FFUInt32 getParameterType(FFUInt32 ParameterNumber)
  {
    return getParameterType_(ParameterNumber);
  }


  FFUInt32 getPluginCaps(FFUInt32 PluginCapsIndex)
  {
    return getPluginCaps_(PluginCapsIndex);
  }

  bool processFrame(imageStruct&img)
  {
    if(NULL==img.data) {
      return true;
    }
    if(!m_instance) {
      instantiate_(img);
    }
    if(m_instance) {
      if(!m_instance->checkDimen(img)) {
        gem::Properties parms=m_parameter;
        deinstantiate_();
        initParameters_();
        instantiate_(img);
        setParameters(parms);
      }
      if(m_instance) {
        return processFrame_(img);
      }
    }
    return false;
  }
  std::string getParameterDisplay(FFUInt32 ParameterNumber)
  {
    return getParameterDisplay_(ParameterNumber);
  }
  bool setParameter(FFUInt32 ParameterNumber, FFMixed ParameterValue)
  {
    return setParameter_(ParameterNumber, ParameterValue);
  }
  FFMixed getParameter(FFUInt32 ParameterNumber)
  {
    return getParameter_( ParameterNumber);
  }

  const gem::Properties&getParameters(void)
  {
    return m_parameter;
  }
  bool setParameter(FFUInt32 ParameterNumber)
  {
    FFMixed value;
    value.PointerValue = 0;
    return setParameter_(ParameterNumber, value);
  }
  bool setParameter(FFUInt32 ParameterNumber, double d)
  {
    FFMixed value;
    value.FloatValue=d;
    return setParameter_(ParameterNumber, value);
  }
  bool setParameter(FFUInt32 ParameterNumber, std::string s)
  {
    FFMixed value;
    value.PointerValue=const_cast<char*>(s.c_str());
    return setParameter_(ParameterNumber, value);
  }

  void setParameters(gem::Properties&parms)
  {
    unsigned int i=0;
    for(i=0; i<m_parameterNames.size(); i++) {
      std::string key=m_parameterNames[i];
      std::string s1, s2;
      double d1, d2;
      switch(m_parameter.type(key)) {
      case gem::Properties::NONE:
        if(gem::Properties::NONE==parms.type(key)) {
          parms.erase(key);
          setParameter(i);
        }
        break;
      case gem::Properties::DOUBLE:
        if(m_parameter.get(key, d1) && parms.get(key, d2)) {
          if(d1!=d2) {
            m_parameter.set(key, d2);
            setParameter(i, d2);
          }
        }
        break;
      case gem::Properties::STRING:
        if(m_parameter.get(key, s1) && parms.get(key, s2)) {
          if(s1!=s2) {
            m_parameter.set(key, s2);
            setParameter(i, s2);
          }
        }
        break;
      default:
        break;
      }
    }
  }

  bool processFrameCopy(ProcessFrameCopyStruct&pfcs)
  {
    return processFrameCopy_(pfcs);
  }
  bool getInputStatus(FFUInt32 InputChannel)
  {
    return getInputStatus_( InputChannel);
  }
};


#endif /* DONT_WANT_FREEFRAME */


CPPEXTERN_NEW_WITH_ONE_ARG(pix_freeframe,  t_symbol*, A_DEFSYMBOL);

/////////////////////////////////////////////////////////
//
// pix_freeframe
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////

pix_freeframe :: pix_freeframe(t_symbol*s)
#ifndef DONT_WANT_FREEFRAME
  : m_plugin(NULL)
  , m_canopen(false)
#endif /* DONT_WANT_FREEFRAME */
{
#ifdef DONT_WANT_FREEFRAME
  throw(GemException("Gem has been compiled without FreeFrame-support!"));
#else
  int can_rgba=0;
  if(!s || s==&s_) {
    m_canopen=true;
    return;
  }
  const char *pluginname = s->s_name;

  m_plugin = new FFPlugin(pluginname, getCanvas());
  m_image.setFormat(m_plugin->GLformat());

  unsigned int numparams = m_plugin->getNumParameters();
  char tempVt[5];

  unsigned int i;
  for(i=1; i<numparams; i++) {
    snprintf(tempVt, 5, "#%d", i);
    tempVt[4]=0;
    unsigned int parmType=0;
    t_symbol*s_inletType;
    parmType=m_plugin->getParameterType(i);
    switch(parmType) {
    case FF_TYPE_EVENT:
      s_inletType=&s_bang;
      break;
    case FF_TYPE_TEXT:
      s_inletType=&s_symbol;
      break;
    default:
      s_inletType=&s_float;
    }
    m_inlet.push_back(inlet_new(this->x_obj, &this->x_obj->ob_pd, s_inletType,
                                gensym(tempVt)));
  }
#endif /* DONT_WANT_FREEFRAME */
}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
pix_freeframe :: ~pix_freeframe()
{
#ifndef DONT_WANT_FREEFRAME
  while(!m_inlet.empty()) {
    t_inlet*in=m_inlet.back();
    if(in) {
      inlet_free(in);
    }
    m_inlet.pop_back();
  }
  closeMess();
#endif /* DONT_WANT_FREEFRAME */
}

#ifndef DONT_WANT_FREEFRAME
void pix_freeframe :: closeMess()
{
  if(m_plugin) {
    delete m_plugin;
  }
  m_plugin=NULL;
}

void pix_freeframe :: openMess(t_symbol*s)
{

  if(!m_canopen) {
    error("this instance cannot dynamically change the plugin");
    return;
  }

  std::string pluginname = s->s_name;
  if(m_plugin) {
    delete m_plugin;
  }
  m_plugin=NULL;
  try {
    m_plugin = new FFPlugin(pluginname, getCanvas());
  } catch(GemException&) {
    m_plugin=NULL;
  }

  if(NULL==m_plugin) {
    error("unable to open '%s'", pluginname.c_str());
    return;
  }
}



/////////////////////////////////////////////////////////
// processImage
//
/////////////////////////////////////////////////////////
void pix_freeframe :: processImage(imageStruct &image)
{
  if(m_plugin==NULL) {
    return;
  }

  // convert the current image into a format that suits the FreeFrame-plugin
  m_image.setFormat();
  if(image.format != m_image.format) {
    if(m_image.convertFrom(&image)) {
      m_plugin->processFrame(m_image);
      m_image.convertTo(&image);
    }
  } else {
    m_plugin->processFrame(image);
  }
}


void pix_freeframe :: parmMess(std::string key, t_atom *value)
{
  if(!m_plugin) {
    error("no instance of plugin available");
    return;
  }
  if(key.empty()) {
    error("unknown key '%s'", key.c_str());
    return;
  }
  gem::Properties props;
  gem::any v;
  if(value) {
    switch(value->a_type) {
    case(A_FLOAT):
      v=atom_getfloat(value);
      break;
    case (A_SYMBOL):
      v=std::string(atom_getsymbol(value)->s_name);
      break;
    default:
      return;
    }
  }
  props.set(key, v);
  m_plugin->setParameters(props);
  setPixModified();
}


void pix_freeframe :: parmMess(int param, t_atom *value)
{
  if(!m_plugin) {
    error("no instance of plugin available");
    return;
  }
  std::string key=m_plugin->getParameterName(param-1);
  parmMess(key, value);
}


static const int offset_pix_=strlen("pix_");

static void*freeframe_loader_new(t_symbol*s, int argc, t_atom*argv)
{
  if(!s) {
    ::logpost(0, 3+2, "freeframe_loader: no name given");
    return 0;
  }

  ::logpost(0, 3+2, "freeframe_loader: %s",s->s_name);
  try {
    \
    const char*realname=s->s_name+offset_pix_; /* strip of the leading 'pix_' */
    const int typespecs[] = {};
    const unsigned int numtypespecs = sizeof(typespecs) / sizeof(*typespecs);
    gem::CPPExtern_proxy proxy(pix_freeframe_class, s->s_name, s, argc, argv,
                               numtypespecs, typespecs, 1);
    argc = proxy.getNumArgs();
    proxy.setObject(new pix_freeframe(gensym(realname)));
    return proxy.initialize();
  } catch (GemException&e) {
    ::logpost(0, 3+2, "freeframe_loader: failed! (%s)", e.what());
    return 0;
  }
  return 0;
}
bool pix_freeframe :: loader(const t_canvas*canvas,
                             const std::string&classname, const std::string&path)
{
  if(strncmp("pix_", classname.c_str(), offset_pix_)) {
    return false;
  }
  std::string pluginname = classname.substr(offset_pix_);
  if(!path.empty())
    pluginname = path + "/" + pluginname;

  pix_freeframe::FFPlugin*plugin=NULL;
  try {
    plugin=new FFPlugin(pluginname, canvas);
  } catch (GemException&) {
    //x.report();
    plugin=NULL;
  }
  if(plugin!=NULL) {
    delete plugin;
    class_addcreator(reinterpret_cast<t_newmethod>(freeframe_loader_new),
                     gensym(classname.c_str()), A_GIMME, 0);
    return true;
  }
  return false;
}

static int freeframe_loader(const t_canvas *canvas, const char *classname,
                            const char *path, bool legacy)
{
  std::string nostring;
  return pix_freeframe::loader(canvas,
                               classname?std::string(classname):nostring,
                               path?std::string(path):nostring);
}

/////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////
#endif /* DONT_WANT_FREEFRAME */

void pix_freeframe :: obj_setupCallback(t_class *classPtr)
{
  class_addanything(classPtr,
                    reinterpret_cast<t_method>(&pix_freeframe::parmCallback));
  class_addmethod  (classPtr,
                    reinterpret_cast<t_method>(&pix_freeframe::openCallback), gensym("load"),
                    A_SYMBOL, A_NULL);
  gem_register_loader(freeframe_loader);
}

void pix_freeframe :: parmCallback(void *data, t_symbol*s, int argc,
                                   t_atom*argv)
{
#ifndef DONT_WANT_FREEFRAME
  if('#'==s->s_name[0]) {
    int i = atoi(s->s_name+1);
    GetMyClass(data)->parmMess(i, (argc>0)?argv:NULL);
  } else {
    GetMyClass(data)->parmMess(std::string(s->s_name), (argc>0)?argv:NULL);
  }
#endif /* DONT_WANT_FREEFRAME */
}


void pix_freeframe :: openCallback(void *data, t_symbol*name)
{
#ifndef DONT_WANT_FREEFRAME
  GetMyClass(data)->openMess(name);
#endif /* DONT_WANT_FREEFRAME */
}
