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
//    Copyright (c) 2001-2002 IOhannes m zmoelnig. forum::für::umläute. IEM
//    Copyright (c) 2002 James Tittle & Chris Clepper
//    For information on usage and redistribution, and for a DISCLAIMER OF ALL
//    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.
//
/////////////////////////////////////////////////////////
#include "Base/GemConfig.h"
#ifndef GEM_FILMBACKEND

#include "pix_film.h"
#include "Base/GemState.h"

#include "Base/GemCache.h"

#include <ctype.h>
#include <stdio.h>

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
      if (me->m_handle->changeImage(reqFrame, reqTrack)!=FILM_ERROR_FAILURE){
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
  m_auto(0), m_format(GL_RGBA),
  m_numFrames(0), m_reqFrame(0), m_curFrame(0),
  m_numTracks(0), m_reqTrack(0), m_curTrack(0),
#ifdef HAVE_PTHREADS
  m_thread_id(0), m_mutex(NULL), m_frame(NULL), m_thread_continue(false),
  m_thread_running(false), m_wantThread(true)
#else
  m_thread_running(false), m_wantThread(false)
#endif
{
  gem::PluginFactory<gem::film>::loadPlugins("film");

  // setting the current frame
  inlet_new(this->x_obj, &this->x_obj->ob_pd, gensym("float"), gensym("img_num"));
  // create an outlet to send out how many frames are in the movie + bang when we reached the end
  m_outNumFrames = outlet_new(this->x_obj, 0);
  m_outEnd       = outlet_new(this->x_obj, 0);
  
  m_handle=NULL;

  std::vector<std::string>available_ids=gem::PluginFactory<gem::film>::getIDs();

  if(!addHandle(available_ids, "DS"))
    addHandle(available_ids, "AVI");

  addHandle(available_ids, "gmerlin");
  addHandle(available_ids, "QT");
  addHandle(available_ids, "QT4L");
  addHandle(available_ids, "MPEG3");
  addHandle(available_ids, "AVIPLAY");
  addHandle(available_ids, "MPEG1");

  // the rest
  addHandle(available_ids);
  //openMess(filename);


  if(m_handles.size()==0) {
    error("no movie decoding backends found!");
  }

}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
pix_film :: ~pix_film()
{
  // Clean up the movie
  closeMess();

  int i=0;
  for(i=0; i<m_handles.size(); i++) {
    delete m_handles[i];
    m_handles[i]=NULL;
  }
}

/////////////////////////////////////////////////////////
// add backends
//
/////////////////////////////////////////////////////////
bool pix_film :: addHandle( std::vector<std::string>available, std::string ID)
{
  int i=0;
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
        gem::film         *handle=gem::PluginFactory<gem::film>::getInstance(key); 
        if(NULL==handle)break;
        m_ids.push_back(key);
        m_handles.push_back(handle);
        count++;
        verbose(2, "added backend#%d '%s' @ 0x%x", m_handles.size()-1, key.c_str(), handle);
    }
  }

  return (count>0);
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

  // Clean up any open files
  int i=m_handles.size();
  debug("closing %d handles", i);
  while(i--){
    debug("close %d", i);
    if(m_handles[i]){
      m_handles[i]->close();
    }
  }
  m_handle=NULL;
  //if(m_handle!=0)m_handle->close();
  debug("closed");
}

/////////////////////////////////////////////////////////
// openMess
//
/////////////////////////////////////////////////////////
void pix_film :: openMess(t_symbol *filename, int format, int codec)
{
  //  if (filename==x_filename)return;
  closeMess();

  char buff[MAXPDSTRING];
  char*buf=buff;
  // we first try to find the file-to-open with canvas_makefilename
  // if this fails, we just pass the given filename (could be a stream)
  canvas_makefilename(const_cast<t_canvas*>(getCanvas()), filename->s_name, buff, MAXPDSTRING);
  if (FILE*fd=fopen(buff, "r"))fclose(fd);
  else buf=filename->s_name;
  m_handle=0;

  if (format==0)format=m_format;
  if(codec>=0){
    codec=codec%m_handles.size();
    if (m_handles[codec] && m_handles[codec]->open(buf, format ))
      m_handle = m_handles[codec];
  }
  debug("handle=%x of %d", m_handle, m_handles.size());
  if(!m_handle && m_handles.size()>0){
    int i=0;
    post("opening %s with format %X", buf, format);
    while(i<m_handles.size()){
      debug("trying handle %d: %x", i, m_handles[i]);
      if (m_handles[i] && m_handles[i]->open(buf, format ))      {
        debug("success", i, m_handles[i]);
        m_handle = m_handles[i];
        break;
      }
      post(" ... ");
      i++;
    }
  }

  debug("got handle = %X", m_handle);

  if (!m_handle){
    post(" ... giving up!");
    error("unable to open file: %s", filename->s_name);
	return;
  }

  t_atom ap[4];
  float fps=static_cast<t_float>(m_handle->getFPS());
  SETFLOAT(ap, m_handle->getFrameNum());
  SETFLOAT(ap+1, m_handle->getWidth());
  SETFLOAT(ap+2, m_handle->getHeight());
  SETFLOAT(ap+3, fps);
  m_numFrames=m_handle->getFrameNum();
  post("loaded file: %s with %d frames (%dx%d) at %f fps", 
       buf, 
       m_handle->getFrameNum(), 
       m_handle->getWidth(), 
       m_handle->getHeight(), fps);
  outlet_list(m_outNumFrames, 0, 4, ap);

#ifdef HAVE_PTHREADS
  if(m_wantThread){
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
  if (!m_handle)return;

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


  m_handle->setAuto(m_auto);

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
  if(!m_handle)return;
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
  if (m_auto!=0){
    if(m_thread_running){
      m_reqFrame+=m_auto;
    } else
      if (m_handle->changeImage(static_cast<int>(m_reqFrame+=m_auto))==FILM_ERROR_FAILURE){
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
      if (m_handle->changeImage(imgNum, trackNum)==FILM_ERROR_FAILURE){
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
  if(immediately && m_handle)m_handle->requestColor(m_format);
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
  class_addmethod(classPtr, reinterpret_cast<t_method>(&pix_film::autoCallback),
		  gensym("auto"), A_DEFFLOAT, A_NULL);
  class_addmethod(classPtr, reinterpret_cast<t_method>(&pix_film::csCallback),
		  gensym("colorspace"), A_DEFSYM, A_NULL);
  class_addmethod(classPtr, reinterpret_cast<t_method>(&pix_film::threadCallback),
		  gensym("thread"), A_FLOAT, A_NULL);
}
void pix_film :: openMessCallback(void *data, t_symbol*s,int argc, t_atom*argv)
{
  int codec=-1;

  if (!argc || argc>3)goto illegal_openmess;
  if (argv[0].a_type != A_SYMBOL)goto illegal_openmess;

  if (argc==2){
    if (argv[1].a_type == A_SYMBOL)
      GetMyClass(data)->csMess(atom_getsymbol(argv+1), false);
    else if (argv[1].a_type == A_FLOAT)
      codec=atom_getint(argv+1);
  } else if (argc==3){
    if ((argv[1].a_type == A_SYMBOL) || (argv[2].a_type == A_FLOAT)) {
      GetMyClass(data)->csMess(atom_getsymbol(argv+1), false);
      codec=atom_getint(argv+2);    
    } else if ((argv[2].a_type == A_SYMBOL) || (argv[1].a_type == A_FLOAT)) {
      GetMyClass(data)->csMess(atom_getsymbol(argv+2), false);
      codec=atom_getint(argv+1);  
    }
  }
  GetMyClass(data)->openMess(atom_getsymbol(argv), 0, codec);

  return;
 illegal_openmess:
  GetMyClass(data)->error("open <filename> [<format>] [<preferred codec#>]");
  return;
  
}

void pix_film :: changeImageCallback(void *data, t_symbol *, int argc, t_atom *argv){
    GetMyClass(data)->changeImage((argc<1)?0:atom_getint(argv), (argc<2)?0:atom_getint(argv+1));
}

void pix_film :: autoCallback(void *data, t_floatarg state)
{
  GetMyClass(data)->m_auto=state;
}
void pix_film :: csCallback(void *data, t_symbol*s)
{
  GetMyClass(data)->csMess(s);
}

void pix_film :: threadCallback(void *data, t_floatarg state)
{
  GetMyClass(data)->threadMess(static_cast<int>(state));
}
#endif /*OS-specific GEM_FILMBACKEND */

