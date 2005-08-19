////////////////////////////////////////////////////////
//
// GEM - Graphics Environment for Multimedia
//
// zmoelnig@iem.kug.ac.at
//
// Implementation file 
//
//    Copyright (c) 1997-1999 Mark Danks.
//    Copyright (c) G�nther Geiger.
//    Copyright (c) 2001-2002 IOhannes m zmoelnig. forum::f�r::uml�ute. IEM
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
#ifdef __WIN32__
    post("pix_film:: avi support");
#endif
    first_time = false;
  }

#ifdef __WIN32__
  m_getFrame     = NULL;
  m_streamVid    = NULL;
  m_pbmihRaw     = NULL;
  m_pbmihDst     = NULL;
  m_hic          = NULL;
  m_RawBuffer    = NULL;
  m_frame        = NULL;
  m_nRawBuffSize = 0;
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
#ifdef __WIN32__
  AVIFileExit();
#endif
}

#ifdef __WIN32__
void filmAVI :: close(void)
{
  if (m_streamVid){
    AVIStreamRelease(m_streamVid);
    m_streamVid = NULL;
  }

  if (m_pbmihRaw){
    delete[] m_pbmihRaw;
    m_pbmihRaw = NULL;
  }

  if (m_pbmihDst){
    delete[] m_pbmihDst;
    m_pbmihDst = NULL;
  }

  if (m_hic)	{
    ICDecompressEnd(m_hic);
    ICClose(m_hic);
    m_hic = NULL;
  }

  if (m_frame){
    delete[]m_frame;
    m_frame = NULL;
  }
  if (m_RawBuffer){
    delete[] m_RawBuffer;
    m_RawBuffer = NULL;
  }    
  m_nRawBuffSize = 0;
}

/////////////////////////////////////////////////////////
// open the file
//
/////////////////////////////////////////////////////////
bool filmAVI :: open(char *filename, int format)
{
  AVISTREAMINFO streaminfo;
  long lSize = 0; // in bytes

  if (format>0)m_wantedFormat=format;
  if (AVIStreamOpenFromFile(&m_streamVid, filename, streamtypeVIDEO, 0, OF_READ, NULL)) {
    //error("GEM: pix_film: Unable to open file: %s", filename);
    goto unsupported;
  }

  if( AVIStreamInfo( m_streamVid, &streaminfo, sizeof(streaminfo)) ||
      AVIStreamReadFormat(m_streamVid, AVIStreamStart(m_streamVid), NULL, &lSize))  {
    //error("GEM: pix_film: Unable to read file format: %s", filename);
    goto unsupported;
  }

  m_pbmihRaw = (BITMAPINFOHEADER*) new char[lSize];

  if(AVIStreamReadFormat(m_streamVid, AVIStreamStart(m_streamVid), m_pbmihRaw, &lSize))	{
    //error("GEM: pix_film: Unable to read file format: %s", filename);
    goto unsupported;
  }
  if ((8 == m_pbmihRaw->biBitCount)
      || ((40 == m_pbmihRaw->biBitCount) && (mmioFOURCC('c','v','i','d') == m_pbmihRaw->biCompression))) {
    // HACK: attempt to decompress 8 bit films or BW cinepak films to greyscale
    m_pbmihDst = (BITMAPINFOHEADER*) new char[sizeof(BITMAPINFOHEADER) + 256*3];
    //post("GEM: pix_film: Loading as greyscale");

    *m_pbmihDst = *m_pbmihRaw;
    m_pbmihDst->biSize = sizeof(BITMAPINFOHEADER);

    m_format = GL_LUMINANCE;
			
    m_pbmihDst->biBitCount			= 8;
    m_pbmihDst->biClrUsed			= 256;
    m_pbmihDst->biClrImportant		= 256;

    char* pClrPtr = ((char*)m_pbmihDst) + sizeof(BITMAPINFOHEADER);
    for (int i = 0; i < 256; i++){
      *pClrPtr++ = i;
      *pClrPtr++ = i;
      *pClrPtr++ = i;
      }
  } else {
    m_pbmihDst = (BITMAPINFOHEADER*) new char[sizeof(BITMAPINFOHEADER)];
    *m_pbmihDst = *m_pbmihRaw;

    m_format = GL_BGR_EXT;
			
    m_pbmihDst->biBitCount	= 24;
    m_pbmihDst->biClrUsed	= 0;
    m_pbmihDst->biClrImportant	= 0;
  }

  m_pbmihDst->biCompression		= BI_RGB;
  m_pbmihDst->biSizeImage			= 0;
    
  // Get the length of the movie
  m_numFrames = streaminfo.dwLength - 1;

  m_image.image.xsize = streaminfo.rcFrame.right - streaminfo.rcFrame.left;
  m_image.image.ysize = streaminfo.rcFrame.bottom - streaminfo.rcFrame.top;
  m_image.image.setCsizeByFormat(m_wantedFormat);
  m_image.image.reallocate();

  if (!(m_hic = ICLocate(ICTYPE_VIDEO, 0, m_pbmihRaw, m_pbmihDst, ICMODE_DECOMPRESS))){
    //error("GEM: pix_film: Could not find decompressor: %s", filename);
    goto unsupported;
  }
  if (m_format==GL_LUMINANCE){
    if (ICERR_OK != ICDecompressSetPalette(m_hic, m_pbmihDst)){
      error("GEM: pix_film: Could not set palette: %s", filename);
    }
  }

  if (ICERR_OK != ICDecompressBegin(m_hic, m_pbmihRaw, m_pbmihDst)){
    //error("GEM: pix_film: Could not begin decompression: %s", filename);
    goto unsupported;
  }
  //if (!m_pbmihRaw->biSizeImage)
  //	m_pbmihRaw->biSizeImage = m_xsize * m_ysize * m_csize;
  //m_nRawBuffSize = min(streaminfo.dwSuggestedBufferSize, m_pbmihRaw->biSizeImage);
  m_nRawBuffSize = max(streaminfo.dwSuggestedBufferSize, m_pbmihRaw->biSizeImage);
  if(!m_nRawBuffSize)m_nRawBuffSize = m_image.image.xsize * m_image.image.ysize * 3;
  
  m_RawBuffer = new unsigned char[m_nRawBuffSize];
  m_frame     = new unsigned char[m_nRawBuffSize];
  m_reqFrame = 0;
  m_curFrame = -1;
  return true;

 unsupported:
  startpost("AVI failed");
  close();
  return false;
}

/////////////////////////////////////////////////////////
// render
//
/////////////////////////////////////////////////////////
pixBlock* filmAVI :: getFrame(){
  BITMAPINFOHEADER* pbmih;
  long lBytesWritten;
  unsigned char*data=NULL;
  if (m_reqFrame > m_numFrames)m_reqFrame=m_numFrames;

  m_image.newimage=1;
  m_image.image.upsidedown=false;
  m_image.image.setCsizeByFormat(m_wantedFormat);
  m_image.image.reallocate();

  if (!AVIStreamRead(m_streamVid, 
		     m_reqFrame, 1, 
		     m_RawBuffer, m_nRawBuffSize,
		     &lBytesWritten, 0)){
    m_pbmihRaw->biSize = lBytesWritten;
    ICDecompress(m_hic, 0, m_pbmihRaw, m_RawBuffer, m_pbmihDst, m_frame);
    data=m_frame;
  } else data=(unsigned char *)AVIStreamGetFrame(m_getFrame, m_curFrame)+40;
  if(!data)return 0;

  switch(m_format){
  case GL_LUMINANCE: m_image.image.fromGray(data); break;
  default:  m_image.image.fromBGR(data);
  }
  return &m_image;
}

int filmAVI :: changeImage(int imgNum, int trackNum){
  if (imgNum<0)return FILM_ERROR_FAILURE;
  if (m_numFrames<0){
    m_reqFrame = imgNum;
    return FILM_ERROR_SUCCESS;
  }
  if (imgNum>m_numFrames){
    m_reqFrame=m_numFrames;
    return FILM_ERROR_FAILURE;
  }

  m_reqFrame = imgNum;
  return FILM_ERROR_SUCCESS;
}
#endif
