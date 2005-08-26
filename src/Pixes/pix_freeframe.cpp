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

#ifndef DONT_WANT_FREEFRAME

# include <stdio.h>
# ifdef __WIN32__
#  include <io.h>
#  include <windows.h>
# else
#  ifdef __APPLE__
#   include <mach-o/dyld.h> 
#  else
#   define DL_OPEN
#   include <dlfcn.h>
#  endif /* __APPLE__ */
#  include <unistd.h>
# endif

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
  : m_plugin(NULL),m_instance(FF_FAIL)
#endif /* DONT_WANT_FREEFRAME */
{
#ifdef DONT_WANT_FREEFRAME
  throw(GemException("Gem has been compiled without FreeFrame-support!"));
#else
  int can_rgba=0;
  char *pluginname = s->s_name;
  char buf[MAXPDSTRING];
  char buf2[MAXPDSTRING];
  char *bufptr=NULL;

  char *extension=
#ifdef __WIN32__
    ".dll";
#else
    ".so";
#endif

  int fd=-1;
  if ((fd=open_via_path(canvas_getdir(getCanvas())->s_name, pluginname, extension, buf2, &bufptr, MAXPDSTRING, 1))>=0){
    close(fd);
    sprintf(buf, "%s/%s", buf2, bufptr);
  } else
    canvas_makefilename(getCanvas(), pluginname, buf, MAXPDSTRING);
  post("trying to load %s", buf);

  m_plugin=ff_loadplugin(buf, &can_rgba);

  post("plugin @ %X", m_plugin);

  m_image.setCsizeByFormat(can_rgba?GL_RGBA:GL_RGB);

  if(!m_plugin)throw(GemException("couldn't load FreeFrame-plugin"));

  PlugInfoStruct *pis = FF_PLUGMAIN_PIS(m_plugin(FF_GETINFO, NULL, 0));

  strncpy(m_pluginName, (char *)(pis->pluginName), 16);
  strncpy(m_pluginId, (char *)(pis->uniqueID), 4);
  m_pluginName[16] = 0;
  m_pluginId[4] = 0;

  int numparams = FF_PLUGMAIN_INT(m_plugin(FF_GETNUMPARAMETERS, NULL, 0));
  if (numparams == FF_FAIL){
    error("plugin %s: numparameters failed",  pluginname);
    throw(GemException("reading numparameters failed"));
  }

  m_inlet=new t_inlet*[numparams];

  int parmType=0;
  char tempVt[2];
  t_symbol *s_inletType=0;
  char *p_name;
  post("pix_freeframe[%s]:", pluginname);

  for(int i=0;i<numparams; i++){
    sprintf(tempVt, "#%d", i);
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
  if(m_plugin){
    if(m_instance!=FF_FAIL)m_plugin(FF_DEINSTANTIATE, NULL, m_instance);
    m_plugin(FF_DEINITIALISE, NULL, 0);
    m_plugin=NULL;
  }
#endif /* DONT_WANT_FREEFRAME */
}

#ifndef DONT_WANT_FREEFRAME
/////////////////////////////////////////////////////////
// processImage
//
/////////////////////////////////////////////////////////
void pix_freeframe :: processImage(imageStruct &image)
{
  int format=m_image.format;
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
    vidinfo.bitDepth = (format==GL_RGBA)?FF_CAP_32BITVIDEO:FF_CAP_32BITVIDEO;
    
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
    if(format==GL_RGBA)
      image.fromRGBA(m_image.data);
    else
      image.fromRGB(m_image.data);
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

}

/////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////
// plugin-loader
//
// LATER: check whether we already have loaded THIS plugin
//
// note: on linux we can load the same dll multiple times, so we don't need this check
// LATER check the other OS's
//
/////////////////////////////////////////////////////////
T_FFPLUGMAIN pix_freeframe :: ff_loadplugin(char*name, int*can_rgba)
{
  const char*hookname="plugMain";
  char*libname=name;

  unsigned int instance;

  if(name==NULL)return NULL;
  
  void *plugin_handle = NULL;
  T_FFPLUGMAIN plugmain = NULL;
  
#ifdef DL_OPEN
  plugin_handle=dlopen(libname, RTLD_NOW);
  if(!plugin_handle){
    error("pix_freeframe[%s]: %s", libname, dlerror());
    return NULL;
  }
  dlerror();

  plugmain = (T_FFPLUGMAIN)(unsigned)dlsym(plugin_handle, hookname);

#elif defined __APPLE__
  NSObjectFileImage image; 
  void *ret;
  NSSymbol s; 
  if ( NSCreateObjectFileImageFromFile( libname, &image) != NSObjectFileImageSuccess ) {
    post("%s: couldn't load", libname);
    return 0;
  }
  ret = NSLinkModule( image, libname, 
		      NSLINKMODULE_OPTION_BINDNOW + NSLINKMODULE_OPTION_PRIVATE); 
  
  s = NSLookupSymbolInModule(ret, hookname); 
  
  if (s)
    plugmain = (T_FFPLUGMAIN)NSAddressOfSymbol( s);
  else plugmain = 0;
  
#elif defined __WIN32__
  HINSTANCE ntdll;

  sys_bashfilename(libname, libname);
  ntdll = LoadLibrary(libname);
  if (!ntdll) {
    post("%s: couldn't load", libname);
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
      error("plugin %s: old api version", name);
      return NULL;
    }
  }

  if (FF_PLUGMAIN_INT(plugmain(FF_INITIALISE, NULL, 0)) == FF_FAIL){
    error("plugin %s: init failed", name);
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
      error("plugin %s: neither RGB32 nor RGB24 support!", name);

      plugmain(FF_DEINITIALISE, NULL, 0);
      return NULL;
    }
  }

  return plugmain;
}
#endif /* DONT_WANT_FREEFRAME */

void pix_freeframe :: obj_setupCallback(t_class *classPtr)
{
  class_addanything(classPtr, (t_method)&pix_freeframe::parmCallback);
}

void pix_freeframe :: parmCallback(void *data, t_symbol*s, int argc, t_atom*argv){
#ifndef DONT_WANT_FREEFRAME
  int i = atoi(s->s_name+1);
  GetMyClass(data)->parmMess(i, (argc>0)?argv:NULL);
#endif /* DONT_WANT_FREEFRAME */
}
