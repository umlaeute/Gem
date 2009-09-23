////////////////////////////////////////////////////////
//
// GEM - Graphics Environment for Multimedia
//
// zmoelnig@iem.kug.ac.at
//
// Implementation file
//
//    Copyright (c) 1997-2000 Mark Danks.
//    Copyright (c) 2001-2002 IOhannes m zmoelnig. forum::für::umläute. IEM
//    Copyright (c) 2002 James Tittle & Chris Clepper
//    For information on usage and redistribution, and for a DISCLAIMER OF ALL
//    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.
//
// this is based on EffecTV by Fukuchi Kentarou
// * Copyright (C) 2001 FUKUCHI Kentarou
//
/////////////////////////////////////////////////////////

#include "pix_freeframe.h"

#include "Base/GemLoaders.h"

#ifndef DONT_WANT_FREEFRAME

#include <stdio.h>
#ifdef _WIN32
# include <io.h>
# include <windows.h>
# define snprintf _snprintf

/*
 * Apple used to use CFBundle's to load FF plugins
 * currently this only crashes (on OSX-10.4 and OSX-10.5)
 * we therefore use dlopen() on OSX as well
 */

#elif defined __APPLE__ && 0
# include <mach-o/dyld.h> 
# include <unistd.h>
#else
# define DL_OPEN
# include <dlfcn.h>
# include <unistd.h>
#endif /* __APPLE__ */

/*
 * here comes some magic:
 *
 * on linux, "m_plugmain" is a pointer to the "plugMainType"-union, 
 *    so we can access the values via .-notation
 * on windows, "m_plugmain" is a pointer to "FF_Main_FuncPtr", which is "void*",
 *    so we have to cast
 * on osX i am not sure, but i guess it is the same as on windows
 *
 * the magic:
 *   "T_FFPLUGMAIN" is the type of m_plugmain (whatever it actually is)
 *   "FF_PLUGMAIN_...()" are some helper-functions, that do the actual union-access (or cast)
 *
 * this should keep the code fairly simple
 */

# ifdef __linux__
#  define FF_PLUGMAIN_INT(x) (x).ivalue
#  define FF_PLUGMAIN_STR(x) (x).svalue
#  define FF_PLUGMAIN_PIS(x) (x).PISvalue
# else
#  define FF_PLUGMAIN_INT(x) (int)(x)
#  define FF_PLUGMAIN_STR(x) (char*)(x)
#  define FF_PLUGMAIN_PIS(x) (PlugInfoStruct*)(x)
# endif



/////////////////////////////////////////////////////////
// plugin-loader
//
// LATER: check whether we already have loaded THIS plugin
//
// note: on linux we can load the same dll multiple times, so we don't need this check
// LATER check the other OS's
//
/////////////////////////////////////////////////////////
static T_FFPLUGMAIN ff_loadplugin(t_glist*canvas, char*pluginname, int*can_rgba, bool loud=true)
{
  const char*hookname="plugMain";

  if(pluginname==NULL)return NULL;
  
  void *plugin_handle = NULL;
  T_FFPLUGMAIN plugmain = NULL;

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
#ifdef DL_OPEN
  snprintf(buf3, MAXPDSTRING, "%s.frf/Contents/MacOS/%s", pluginname, pluginname);
#else
  // this can never work...
  snprintf(buf3, MAXPDSTRING, "%s.frf/%s", pluginname, pluginname);
#endif
  buf3[MAXPDSTRING-1]=0;
  pluginname=buf3;
#endif

  int fd=-1;
  if ((fd=open_via_path(canvas_getdir(canvas)->s_name, pluginname, extension, buf2, &bufptr, MAXPDSTRING, 1))>=0){
    close(fd);
#if defined __APPLE__ && 0
    snprintf(buf, MAXPDSTRING, "%s", buf2);
#else
    snprintf(buf, MAXPDSTRING, "%s/%s", buf2, bufptr);
post("path: %s", buf);
#endif
    buf[MAXPDSTRING-1]=0;
  } else
    canvas_makefilename(canvas, pluginname, buf, MAXPDSTRING);
  char*name=buf;
  char*libname=name;

  if(loud)::post("trying to load %s", buf);
  
#ifdef DL_OPEN
  plugin_handle=dlopen(libname, RTLD_NOW);
  if(!plugin_handle){
    if(loud)::error("pix_freeframe[%s]: %s", libname, dlerror());
    return NULL;
  }
  dlerror();

  plugmain = (T_FFPLUGMAIN)dlsym(plugin_handle, hookname);

#elif defined __APPLE__
  CFURLRef bundleURL = NULL;
  CFBundleRef theBundle = NULL;
  CFStringRef plugin = CFStringCreateWithCString(NULL, 
											 libname, kCFStringEncodingMacRoman);
  
  bundleURL = CFURLCreateWithFileSystemPath( kCFAllocatorSystemDefault,
											 plugin,
											 kCFURLPOSIXPathStyle,
											 true );
  theBundle = CFBundleCreate( kCFAllocatorSystemDefault, bundleURL );
  
  // Get a pointer to the function.
  if (theBundle){
	plugmain = (T_FFPLUGMAIN)CFBundleGetFunctionPointerForName(
            theBundle, CFSTR("plugMain") );
  }else{
    if(loud)::post("%s: couldn't load", libname);
    return 0;
  }
  if(bundleURL != NULL) CFRelease( bundleURL );
  if(theBundle != NULL) CFRelease( theBundle );
  if(plugin != NULL)    CFRelease( plugin );
#elif defined _WIN32
  HINSTANCE ntdll;

  sys_bashfilename(libname, libname);
  ntdll = LoadLibrary(libname);
  if (!ntdll) {
    if(loud)::post("%s: couldn't load", libname);
    return NULL;
  }
  plugmain = (T_FFPLUGMAIN)GetProcAddress(ntdll, hookname);
#else
# error no way to load dynamic linked libraries on this OS
#endif
  if(plugmain == NULL){
    return NULL;
  }

  PlugInfoStruct *pis = FF_PLUGMAIN_PIS(plugmain(FF_GETINFO, NULL, 0));
  if(pis){
    if (pis->APIMajorVersion < 1){
      ::error("plugin %s: old api version", name);
      return NULL;
    }
  }

  if (FF_PLUGMAIN_INT(plugmain(FF_INITIALISE, NULL, 0)) == FF_FAIL){
    ::error("plugin %s: init failed", name);
    return NULL;
  }

  /*
   * check which formats are supported:
   * currently we cannot handle RGB16 as we don't have any conversion routines implemented
   * the other options are RGB==RGB24 and RGBA=RGB32
   * we prefer RGB32, as this is one of our native formats (and YUV2RGBA conversion is likely to be faster)
   * so we check whether the plugin knows how to do RGB32
   * if it doesn't, we try RGB24
   */
  if (FF_PLUGMAIN_INT(plugmain(FF_GETPLUGINCAPS, (LPVOID)FF_CAP_32BITVIDEO, 0)) == FF_TRUE){
    if(can_rgba)*can_rgba=1;
  } else {
    if(can_rgba)*can_rgba=0;
    if (FF_PLUGMAIN_INT(plugmain(FF_GETPLUGINCAPS, (LPVOID)FF_CAP_24BITVIDEO, 0)) != FF_TRUE){
      ::error("plugin %s: neither RGB32 nor RGB24 support!", name);

      plugmain(FF_DEINITIALISE, NULL, 0);
      return NULL;
    }
  }

  return plugmain;
}
#endif /* DONT_WANT_FREEFRAME */


CPPEXTERN_NEW_WITH_ONE_ARG(pix_freeframe,  t_symbol *, A_DEFSYM)

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
  : m_plugin(NULL),m_instance(FF_FAIL),
    m_numparameters(0),
    m_inlet(NULL)
#endif /* DONT_WANT_FREEFRAME */
{
#ifdef DONT_WANT_FREEFRAME
  throw(GemException("Gem has been compiled without FreeFrame-support!"));
#else
  int can_rgba=0;
  char *pluginname = s->s_name;
  m_plugin=ff_loadplugin(getCanvas(), pluginname, &can_rgba);

  m_image.setCsizeByFormat(can_rgba?GL_RGBA_GEM:GL_RGB);

  if(!m_plugin)throw(GemException("couldn't load FreeFrame-plugin"));

  PlugInfoStruct *pis = FF_PLUGMAIN_PIS(m_plugin(FF_GETINFO, NULL, 0));

  strncpy(m_pluginName, (char *)(pis->pluginName), 16);
  strncpy(m_pluginId, (char *)(pis->uniqueID), 4);
  m_pluginName[16] = 0;
  m_pluginId[4] = 0;

  unsigned int numparams = FF_PLUGMAIN_INT(m_plugin(FF_GETNUMPARAMETERS, NULL, 0));
  if (numparams == FF_FAIL){
    error("plugin %s: numparameters failed",  pluginname);
    throw(GemException("reading numparameters failed"));
  }
  m_inlet=new t_inlet*[numparams];
  m_numparameters=numparams;

  int parmType=0;
  char tempVt[5];
  t_symbol *s_inletType=0;
  char *p_name;
  post("parameters for '%s':", pluginname);
  for(unsigned int i=0;i<numparams; i++){
    snprintf(tempVt, 5, "#%d", i);
    tempVt[4]=0;
    // display
    //   ParameterName:
    //   ParameterDisplayValue:
    // use
    //   ParameterType
    parmType=FF_PLUGMAIN_INT(m_plugin(FF_GETPARAMETERTYPE, (LPVOID)i, 0));
    p_name=  FF_PLUGMAIN_STR(m_plugin(FF_GETPARAMETERNAME, (LPVOID)i, 0));

    post("\tparam%s: %s", tempVt, p_name);

    switch(parmType){
    case FF_TYPE_EVENT:
      s_inletType=&s_bang;
      break;
    case FF_TYPE_TEXT:
      s_inletType=&s_symbol;
      break;
    default:
      s_inletType=&s_float;    
    }
    m_inlet[i]=inlet_new(this->x_obj, &this->x_obj->ob_pd, s_inletType, gensym(tempVt));
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
  if(m_inlet){
    delete[]m_inlet;
  }
  closeMess();
#endif /* DONT_WANT_FREEFRAME */
}

#ifndef DONT_WANT_FREEFRAME
void pix_freeframe :: closeMess()
{
  if(m_plugin){
    if(m_instance!=FF_FAIL)m_plugin(FF_DEINSTANTIATE, NULL, m_instance);
    m_plugin(FF_DEINITIALISE, NULL, 0);
    m_plugin=NULL;
  }
}

void pix_freeframe :: openMess(t_symbol*s)
{
  int can_rgba=0;
  char *pluginname = s->s_name;
  T_FFPLUGMAIN plugin=ff_loadplugin(getCanvas(), pluginname, &can_rgba);
  if(NULL==plugin) {
    error("unable to open '%s'", pluginname);
    return;
  }
  unsigned int numparams = FF_PLUGMAIN_INT(plugin(FF_GETNUMPARAMETERS, NULL, 0));
  if (numparams == FF_FAIL) {
    error("unable to query numparameters of '%s'", pluginname);
    return;
  }


  closeMess();
  m_plugin=plugin;

  PlugInfoStruct *pis = FF_PLUGMAIN_PIS(m_plugin(FF_GETINFO, NULL, 0));

  strncpy(m_pluginName, (char *)(pis->pluginName), 16);
  strncpy(m_pluginId, (char *)(pis->uniqueID), 4);
  m_pluginName[16] = 0;
  m_pluginId[4] = 0;

  m_numparameters=numparams;
}



/////////////////////////////////////////////////////////
// processImage
//
/////////////////////////////////////////////////////////
void pix_freeframe :: processImage(imageStruct &image)
{
  unsigned int format=m_image.format;
  unsigned char*data=image.data;

  if(m_plugin==NULL)return;

  if(m_instance==FF_FAIL ||  m_image.xsize!=image.xsize || m_image.ysize!=image.ysize){
    // either make the first instance of the plugin
    // OR the input format has changed, so we have to re-instantiate

    if(m_instance!=FF_FAIL)m_plugin(FF_DEINSTANTIATE, NULL, m_instance);
    
    VideoInfoStruct vidinfo;
    m_image.xsize=image.xsize;
    m_image.ysize=image.ysize;
    
    vidinfo.frameWidth = image.xsize;
    vidinfo.frameHeight = image.ysize;
    // the default openGL-orientation is (0,0)==lowerleft, which is 2 in FreeFrame
    vidinfo.orientation = (image.upsidedown)?1:2;

    // this needs a bit more intelligence:
    // the plugin might support RGBA and/or RGB
    // what is fastest ???
    vidinfo.bitDepth = (format==GL_RGBA_GEM)?FF_CAP_32BITVIDEO:FF_CAP_32BITVIDEO;
    
    m_instance = FF_PLUGMAIN_INT(m_plugin(FF_INSTANTIATE, &vidinfo, 0));
    
    if(m_instance==FF_FAIL)return;
    m_image.reallocate();
    
  }

  // convert the current image into a format that suits the FreeFrame-plugin
  if(image.format!=format){
    switch (image.format){
    case GL_RGBA:
      m_image.fromRGBA(image.data);
      break;
    case GL_BGRA_EXT: /* "RGBA" on apple */
      m_image.fromBGRA(image.data);
      break;
    case GL_LUMINANCE: // greyscale
      m_image.fromGray(image.data);
      break;
    case GL_YUV422_GEM: // YUV
      m_image.fromYUV422(image.data);
      break;
    }
    data=m_image.data;
  } else 
    data=image.data;
  
  // yeah, do it!
  m_plugin(FF_PROCESSFRAME, data,  m_instance);
  



  // check whether we have converted our image data
  if(image.data!=data)
    // it seems, like we did: convert it back


  // just copied the code from [pix_rgba]
    switch(format) {
    case GL_RGBA: 
      image.fromRGBA(m_image.data);
      break;
    case GL_RGB:  
      image.fromRGB(m_image.data);
      break;
    case GL_BGR_EXT:
      image.fromBGR(m_image.data);
      break;
    case GL_BGRA_EXT: /* "RGBA" on apple */
      image.fromBGRA(m_image.data);
      break;
    case GL_LUMINANCE:
      image.fromGray(m_image.data);
      break;
    case GL_YCBCR_422_GEM: // YUV
      image.fromUYVY(m_image.data);
      break;
    default:
      error("no method for this format !!!");
      error("if you know how to convert this format (%X),\n"
            "please contact the authors of this software", image.format);
      return;
    }
}


void pix_freeframe :: parmMess(int param, t_atom *value){
  if(m_instance!=FF_FAIL){
    SetParameterStruct sps;
    sps.index = param;

    switch (FF_PLUGMAIN_INT(m_plugin(FF_GETPARAMETERTYPE, (LPVOID)param, 0))){
    case FF_TYPE_EVENT:
      sps.value.fvalue=1.0;
      break;
    case FF_TYPE_TEXT:
      sps.value.svalue=atom_getsymbol(value)->s_name;
      break;
    default:
      sps.value.fvalue=atom_getfloat(value);
    }

    m_plugin(FF_SETPARAMETER, &sps, m_instance);
  } else {
    error("pix_freeframe: no instance of plugin available");
  }
  setModified();
}


static const int offset_pix_=strlen("pix_");

static void*freeframe_loader_new(t_symbol*s, int argc, t_atom*argv) {
  ::verbose(2, "freeframe_loader: %s",(s?(s->s_name):"<none>"));
  try{	    	    	    	    	    	    	    	\
    Obj_header *obj = new (pd_new(pix_freeframe_class),(void *)NULL) Obj_header;
    char*realname=s->s_name+offset_pix_; /* strip of the leading 'pix_' */
    CPPExtern::m_holder = &obj->pd_obj;
    CPPExtern::m_holdname=s->s_name;
    obj->data = new pix_freeframe(gensym(realname));
    CPPExtern::m_holder = NULL;
    CPPExtern::m_holdname=NULL;
    return(obj);
  } catch (GemException e) {
    ::verbose(2, "freeframe_loader: failed!");
    //e.report(); 
    return NULL;
  }
  return 0;
}

static int freeframe_loader(t_canvas *canvas, char *classname) {
  int dummy;

  if(strncmp("pix_", classname, offset_pix_))
    return 0;
  char*pluginname=classname+offset_pix_;

  T_FFPLUGMAIN plugin = ff_loadplugin(canvas, pluginname, &dummy, false);
  if(plugin!=NULL) {
    plugin(FF_DEINITIALISE, NULL, 0);
    class_addcreator((t_newmethod)freeframe_loader_new, gensym(classname), A_GIMME, 0);
    return 1;
  }
  return 0;
}  

/////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////
#endif /* DONT_WANT_FREEFRAME */

void pix_freeframe :: obj_setupCallback(t_class *classPtr)
{
  class_addanything(classPtr, (t_method)&pix_freeframe::parmCallback);
  gem_register_loader(freeframe_loader);
}

void pix_freeframe :: parmCallback(void *data, t_symbol*s, int argc, t_atom*argv){
#ifndef DONT_WANT_FREEFRAME
  int i = atoi(s->s_name+1);
  GetMyClass(data)->parmMess(i, (argc>0)?argv:NULL);
#endif /* DONT_WANT_FREEFRAME */
}


void pix_freeframe :: openCallback(void *data, t_symbol*name){
#ifndef DONT_WANT_FREEFRAME
  GetMyClass(data)->openMess(name);
#endif /* DONT_WANT_FREEFRAME */
}
