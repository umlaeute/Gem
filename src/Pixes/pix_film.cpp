////////////////////////////////////////////////////////
//
// GEM - Graphics Environment for Multimedia
//
// zmoelnig@iem.kug.ac.at
//
// Implementation file
//
//    Copyright (c) 1997-1999 Mark Danks.
//    Copyright (c) Günther Geiger.
//    Copyright (c) 2001-2011 IOhannes m zmölnig. forum::für::umläute. IEM. zmoelnig@iem.at
//    Copyright (c) 2002 James Tittle & Chris Clepper
//    For information on usage and redistribution, and for a DISCLAIMER OF ALL
//    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.
//
/////////////////////////////////////////////////////////
#include "Gem/GemConfig.h"
#ifndef GEM_FILMBACKEND

#include "pix_film.h"
#include "Gem/Image.h"

#include "Gem/State.h"
#include "Gem/Cache.h"
#include "Gem/Properties.h"

#include "plugins/PluginFactory.h"
#include "Gem/Exception.h"

#include <ctype.h>
#include <stdio.h>

#include <sstream>

/***************************************
 * on the order of codec-libraries
 *
 * the various libraries should be called
 * in such order, as to guarantee the most stable
 * and fastest and generally best decoding for
 * all movies
 *
 * libraries that know most formats need not be first
 * libraries which are likely to crash should be last
 * libraries which are fast should be first
 * libraries which offer features (like seeking) should be first
 *
 * of course we have a problem, if we have one fast non-seeking library and
 * one slower seeking library; which one should go first ???
 *
 * **********************
 *
 * available libraries:
 *
 * AVI: w32 only
 *      can only decode a small number of movies (using a rather ancient API)
 *      generally considered stable
 * QT : osX and w32
 *      will decode pretty everything on osX and (i guess) it is rock stable there
 *      on w32 it might well crash (at least when fed with an mpeg)
 *      should therefore be one of the last libraries for w23
 * gmerlin: linux only (although there are ports to w32/osX (?))
 *         should decode pretty much
 *         API for a bunch of other decoding libraries (e.g. FFMPEG)
 *         probems with mpeg files
 * MPEG1: linux only
 *        no seeking support
 *        likely to crash
 *        ONLY as fallback if nothing else works or is installed
 * MPEG3: linux only
 *        stable
 *        seeking support (have to create TOC out of MPEG-file)
 * QT4L : linux only
 *        stable
 *        view codecs (?)
 * AVIPLAY: linux only
 *          stable (?)
 *          API for a bunch of other decoding libraries (e.g. FFMPEG)
 *          slow (?? but if it uses FFMPEG then it should be as fast as FFMPEG)
 *
 * libraries which might be worth done:
 *
 * MPEG2: linux only
 *        fast (?)
 *        no seeking
 *
 * *************************
 * proposed order:
 *
 * GMERLIN, AVI, QT, QT4L, MPEG3, AVIPLAY, MPEG1
 *
 * W32:
 *  the few movies that AVI can decode it will handle fine
 *  the rest is caught by QT
 *  no MPEG-decoding though (and QT is likely to crash with it)
 *
 * osX:
 *  everything is handled by QT
 *
 * linux:
 *  most movies will be decoded by GMERLIN
 *  few movies will be decoded by QT4L (but those are OK: seekable, stable)
 *  mpegs will be decoded by MPEG3 in a stable way
 *  everything else should be handled by AVIPLAY or FFMPEG (aviplay including ffmpeg)
 *  if there is no MPEG3 available at compile time, we have MPEG1 as a last fallback (unstable)
 *
 * LATER:
 *   think about the gmerlin+mpeg problem
 *
 ***************************************/


#if 1
# define debug
#else
# define debug post
#endif

static std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems) {
    std::stringstream ss(s);
    std::string item;
    while(std::getline(ss, item, delim)) {
        elems.push_back(item);
    }
    return elems;
}


static std::vector<std::string> split(const std::string &s, char delim) {
    std::vector<std::string> elems;
    return split(s, delim, elems);
}




CPPEXTERN_NEW_WITH_ONE_ARG(pix_film, t_symbol *, A_DEFSYM);

#ifdef HAVE_PTHREADS
/* the "capturing"-thread */
void *pix_film :: grabThread(void*you)
{
  pix_film *me=reinterpret_cast<pix_film*>(you);
  struct timeval timout;
  me->m_thread_running=true;
  //me->post("using pthreads");
  while(me->m_thread_continue){
    int reqFrame=static_cast<int>(me->m_reqFrame);
    int reqTrack=static_cast<int>(me->m_reqTrack);
    timout.tv_sec = 0;
    timout.tv_usec=100;

    if(reqFrame!=me->m_curFrame || reqTrack!=me->m_curTrack){

      pthread_mutex_lock(me->m_mutex);
      if (gem::plugins::film::FAILURE!=me->m_handle->changeImage(reqFrame, reqTrack)){
        me->m_frame=me->m_handle->getFrame();
      } else me->m_frame=0;

      me->m_curFrame=reqFrame;
      me->m_curTrack=reqTrack;

      pthread_mutex_unlock(me->m_mutex);
    }
    select(0,0,0,0,&timout);
  }

  me->m_thread_running=false;
  return NULL;
}
#endif

/////////////////////////////////////////////////////////
//
// pix_film
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
pix_film :: pix_film(t_symbol *filename) :
  m_haveMovie(0),
  m_auto(0), m_format(GL_RGBA),
  m_numFrames(0), m_reqFrame(0), m_curFrame(0),
  m_numTracks(0), m_reqTrack(0), m_curTrack(0),
  m_handle(NULL),
  m_outNumFrames(NULL), m_outEnd(NULL),
#ifdef HAVE_PTHREADS
  m_thread_id(0), m_mutex(NULL), m_frame(NULL), m_thread_continue(false),
  m_thread_running(false), m_wantThread(true)
#else
  m_thread_running(false), m_wantThread(false)
#endif
{
  m_handle = gem::plugins::film::getInstance();

  // setting the current frame
  inlet_new(this->x_obj, &this->x_obj->ob_pd, gensym("float"), gensym("img_num"));
  // create an outlet to send out how many frames are in the movie + bang when we reached the end
  m_outNumFrames = outlet_new(this->x_obj, 0);
  m_outEnd       = outlet_new(this->x_obj, 0);


  if(m_handle) {
    std::string backends;
    gem::Properties props;
    gem::any value;
    value=backends;
    props.set("backends", value);
    m_handle->getProperties(props);
    if(props.get("backends", backends)) {
      m_ids=split(backends, ' ', m_ids);
    }
  }
  unsigned int i;
  static bool firsttime=true;
  if(firsttime) {
    for(i=0; i<m_ids.size(); i++) {
      verbose(0, "%s support", m_ids[i].c_str());
    }
  }
  firsttime=false;
}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
pix_film :: ~pix_film()
{
  // Clean up the movie
  closeMess();

  delete m_handle;
  m_handle=NULL;
}

/////////////////////////////////////////////////////////
// closeMess
//
/////////////////////////////////////////////////////////
void pix_film :: closeMess(void){

#ifdef HAVE_PTHREADS
  if(m_thread_running)
    {
      void *dummy=0;
      int counter=0;
      m_thread_continue = false;
      pthread_join (m_thread_id, &dummy);
      while(m_thread_running)
        {
          counter++;
        }
    }
  m_thread_id=0;

  if ( m_mutex )
    {
      pthread_mutex_destroy(m_mutex);
      free(m_mutex);
      m_mutex=NULL;
    }
#endif

  if(m_handle)
    m_handle->close();

  m_haveMovie=0;
}

/////////////////////////////////////////////////////////
// openMess
//
/////////////////////////////////////////////////////////
void pix_film :: openMess(std::string filename, int format, unsigned int backendNum) {
  std::string backend;
  if(m_ids.size()>0)
    backend=m_ids[backendNum%m_ids.size()];

  openMess(filename, format, backend);
}

void pix_film :: openMess(std::string filename, int format, std::string backend)
{
  gem::Properties wantProps, gotProps;

  closeMess();

  char buff[MAXPDSTRING];
  std::string fname=filename;
  // we first try to find the file-to-open with canvas_makefilename
  // if this fails, we just pass the given filename (could be a stream)
  canvas_makefilename(const_cast<t_canvas*>(getCanvas()), const_cast<char*>(filename.c_str()), buff, MAXPDSTRING);
  if (FILE*fd=fopen(buff, "r")) {
    fname=buff;
    fclose(fd);
  }


  if (format==0)format=m_format;
  double d=(double)format;
  wantProps.set("colorspace", format);

  if(m_auto!=0.f) {
    double v=m_auto;
    wantProps.set("auto", v);
  }

  if(!backend.empty()) {
    wantProps.set("backends", backend);
  }

  if(!m_handle->open(fname, wantProps)) {
    error("unable to open file: %s", filename.c_str());
    return;
  }

  m_haveMovie=1;


  double width=-1;
  double height=-1;
  double frames=-1;
  double fps=-1;
  gotProps.set("width", 0);
  gotProps.set("height", 0);
  gotProps.set("frames", 0);
  gotProps.set("fps", 0);

  m_handle->getProperties(gotProps);

  gotProps.get("width", width);
  gotProps.get("height", height);
  gotProps.get("frames", frames);
  gotProps.get("fps", fps);

  t_atom ap[4];
  SETFLOAT(ap, frames);
  SETFLOAT(ap+1, width);
  SETFLOAT(ap+2, height);
  SETFLOAT(ap+3, fps);
  m_numFrames=frames;
  post("loaded file: %s with %d frames (%dx%d) at %f fps",
       fname.c_str(),
       (int)frames,
       (int)width,
       (int)height,
       fps);
  outlet_list(m_outNumFrames, 0, 4, ap);

#ifdef HAVE_PTHREADS
  bool canThread=m_handle->isThreadable();
  if(canThread && m_wantThread){
    debug("creating thread");
    m_mutex = (pthread_mutex_t*) malloc(sizeof(pthread_mutex_t));
    if ( pthread_mutex_init(m_mutex, NULL) < 0 ) {
      perror("pix_film : couldn't create mutex");
    } else {
      m_thread_continue = true;
      m_reqFrame=0;
      m_curFrame=-1;
      pthread_create(&m_thread_id, 0, grabThread, this);
      debug("thread created");
    }
  }
#endif
}

/////////////////////////////////////////////////////////
// render
//
/////////////////////////////////////////////////////////
void pix_film :: render(GemState *state)
{
  int frame=-1;
  /* get the current frame from the file */
  if (!m_handle || m_haveMovie==0)return;

#ifdef HAVE_PTHREADS
  if(m_thread_running) {
    pthread_mutex_lock(m_mutex);
    state->set(GemState::_PIX, m_frame);
  } else
#endif /* PTHREADS */
    state->set(GemState::_PIX, m_handle->getFrame());

  pixBlock*img=NULL;
  state->get(GemState::_PIX, img);

  // someone wants to process the image downstream, so make sure they get it
  if (m_cache&&m_cache->resendImage&&img)
    {
      img->newimage=true;
      m_cache->resendImage = 0;
    }

  frame=static_cast<int>(m_reqFrame);
  if (NULL==img){
    outlet_float(m_outEnd,(m_numFrames>0 && static_cast<int>(m_reqFrame)<0)?(m_numFrames-1):0);

    if(frame!=static_cast<int>(m_reqFrame)){
      // someone responded immediately to the outlet_float and changed the requested frame
      // so get the newly requested frame:

      if(m_thread_running){
        /* if we are threaded (currently locked!), we change the frame# and grab the frame immediately
         * (if we are not threaded, the frame# is already changed and the grabbing is always immediately)
         */
        m_handle->changeImage(static_cast<int>(m_reqFrame), m_reqTrack);
      }
      state->set(GemState::_PIX, m_handle->getFrame());

    }
  }
}

/////////////////////////////////////////////////////////
// postrender
//
/////////////////////////////////////////////////////////
void pix_film :: postrender(GemState *state)
{
  if (!m_handle || m_haveMovie==0)return;
  if (state) {
    pixBlock*img=NULL;
    state->get(GemState::_PIX, img);
    if(img)
      img->newimage = false;
  }

#ifdef HAVE_PTHREADS
  if(m_thread_running){
    pthread_mutex_unlock(m_mutex);
  }
#endif /* PTHREADS */

  // automatic proceeding
  m_reqFrame+=m_auto;

  if (m_auto!=0 && !m_thread_running){
    if (gem::plugins::film::FAILURE==m_handle->changeImage(static_cast<int>(m_reqFrame+=m_auto))){
      //      m_reqFrame = m_numFrames;
      outlet_bang(m_outEnd);
    }
  }
}

/////////////////////////////////////////////////////////
// changeImage
//
/////////////////////////////////////////////////////////
void pix_film :: changeImage(int imgNum, int trackNum)
{
  if (imgNum < 0){
    error("selection number must be > 0");
    imgNum=0;
  }
#if 0
  if (trackNum < 0){
    error("track number must be > 0");
    trackNum=0;
  }
#endif
  if (m_handle){
    if(!m_thread_running){
      if (gem::plugins::film::FAILURE==m_handle->changeImage(imgNum, trackNum)){
        outlet_bang(m_outEnd);
      }
    }
    m_reqFrame=imgNum;
    m_reqTrack=trackNum;
  }
}
/////////////////////////////////////////////////////////
// colorSpace
//
/////////////////////////////////////////////////////////
void pix_film :: csMess(t_symbol *s, bool immediately)
{
  char c =*s->s_name;
  switch (c){
  case 'g': case 'G': m_format=GL_LUMINANCE; break;
  case 'y': case 'Y': m_format=GL_YCBCR_422_GEM; break;
  case 'r': case 'R':
    if(gensym("RGB")==s||gensym("rgb")==s)
      m_format=GL_RGB;
    else
      m_format=GL_RGBA;
    break;
  default:
    error("colorspace must be 'RGBA', 'YUV' or 'Gray'");
  }

  gem::Properties props;
  double d=(double)m_format;
  gem::any value=d;
  props.set("colorspace", value);
  if(immediately && m_handle)
    m_handle->setProperties(props);
}
/////////////////////////////////////////////////////////
// threading
//
/////////////////////////////////////////////////////////
void pix_film :: threadMess(int state)
{
  m_wantThread=!(!state);
#ifdef HAVE_PTHREADS
  post("thread settings will have an effect on next open!");
#else
  post("no thread support");
#endif
}

void pix_film :: autoMess(double speed)
{
  m_auto=(t_float)speed;
  gem::Properties props;
  gem::any value=speed;
  props.set("auto", value);
  if(m_handle)
    m_handle->setProperties(props);
}




/////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////
void pix_film :: obj_setupCallback(t_class *classPtr)
{
  /* really: Pd shouldn't bail out,
   * if a selector is bound to the same method of a class a 2nd time
   */
  if(pd_objectmaker && reinterpret_cast<t_gotfn>(create_pix_film)==zgetfn(&pd_objectmaker, gensym("pix_filmQT"))) {
    ::verbose(2, "not registering [pix_filmQT] again...");
  } else {
    class_addcreator(reinterpret_cast<t_newmethod>(create_pix_film), gensym("pix_filmQT"), A_DEFSYM, A_NULL);
  }

  class_addmethod(classPtr, reinterpret_cast<t_method>(&pix_film::openMessCallback),
		  gensym("open"), A_GIMME, A_NULL);
  class_addmethod(classPtr, reinterpret_cast<t_method>(&pix_film::changeImageCallback),
		  gensym("img_num"), A_GIMME, A_NULL);

  CPPEXTERN_MSG1(classPtr, "auto", autoMess, t_float);
  CPPEXTERN_MSG1(classPtr, "colorspace", csMess, t_symbol*);
  CPPEXTERN_MSG1(classPtr, "thread", threadMess, bool);
}
void pix_film :: openMessCallback(void *data, t_symbol*s,int argc, t_atom*argv)
{
  // possible messages:
  //  's:filename'
  //  's:filename' 's:colorspace'
  //  's:filename' 'i:codec'
  //  's:filename' 's:colorspace' 'i:codec'
  //  's:filename' 'i:codec' 's:colorspace'
  //  's:filename' 's:colorspace' 's:codec'

  int backendID=-1;
  std::string backend;

  if (!argc || argc>3)goto illegal_openmess;
  if (argv[0].a_type != A_SYMBOL)goto illegal_openmess;

  if (argc==2){
    if (argv[1].a_type == A_SYMBOL)
      GetMyClass(data)->csMess(atom_getsymbol(argv+1), false);
    else if (argv[1].a_type == A_FLOAT)
      backendID=atom_getint(argv+1);
  } else if (argc==3){
    if ((argv[1].a_type == A_SYMBOL) && (argv[2].a_type == A_FLOAT)) {
      GetMyClass(data)->csMess(atom_getsymbol(argv+1), false);
      backendID=atom_getint(argv+2);
    } else if ((argv[2].a_type == A_SYMBOL) && (argv[1].a_type == A_FLOAT)) {
      GetMyClass(data)->csMess(atom_getsymbol(argv+2), false);
      backendID=atom_getint(argv+1);

    } else if ((argv[1].a_type == A_SYMBOL) && (argv[2].a_type == A_SYMBOL)) {
      GetMyClass(data)->csMess(atom_getsymbol(argv+1), false);
      backend=atom_getsymbol(argv+2)->s_name;
    }
  }
  if(backendID>=0)
    GetMyClass(data)->openMess(atom_getsymbol(argv)->s_name, 0, backendID);
  else
    GetMyClass(data)->openMess(atom_getsymbol(argv)->s_name, 0, backend);

  return;
 illegal_openmess:
  GetMyClass(data)->error("open <filename> [<format>] [<preferred backend>]");
  return;

}

void pix_film :: changeImageCallback(void *data, t_symbol *, int argc, t_atom *argv){
    GetMyClass(data)->changeImage((argc<1)?0:atom_getint(argv), (argc<2)?0:atom_getint(argv+1));
}
#endif /*OS-specific GEM_FILMBACKEND */

