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
#include "Pixes/filmAVI.h"

/////////////////////////////////////////////////////////
//
// filmAVI
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////

filmAVI :: filmAVI(int format) : film(format) {
  static bool first_time=true;
  if (first_time) {
#ifdef _WINDOWS
    post("pix_film:: avi support");
#endif
    first_time = false;
  }

#ifdef _WINDOWS
  m_getFrame=NULL;
  AVIFileInit();
#endif
}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
filmAVI :: ~filmAVI()
{
  close();
#ifdef _WINDOWS
  AVIFileExit();
#endif
}

#ifdef _WINDOWS
void filmAVI :: close(void)
{
    if(m_getFrame)AVIStreamGetFrameClose(m_getFrame);
    m_getFrame=NULL;
}

/////////////////////////////////////////////////////////
// open the file
//
/////////////////////////////////////////////////////////
bool filmAVI :: open(char *filename, int format)
{
  if (format>0)m_wantedFormat=format;
  if (AVIStreamOpenFromFile(&m_streamVid, filename, streamtypeVIDEO, 0, OF_READ, NULL)) {
    error("GEM: pix_film: Unable to open file: %s", filename);
    goto unsupported;
  }
  
  m_curFrame = -1;

  // Create the PGETFRAME
  if (!(m_getFrame = AVIStreamGetFrameOpen(m_streamVid,NULL)))return false;

  // get all of the information about the stream
  AVISTREAMINFO psi;
  if (AVIStreamInfo(m_streamVid, &psi, sizeof(AVISTREAMINFO)))return false;

  // Get the length of the movie
  m_numFrames = psi.dwLength - 1;

  m_image.image.xsize = psi.rcFrame.right - psi.rcFrame.left;
  m_image.image.ysize = psi.rcFrame.bottom - psi.rcFrame.top;

  m_image.image.csize=3;
  m_image.image.format=GL_BGR_EXT;
  m_image.image.reallocate();
  return true;
 unsupported:
  post("AVI: unsupported!");
  close();
  return false;
}

/////////////////////////////////////////////////////////
// render
//
/////////////////////////////////////////////////////////
pixBlock* filmAVI :: getFrame(){
  m_image.image.data = (unsigned char *)AVIStreamGetFrame(m_getFrame, m_curFrame)+40;
  return 0;
}

int filmAVI :: changeImage(int imgNum, int trackNum){
  m_curFrame=imgNum;
  return FILM_ERROR_SUCCESS;
}
#endif
