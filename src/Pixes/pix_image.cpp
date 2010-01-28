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
//    For information on usage and redistribution, and for a DISCLAIMER OF ALL
//    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.
//
/////////////////////////////////////////////////////////
#include "Base/GemConfig.h"
#include "pix_image.h"
#include "Base/GemState.h"

#ifdef _WIN32
# include <io.h>
# define close _close
# define snprintf _snprintf
#endif

#if defined(__unix__) || defined(__APPLE__) 
# include <unistd.h>
# include <strings.h>
#endif

#include <stdio.h>
#include "Base/GemCache.h"

CPPEXTERN_NEW_WITH_ONE_ARG(pix_image, t_symbol *, A_DEFSYM)

/////////////////////////////////////////////////////////
//
// pix_image
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
pix_image :: pix_image(t_symbol *filename) :
#ifdef HAVE_PTHREADS
  m_thread_id(0), m_mutex(NULL), m_thread_continue(false),
#endif
  m_thread_running(false), m_threadloaded(false), 
  m_loadedImage(NULL)
{
  int i=MAXPDSTRING;
  while(i--)m_filename[i]=0;

  m_pixBlock.image = m_imageStruct;
#ifdef HAVE_PTHREADS
  m_mutex = (pthread_mutex_t*) malloc(sizeof(pthread_mutex_t));
  if(m_mutex){
    if ( pthread_mutex_init(m_mutex, NULL) < 0 ) {
      free(m_mutex);
      m_mutex=NULL;
    } else 
      threadMess(1);
  }
#endif
  if(filename!=&s_)openMess(filename);

}

////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
pix_image :: ~pix_image()
{
  threadMess(0);
  cleanImage();

#ifdef HAVE_PTHREADS
  freebytes(m_mutex ,sizeof(pthread_mutex_t));
#endif
}

void pix_image :: threadMess(int onoff)
{
#ifdef HAVE_PTHREADS
  if(m_mutex){ /* if we don't have a mutex, we don't want threads either! */
    if(onoff){
      /* if we don't have a thread, create one */
      if(!m_thread_running){
        int err=0;
        m_thread_running=false;
        m_thread_continue=true;
       
        pthread_mutex_lock(m_mutex);

        if((err=pthread_create(&m_thread_id, 0, openThread, this)))
          {
            /* ack! thread creation failed! fall back to unthreaded loading */
            //post("couldn't create thread! %d", err);
          } else {
          // wait until the thread is up and running
          pthread_mutex_lock(m_mutex);
          pthread_mutex_unlock(m_mutex);
          // now m_thread_running has been set by the child thread */

          //post("created thread %x", m_thread_id);
          return;
        }
      }
    } else {
      /* if we have a thread, destroy it! */
      if(m_thread_running){
        int counter=0;
        m_thread_continue=false;
        pthread_join(m_thread_id, 0);
        while(m_thread_running)counter++;
      }
      m_thread_id=0;
    }
  }
#endif
}

/////////////////////////////////////////////////////////
// openMess
//
/////////////////////////////////////////////////////////
#ifdef HAVE_PTHREADS
void *pix_image :: openThread(void*you)
{
  pix_image *me=(pix_image*)you;
  pthread_mutex_t *mutex=me->m_mutex;
  imageStruct     *loadedImage=me->m_loadedImage;
  struct timeval timout;
  int i=MAXPDSTRING;
  char*orgfilename=new char[MAXPDSTRING];
  while(i--)orgfilename[i]=0;

  me->m_thread_running=true;
  
  // now that m_thread_running is set, we unlock the main-thread
  // the lock has been set outside
  pthread_mutex_unlock(mutex);

  while(me->m_thread_continue){

    if(me->m_threadloaded || (*me->m_filename==0)) {
      
      // nothing to do, so sleep a bit...
      timout.tv_sec = 0;
      timout.tv_usec=100;
      select(0,0,0,0,&timout);
    } else {
      /* make a backup-copy of the filename that is stored locally
       * before returning the data to Gem we check, whether the 
       * main thread still wants _this_ file to be opened
       */

      pthread_mutex_lock(mutex);
      strcpy(orgfilename, me->m_filename);
      pthread_mutex_unlock(mutex);

      //post("loading in thread %s", orgfilename);
      
      imageStruct*newloadedImage = image2mem(orgfilename);
      
      pthread_mutex_lock(mutex);
      if(newloadedImage) {
	if(loadedImage)delete loadedImage;
	loadedImage=newloadedImage;
      }

      if(!strcmp(orgfilename, me->m_filename))
        {
          /* ok, we got what we wanted!  */
          me->m_loadedImage=loadedImage;
          me->m_threadloaded=true;
        }
      pthread_mutex_unlock(mutex);
    }
  }

  if(me->m_loadedImage==loadedImage)
    me->m_loadedImage=NULL;

  if(loadedImage)delete loadedImage;  loadedImage=NULL;
  if(orgfilename)delete[]orgfilename; orgfilename=NULL;
  
  me->m_thread_running=false;
  return 0;
}
#endif


void pix_image :: openMess(t_symbol *filename)
{
  if(NULL==filename || NULL==filename->s_name || 0==*filename->s_name)return;
 
  std::string fn = findFile(filename->s_name);
  snprintf(m_filename, MAXPDSTRING, "%s", fn.c_str());

  m_threadloaded=false;

  if(m_thread_running)
    { 
      /* we have a thread for image-loading; delegate the hard stuff to it */
      return;
    }

  cleanImage();
  
  if ( !(m_loadedImage = image2mem(m_filename)) )
    {
      return;
    }
  m_loadedImage->copy2Image(&m_pixBlock.image);
  m_pixBlock.newimage = 1;
  post("loaded image: %s", m_filename);
}

/////////////////////////////////////////////////////////
// render
//
/////////////////////////////////////////////////////////
void pix_image :: render(GemState *state)
{
  // if we don't have an image, just return
  if (!m_loadedImage){
    return;
  }

#ifdef HAVE_PTHREADS
  if(m_thread_running){
    pthread_mutex_lock(m_mutex);
      
    if(m_threadloaded)
      {
        /* yep, the thread might have something for us */
        if(m_loadedImage){
          m_loadedImage->copy2Image(&m_pixBlock.image);
          m_pixBlock.newimage = 1;
          post("thread loaded image: %s", m_filename);
        }
        m_threadloaded=false;
        *m_filename=0;
          
        if (m_cache&&m_cache->resendImage){
          // we just copied it, so we don't want to resend the image...
          m_cache->resendImage=0;
        }
      }
  }
#endif
  // do we need to reload the image?    
  if (m_cache&&m_cache->resendImage)
    {
      m_loadedImage->refreshImage(&m_pixBlock.image);
      m_pixBlock.newimage = 1;
      m_cache->resendImage = 0;
    }
  state->image = &m_pixBlock;
}

/////////////////////////////////////////////////////////
// postrender
//
/////////////////////////////////////////////////////////
void pix_image :: postrender(GemState *state)
{
  m_pixBlock.newimage = 0;
  state->image = NULL;

#ifdef HAVE_PTHREADS
  if(m_mutex)
    pthread_mutex_unlock(m_mutex);
#endif /* HAVE_PTHREADS */
}

/////////////////////////////////////////////////////////
// startRendering
//
/////////////////////////////////////////////////////////
void pix_image :: startRendering()
{
  if (!m_loadedImage) return;
  m_loadedImage->refreshImage(&m_pixBlock.image);
  m_pixBlock.newimage = 1;
}

/////////////////////////////////////////////////////////
// cleanImage
//
/////////////////////////////////////////////////////////
void pix_image :: cleanImage()
{
  // release previous data
  if (m_loadedImage)
    {
      delete m_loadedImage;
      m_loadedImage = NULL;
      m_pixBlock.image.clear();
      m_pixBlock.image.data = NULL;
    }
}

/////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////
void pix_image :: obj_setupCallback(t_class *classPtr)
{
  class_addmethod(classPtr, reinterpret_cast<t_method>(&pix_image::openMessCallback),
                  gensym("open"), A_SYMBOL, A_NULL);
  class_addmethod(classPtr, reinterpret_cast<t_method>(&pix_image::threadMessCallback),
                  gensym("thread"), A_FLOAT, A_NULL);
}
void pix_image :: openMessCallback(void *data, t_symbol *filename)
{
  GetMyClass(data)->openMess(filename);
}
void pix_image :: threadMessCallback(void *data, t_floatarg f)
{
  GetMyClass(data)->threadMess(f>0);
}
