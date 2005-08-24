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

#define HELPSYMBOL "pix_film"

#include "pix_filmNEW.h"
#include <ctype.h>

#include "Pixes/filmQT4L.h"
#include "Pixes/filmAVI.h"
#include "Pixes/filmAVIPLAY.h"
#include "Pixes/filmFFMPEG.h"
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
 * FFMPEG: linux only (although there are ports to w32/osX (?))
 *         should decode pretty much
 *         lacks seeking support for several formats (experienced problems with mjpeg MOVs)
 *         (still) likes to crash
 *         sometimes problems with decoding when keyframe cannot be found (eg: SVQ3)
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


#if 0
# define debug post
#else
# define debug
#endif


CPPEXTERN_NEW_WITH_ONE_ARG(pix_filmNEW, t_symbol *, A_DEFSYM)

/////////////////////////////////////////////////////////
//
// pix_filmNEW
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
pix_filmNEW :: pix_filmNEW(t_symbol *filename) :
  m_auto(0), 
  m_numFrames(0), m_reqFrame(0), m_curFrame(0),
  m_numTracks(0), m_track(0), 
  m_format(GL_RGBA)
{
  debug("pix_filmNEW");
  // setting the current frame
  inlet_new(this->x_obj, &this->x_obj->ob_pd, gensym("float"), gensym("img_num"));
  // create an outlet to send out how many frames are in the movie + bang when we reached the end
  m_outNumFrames = outlet_new(this->x_obj, 0);
  m_outEnd       = outlet_new(this->x_obj, 0);
  
  m_handle=0;
  int i=MAX_FILM_HANDLES;
  while(i--)m_handles[i]=0;
  m_numHandles=0;

  m_handles[m_numHandles]=new filmAVI();    debug("handle %d\t%X", m_numHandles, m_handles[m_numHandles]);m_numHandles++;
  m_handles[m_numHandles]=new filmQT();    debug("handle %d\t%X", m_numHandles, m_handles[m_numHandles]);m_numHandles++;
  m_handles[m_numHandles]=new filmQT4L();    debug("handle %d\t%X", m_numHandles, m_handles[m_numHandles]);m_numHandles++;
  m_handles[m_numHandles]=new filmMPEG3();    debug("handle %d\t%X", m_numHandles, m_handles[m_numHandles]);m_numHandles++;
  m_handles[m_numHandles]=new filmAVIPLAY();  debug("handle %d\t%X", m_numHandles, m_handles[m_numHandles]);m_numHandles++;
  m_handles[m_numHandles]=new filmFFMPEG();   debug("handle %d\t%X", m_numHandles, m_handles[m_numHandles]);m_numHandles++;
  m_handles[m_numHandles]=new filmMPEG1();    debug("handle %d\t%X", m_numHandles, m_handles[m_numHandles]);m_numHandles++;

  //openMess(filename);
}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
pix_filmNEW :: ~pix_filmNEW()
{
  // Clean up the movie
  closeMess();
}

/////////////////////////////////////////////////////////
// closeMess
//
/////////////////////////////////////////////////////////
void pix_filmNEW :: closeMess(void){
  // Clean up any open files
  int i=MAX_FILM_HANDLES;
  debug("closing %d handles", i);
  while(i--){
    debug("close %d", i);
    if(m_handles[i])m_handles[i]->close();
  }
  m_handle=NULL;
  //if(m_handle!=0)m_handle->close();
  debug("closed");
}

/////////////////////////////////////////////////////////
// openMess
//
/////////////////////////////////////////////////////////

void pix_filmNEW :: openMess(t_symbol *filename, int format, int codec)
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
    //post("opening %s with format %X", buf, format);
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
    error("GEM: pix_film: Unable to open file: %s", filename->s_name);
	return;
  }

  t_atom ap[4];
  SETFLOAT(ap, m_handle->getFrameNum());
  SETFLOAT(ap+1, m_handle->getWidth());
  SETFLOAT(ap+2, m_handle->getHeight());
  SETFLOAT(ap+3, m_handle->getFPS());
  m_numFrames=m_handle->getFrameNum();
  post("GEM: pix_film: Loaded file: %s with %d frames (%dx%d) at %f fps", 
       buf, 
       m_handle->getFrameNum(), 
       m_handle->getWidth(), 
       m_handle->getHeight(), m_handle->getFPS());
  outlet_list(m_outNumFrames, 0, 4, ap);
}

/////////////////////////////////////////////////////////
// render
//
/////////////////////////////////////////////////////////
void pix_filmNEW :: render(GemState *state)
{
  /* get the current frame from the file */
  if (!m_handle)return;
  state->image=m_handle->getFrame();
  int frame=(int)m_reqFrame;
  if (state->image==0){
    outlet_float(m_outEnd,(m_numFrames>0 && (int)m_reqFrame<0)?(m_numFrames-1):0);
  }
  if (frame!=(int)m_reqFrame)render(state);//state->image=m_handle->getFrame();
}

/////////////////////////////////////////////////////////
// postrender
//
/////////////////////////////////////////////////////////
void pix_filmNEW :: postrender(GemState *state)
{
  if (state && state->image)state->image->newimage = 0;
  // automatic proceeding
  if (m_auto!=0){
    if (m_handle&&m_handle->changeImage((int)(m_reqFrame+=m_auto))==FILM_ERROR_FAILURE){
      //      m_reqFrame = m_numFrames;
      outlet_bang(m_outEnd);
    }
  }
}

/////////////////////////////////////////////////////////
// changeImage
//
/////////////////////////////////////////////////////////
void pix_filmNEW :: changeImage(int imgNum, int trackNum)
{
  if (imgNum < 0){
    error("GEM: pix_filmNEW: selection number must be > 0");
    imgNum=0;
  }
  if (trackNum < 0){
    error("GEM: pix_filmNEW: track number must be > 0");
    trackNum=0;
  }
  if (m_handle)
    if (m_handle->changeImage(imgNum, trackNum)==FILM_ERROR_FAILURE){
      outlet_bang(m_outEnd);
    }
  m_reqFrame=imgNum;
}
/////////////////////////////////////////////////////////
// colorSpace
//
/////////////////////////////////////////////////////////
void pix_filmNEW :: csMess(t_symbol *s, bool immediately)
{
  int format=0;
  char c =*s->s_name;
  switch (c){
  case 'g': case 'G': m_format=GL_LUMINANCE; break;
  case 'y': case 'Y': m_format=GL_YCBCR_422_GEM; break;
  case 'r': case 'R': m_format=GL_RGBA; break;
  default:
    error("pix_film: colorspace must be 'RGBA', 'YUV' or 'Gray'");
  }
  if(immediately && m_handle)m_handle->requestColor(m_format);
}
/////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////
void pix_filmNEW :: obj_setupCallback(t_class *classPtr)
{
  class_addcreator((t_newmethod)_classpix_filmNEW, gensym("pix_film"), A_DEFSYM, A_NULL);
  class_addcreator((t_newmethod)_classpix_filmNEW, gensym("pix_filmQT"), A_DEFSYM, A_NULL);

  class_addmethod(classPtr, (t_method)&pix_filmNEW::openMessCallback,
		  gensym("open"), A_GIMME, A_NULL);
  class_addmethod(classPtr, (t_method)&pix_filmNEW::changeImageCallback,
		  gensym("img_num"), A_GIMME, A_NULL);
  class_addmethod(classPtr, (t_method)&pix_filmNEW::autoCallback,
		  gensym("auto"), A_DEFFLOAT, A_NULL);
  class_addmethod(classPtr, (t_method)&pix_filmNEW::csCallback,
		  gensym("colorspace"), A_DEFSYM, A_NULL);
}
void pix_filmNEW :: openMessCallback(void *data, t_symbol*s,int argc, t_atom*argv)
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
  error("pix_film: \"open <filename> [<format>] [<preferred codec#>]\"");
  return;
  
}

void pix_filmNEW :: changeImageCallback(void *data, t_symbol *, int argc, t_atom *argv){
    GetMyClass(data)->changeImage((argc<1)?0:atom_getint(argv), (argc<2)?0:atom_getint(argv+1));
}

void pix_filmNEW :: autoCallback(void *data, t_floatarg state)
{
  GetMyClass(data)->m_auto=state;
}
void pix_filmNEW :: csCallback(void *data, t_symbol*s)
{
  GetMyClass(data)->csMess(s);
}
