    ////////////////////////////////////////////////////////
    //
// GEM - Graphics Environment for Multimedia
    //
// zmoelnig@iem.kug.ac.at
    //
// Implementation file
    //
//    Copyright (c) 1997-1999 Mark Danks.
//    Copyright (c) Gnther Geiger.
//    Copyright (c) 2001-2002 IOhannes m zmoelnig. forum::fr::uml�te. IEM
//    Copyright (c) 2002 James Tittle & Chris Clepper
//    For information on usage and redistribution, and for a DISCLAIMER OF ALL
//    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.
    //
    /////////////////////////////////////////////////////////

#include "pix_file_read.h"
#include <ctype.h>


#include <stdio.h>
#include <sys/time.h>
#include <string>


#if 1
# define debug
#else
# define debug post
#endif


    CPPEXTERN_NEW_WITH_ONE_ARG(pix_file_read, t_symbol *, A_DEFSYM)

// #ifdef HAVE_PTHREADS
//     /* the "capturing"-thread */
//     void *pix_file_read :: grabThread(void*you)
// {
//   pix_file_read *me=(pix_file_read*)you;
//   pthread_mutex_t *mutex=me->m_mutex;
//   struct timeval timout;
//   me->m_thread_running=true;
//   //me->post("using pthreads");
//   while(me->m_thread_continue){
//     int reqFrame=(int)me->m_reqFrame;
//     int reqTrack=(int)me->m_reqTrack;
//     timout.tv_sec = 0;
//     timout.tv_usec=100;
// 
//     if(reqFrame!=me->m_curFrame || reqTrack!=me->m_curTrack){
// 
//       pthread_mutex_lock(me->m_mutex);
// 
//       if (me->fileReader.setPosition(reqFrame, reqTrack)!=FILM_ERROR_FAILURE){
//         me->m_frame=me->fileReader.getFrame();
//       } else me->m_frame=0;
// 
//       me->m_curFrame=reqFrame;
//       me->m_curTrack=reqTrack;
// 
//       pthread_mutex_unlock(me->m_mutex);
//     }
//     select(0,0,0,0,&timout);
//   }
//   
//   me->m_thread_running=false;
//   return NULL;
// }
// #endif

/////////////////////////////////////////////////////////
//
// pix_file_read
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
pix_file_read :: pix_file_read(t_symbol *filename) :
    m_auto(0), m_format(GL_RGBA),
  m_numFrames(0), m_reqFrame(0), m_curFrame(0),
  m_numTracks(0), m_reqTrack(0), m_curTrack(0),
// #ifdef HAVE_PTHREADS
//   m_thread_id(0), m_mutex(NULL), m_frame(NULL), m_thread_continue(false),
//   m_thread_running(false), m_wantThread(true)
// #else
      m_thread_running(false), m_wantThread(false)
// #endif

{
  m_frame = new pixBlock();
  // setting the current frame
  inlet_new(this->x_obj, &this->x_obj->ob_pd, gensym("float"), gensym("img_num"));
  // create an outlet to send out how many frames are in the movie + bang when we reached the end
  m_outNumFrames = outlet_new(this->x_obj, 0);
  m_outEnd       = outlet_new(this->x_obj, 0);
  

  //openMess(filename);
}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
pix_file_read :: ~pix_file_read()
{
  // Clean up the movie
  closeMess();
}

/////////////////////////////////////////////////////////
// closeMess
//
/////////////////////////////////////////////////////////
void pix_file_read :: closeMess(void){

  fileReader->closeFile();
  delete m_frame;

}

/////////////////////////////////////////////////////////
// openMess
//
/////////////////////////////////////////////////////////

    void pix_file_read :: openMess(t_symbol *filename, int format, int codec)
{
  //  if (filename==x_filename)return;
  closeMess();
  
  char buff[MAXPDSTRING];
  char *buf=buff;
  // we first try to find the file-to-open with canvas_makefilename
  // if this fails, we just pass the given filename (could be a stream)
  canvas_makefilename(getCanvas(), filename->s_name, buff, MAXPDSTRING);
  
   if (FILE*fd=fopen(buff, "r"))fclose(fd);
    else buf=filename->s_name;
  
//  printf("----------------------- buff: %s --------------\n", buff);
  
//   printf("------------------------- before fileReader.openFile ------------------------\n");
  *fileReader = m_kernel.getFileReadServer().getPlugin();
    
  fileReader->openFile((string) buff);
//   printf("------------------------- after fileReader.openFile ------------------------\n");

  
  t_atom ap[4];
  SETFLOAT(ap, fileReader->getNrOfFrames());
  SETFLOAT(ap+1, fileReader->getWidth());
  SETFLOAT(ap+2, fileReader->getHeight());
  SETFLOAT(ap+3, (float)fileReader->getFPS());
  m_numFrames=fileReader->getNrOfFrames();
//   printf("------------------------- after fileReader get functions ------------------------\n");
  
   post("loaded file: %s with %d frames (%dx%d) at %f fps", 
      buff, 
      fileReader->getNrOfFrames(), 
      fileReader->getWidth(), 
      fileReader->getHeight(), (float)fileReader->getFPS());
  outlet_list(m_outNumFrames, 0, 4, ap);
  


}

/////////////////////////////////////////////////////////
// render
//
/////////////////////////////////////////////////////////
void pix_file_read :: render(GemState *state)
{
  int frame=-1;
  /* get the current frame from the file */
  if (fileReader->hasVideo() == false)
    return;

// #ifdef HAVE_PTHREADS
//   if(m_thread_running) {
//     pthread_mutex_lock(m_mutex);
//     state->image=m_frame;
//   } else
// #endif /* PTHREADS */
    
   
   printf("------------------------- before getFrameData ------------------------\n");
   
   m_frame->image.data = fileReader->getFrameData();
    
   printf("------------------------- after getFrameData ------------------------\n");
   
    //state->image= (pixBlock *) fileReader.getFrame();
    state->image = m_frame;
    printf("------------------------- state->image = m_frame ------------------------\n");
    fileReader->setAutoIncrement((float)m_auto);
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
          fileReader->setPosition((int)m_reqFrame, m_reqTrack);
        }
      
        //state->image= (pixBlock *) fileReader.getFrame();
        m_frame->image.data = fileReader->getFrameData();
        state->image = m_frame;
      }
    }
}

/////////////////////////////////////////////////////////
// postrender
//
/////////////////////////////////////////////////////////
void pix_file_read :: postrender(GemState *state)
{
  if(fileReader->hasVideo() == false)
    return;
  if (state && state->image)
    state->image->newimage = 0;

// #ifdef HAVE_PTHREADS
//   if(m_thread_running){
//     pthread_mutex_unlock(m_mutex);
//   }
// #endif /* PTHREADS */

  // automatic proceeding
  if (m_auto!=0){
    if(m_thread_running){
      m_reqFrame+=m_auto;
    } else
      if (fileReader->setPosition((int)(m_reqFrame+=m_auto))==FILM_ERROR_FAILURE){
        //      m_reqFrame = m_numFrames;
        outlet_bang(m_outEnd);
      }
  }
}

/////////////////////////////////////////////////////////
// setPosition
//
/////////////////////////////////////////////////////////
void pix_file_read :: changeImage(int imgNum, int trackNum)
{
  if (imgNum < 0){
    error("GEM: pix_file_read: selection number must be > 0");
    imgNum=0;
  }
  if (trackNum < 0){
    error("GEM: pix_file_read: track number must be > 0");
    trackNum=0;
  }
  if (fileReader->hasVideo()){
    if(!m_thread_running){
      if (fileReader->setPosition(imgNum, trackNum)==FILM_ERROR_FAILURE){
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
void pix_file_read :: csMess(t_symbol *s, bool immediately)
{
//   char c =*s->s_name;
//   switch (c){
//     case 'g': case 'G': fileReader.setMFormat(GL_LUMINANCE, immediately); break;
//     case 'y': case 'Y': fileReader.setMFormat(GL_YCBCR_422_GEM, immediately); break;
//     case 'r': case 'R': 
//       if(gensym("RGB")==s||gensym("rgb")==s)
//         fileReader.setMFormat(GL_RGB, immediately); 
//       else
//         fileReader.setMFormat(GL_RGBA, immediately); 
//       break;
//     default:
//       error("colorspace must be 'RGBA', 'YUV' or 'Gray'");
//  }

}
/////////////////////////////////////////////////////////
// threading
//
/////////////////////////////////////////////////////////
void pix_file_read :: threadMess(int state)
{
  m_wantThread=!(!state);
// #ifdef HAVE_PTHREADS
//   post("thread settings will have an effect on next open!");
// #else
  post("no thread support");
// #endif
}


/////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////
void pix_file_read :: obj_setupCallback(t_class *classPtr)
{
  class_addcreator((t_newmethod)_classpix_file_read, gensym("pix_file_read"), A_DEFSYM, A_NULL);

  class_addmethod(classPtr, (t_method)&pix_file_read::openMessCallback,
                  gensym("open"), A_GIMME, A_NULL);
  class_addmethod(classPtr, (t_method)&pix_file_read::changeImageCallback,
                  gensym("img_num"), A_GIMME, A_NULL);
  class_addmethod(classPtr, (t_method)&pix_file_read::autoCallback,
                  gensym("auto"), A_DEFFLOAT, A_NULL);
  class_addmethod(classPtr, (t_method)&pix_file_read::csCallback,
                  gensym("colorspace"), A_DEFSYM, A_NULL);
  class_addmethod(classPtr, (t_method)&pix_file_read::threadCallback,
                  gensym("thread"), A_FLOAT, A_NULL);
}
void pix_file_read :: openMessCallback(void *data, t_symbol*s,int argc, t_atom*argv)
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

void pix_file_read :: changeImageCallback(void *data, t_symbol *, int argc, t_atom *argv){
  GetMyClass(data)->changeImage((argc<1)?0:atom_getint(argv), (argc<2)?0:atom_getint(argv+1));
}

void pix_file_read :: autoCallback(void *data, t_floatarg state)
{
  GetMyClass(data)->m_auto=state;
}
void pix_file_read :: csCallback(void *data, t_symbol*s)
{
  GetMyClass(data)->csMess(s);
}

void pix_file_read :: threadCallback(void *data, t_floatarg state)
{
  GetMyClass(data)->threadMess((int)state);
}

