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
#ifdef _WINDOWS

#include "Pixes/pix_filmNT.h"

CPPEXTERN_NEW_WITH_ONE_ARG(pix_filmNT, t_symbol *, A_DEFSYM)

/////////////////////////////////////////////////////////
//
// pix_filmNT
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
pix_filmNT :: pix_filmNT(t_symbol *filename)  : 
  pix_film(filename)
{
  AVIFileInit();
  if (filename->s_name[0]) openMess(filename);
}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
pix_filmNT :: ~pix_filmNT()
{
  // Clean up the movie
  closeMess();
  deleteBuffer();

  outlet_free(m_outNumFrames);
  outlet_free(m_outEnd);


  AVIFileExit();
}

void pix_filmNT :: closeMess(void)
{
  switch (m_haveMovie) {
  case GEM_MOVIE_AVI:
    AVIStreamGetFrameClose(m_getFrame);
    m_haveMovie = GEM_MOVIE_NONE;
    m_getFrame = NULL;
    break;
  case GEM_MOVIE_NONE:
  default:
    break;
  }
}

/////////////////////////////////////////////////////////
// openMess
//
/////////////////////////////////////////////////////////
void pix_filmNT :: realOpen(char *filename)
{

	// Opens the AVI stream
	if (AVIStreamOpenFromFile(&m_streamVid, filename, streamtypeVIDEO, 0, OF_READ, NULL)) {
		error("GEM: pix_film: Unable to open file: %s", filename);
		return;
	}

  m_reqFrame = 0;
  m_curFrame = -1;

  // Create the PGETFRAME
  if (!(m_getFrame = AVIStreamGetFrameOpen(m_streamVid,NULL)))return;

  // get all of the information about the stream
  AVISTREAMINFO psi;
  if (AVIStreamInfo(m_streamVid, &psi, sizeof(AVISTREAMINFO)))return;

  m_haveMovie = GEM_MOVIE_AVI;
  // Get the length of the movie
  m_numFrames = psi.dwLength - 1;

  m_xsize = psi.rcFrame.right - psi.rcFrame.left;
  m_ysize = psi.rcFrame.bottom - psi.rcFrame.top;

  m_csize=3;
  m_format=GL_BGR_EXT;
}

/////////////////////////////////////////////////////////
// render
//
/////////////////////////////////////////////////////////
void pix_filmNT :: getFrame()
{
	unsigned char *pt=0;
  int i;
  switch (m_haveMovie) {
  case GEM_MOVIE_AVI:
    if (m_curFrame > m_numFrames)m_curFrame=m_numFrames;
    pt = (unsigned char *)AVIStreamGetFrame(m_getFrame, m_curFrame);
	m_frame = (pt+40);
    break;
  case GEM_MOVIE_NONE:
  default:
    break;
  }
}


/////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////
void pix_filmNT :: obj_setupCallback(t_class *classPtr)
{
  class_addcreator((t_newmethod)_classpix_filmNT, gensym("pix_film"), A_DEFSYM, A_NULL);
  pix_film::real_obj_setupCallback(classPtr);

  class_addmethod(classPtr, (t_method)&pix_filmNT::openMessCallback,
		  gensym("open"), A_SYMBOL, A_NULL);
  class_addmethod(classPtr, (t_method)&pix_filmNT::changeImageCallback,
		  gensym("img_num"), A_GIMME, A_NULL);
  class_addmethod(classPtr, (t_method)&pix_filmNT::autoCallback,
		  gensym("auto"), A_DEFFLOAT, A_NULL);
}
void pix_filmNT :: openMessCallback(void *data, t_symbol *filename)
{
  GetMyClass(data)->openMess(filename);
}
void pix_filmNT :: changeImageCallback(void *data, t_symbol *, int argc, t_atom *argv)
{
    GetMyClass(data)->changeImage((argc<1)?0:atom_getint(argv), (argc<2)?0:atom_getint(argv+1));
}

void pix_filmNT :: autoCallback(void *data, t_floatarg state)
{
  GetMyClass(data)->m_auto=!(!(int)state);
}
#endif // _WINDOWS
