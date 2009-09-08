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
#ifndef __APPLE__

#include "pix_film.h"
#include <ctype.h>

#include "Pixes/filmGMERLIN.h"
#include "Pixes/filmQT4L.h"
#include "Pixes/filmAVI.h"
#include "Pixes/filmDS.h"
#include "Pixes/filmAVIPLAY.h"
#include "Pixes/filmMPEG1.h"
#include "Pixes/filmMPEG3.h"
#include "Pixes/filmQT.h"
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
 *          FFMPEG support is better than the "native" one (see above)
 *             the keyframe-problem i had with the LOTR-trailer (SVQ3) did not appear 
 *             here, even though FFMPEG was used
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
 * AVI, QT, QT4L, MPEG3, AVIPLAY, FFMPEG, MPEG1
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
 *  few movies will be devoded by QT4L (but those are OK: seekable, stable)
 *  mpegs will be decoded by MPEG3 in a stable way
 *  everything else should be handled by AVIPLAY or FFMPEG (aviplay including ffmpeg)
 *  if there is no MPEG3 available at compile time, we have MPEG1 as a last fallback (unstable)
 *
 ***************************************/


#if 1
# define debug
#else
# define debug post
#endif


CPPEXTERN_NEW_WITH_ONE_ARG(pix_film, t_symbol *, A_DEFSYM)

#ifdef HAVE_PTHREADS
/* the "capturing"-thread */
void *pix_film :: grabThread(void*you)
{
  pix_film *me=(pix_film*)you;
  pthread_mutex_t *mutex=me->m_mutex;
  struct timeval timout;
  me->m_thread_running=true;
  //me->post("using pthreads");
  while(me->m_thread_continue){
    int reqFrame=(int)me->m_reqFrame;
    int reqTrack=(int)me->m_reqTrack;
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
  // setting the current frame
  inlet_new(this->x_obj, &this->x_obj->ob_pd, gensym("float"), gensym("img_num"));
  // create an outlet to send out how many frames are in the movie + bang when we reached the end
  m_outNumFrames = outlet_new(this->x_obj, 0);
  m_outEnd       = outlet_new(this->x_obj, 0);
  
  m_handle=0;
  int i=MAX_FILM_HANDLES;
  while(i--)m_handles[i]=0;
  m_numHandles=0;

#define DEBUG_HANDLE verbose(2, "handle %d\t%X", m_numHandles, m_handles[m_numHandles])
#if defined(__WIN32__) && defined(HAVE_DIRECTSHOW)
	m_handles[m_numHandles]=new filmDS();      DEBUG_HANDLE; m_numHandles++;
#else
  m_handles[m_numHandles]=new filmAVI();      DEBUG_HANDLE; m_numHandles++;
#endif
  m_handles[m_numHandles]=new filmGMERLIN();  DEBUG_HANDLE; m_numHandles++;
  m_handles[m_numHandles]=new filmQT();       DEBUG_HANDLE; m_numHandles++;
  m_handles[m_numHandles]=new filmQT4L();     DEBUG_HANDLE; m_numHandles++;
  m_handles[m_numHandles]=new filmMPEG3();    DEBUG_HANDLE; m_numHandles++;
  m_handles[m_numHandles]=new filmAVIPLAY();  DEBUG_HANDLE; m_numHandles++;
  m_handles[m_numHandles]=new filmMPEG1();    DEBUG_HANDLE; m_numHandles++;

  //openMess(filename);
}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
pix_film :: ~pix_film()
{
  // Clean up the movie
  closeMess();
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
  int i=MAX_FILM_HANDLES;
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
  canvas_makefilename(getCanvas(), filename->s_name, buff, MAXPDSTRING);
  if (FILE*fd=fopen(buff, "r"))fclose(fd);
  else buf=filename->s_name;
  m_handle=0;

  if (format==0)format=m_format;
  if(codec>=0){
    codec=codec%m_numHandles;
    if (m_handles[codec] && m_handles[codec]->open(buf, format ))
      m_handle = m_handles[codec];
  }
  if(!m_handle && m_handles){
    int i=-1;
    post("opening %s with format %X", buf, format);
    while(i++<m_numHandles){
      debug("trying handle %d: %X", i, m_handles[i]);
      if (m_handles[i] && m_handles[i]->open(buf, format ))      {
        m_handle = m_handles[i];
        break;
      }
      post(" ... ");
    }
  }

  debug("got handle = %X", m_handle);

  if (!m_handle){
    post(" ... giving up!");
    error("unable to open file: %s", filename->s_name);
	return;
  }

  t_atom ap[4];
  SETFLOAT(ap, m_handle->getFrameNum());
  SETFLOAT(ap+1, m_handle->getWidth());
  SETFLOAT(ap+2, m_handle->getHeight());
  SETFLOAT(ap+3, (float)m_handle->getFPS());
  m_numFrames=m_handle->getFrameNum();
  post("loaded file: %s with %d frames (%dx%d) at %f fps", 
       buf, 
       m_handle->getFrameNum(), 
       m_handle->getWidth(), 
       m_handle->getHeight(), (float)m_handle->getFPS());
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
    state->image=m_frame;
  } else
#endif /* PTHREADS */
    state->image=m_handle->getFrame();
  m_handle->setAuto(m_auto);
  frame=(int)m_reqFrame;
  if (NULL==state->image){
    outlet_float(m_outEnd,(m_numFrames>0 && (int)m_reqFrame<0)?(m_numFrames-1):0);

    if(frame!=(int)m_reqFrame){
      // someone responded immediately to the outlet_float and changed the requested frame
      // so get the newly requested frame:

      if(m_thread_running){
	/* if we are threaded (currently locked!), we change the frame# and grab the frame immediately
	 * (if we are not threaded, the frame# is already changed and the grabbing is always immediately)
	 */
	m_handle->changeImage((int)m_reqFrame, m_reqTrack);
      }
      
      state->image=m_handle->getFrame();
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
  if (state && state->image)state->image->newimage = 0;

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
      if (m_handle->changeImage((int)(m_reqFrame+=m_auto))==FILM_ERROR_FAILURE){
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
  if (trackNum < 0){
    error("track number must be > 0");
    trackNum=0;
  }
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
#ifdef __GNUC__
# warning the class_addcreator gets called twice: once by pix_film, once by pix_movie
  /* but really: Pd shouldn't bail out,
   * if a selector is bound to the same method of a class a 2nd time
   */
#endif
  class_addcreator((t_newmethod)create_pix_film, gensym("pix_filmQT"), A_DEFSYM, A_NULL);
 
  class_addmethod(classPtr, (t_method)&pix_film::openMessCallback,
		  gensym("open"), A_GIMME, A_NULL);
  class_addmethod(classPtr, (t_method)&pix_film::changeImageCallback,
		  gensym("img_num"), A_GIMME, A_NULL);
  class_addmethod(classPtr, (t_method)&pix_film::autoCallback,
		  gensym("auto"), A_DEFFLOAT, A_NULL);
  class_addmethod(classPtr, (t_method)&pix_film::csCallback,
		  gensym("colorspace"), A_DEFSYM, A_NULL);
  class_addmethod(classPtr, (t_method)&pix_film::threadCallback,
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
  GetMyClass(data)->threadMess((int)state);
}
#endif /*__APPLE__*/

