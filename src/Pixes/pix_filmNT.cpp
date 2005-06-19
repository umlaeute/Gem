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
#ifdef __WIN32__

#include "Pixes/pix_filmNT.h"
#include <memory>

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
  pix_film(filename),
  m_streamVid(NULL),
  m_hic(NULL),
  m_pbmihRaw(NULL),
  m_pbmihDst(NULL),
  m_RawBuffer(NULL),
  m_nRawBuffSize(0)
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
    m_haveMovie = GEM_MOVIE_NONE;

	if (m_streamVid)
	{
		AVIStreamRelease(m_streamVid);
		m_streamVid = NULL;
	}

	if (m_pbmihRaw)
	{
		delete[] m_pbmihRaw;
		m_pbmihRaw = NULL;
	}

	if (m_pbmihDst)
	{
		delete[] m_pbmihDst;
		m_pbmihDst = NULL;
	}

	if (m_hic)
	{
		ICDecompressEnd(m_hic);
		ICClose(m_hic);
		m_hic = NULL;
	}

	if (m_RawBuffer)
	{
		delete[] m_RawBuffer;
		m_RawBuffer = NULL;
		m_nRawBuffSize = 0;
	}    
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
	m_format = m_colorspace;
  // Opens the AVI stream
  if (AVIStreamOpenFromFile(&m_streamVid, filename, streamtypeVIDEO, 0, OF_READ, NULL)) {
    error("GEM: pix_film: Unable to open file: %s", filename);
    return;
  }

  AVISTREAMINFO streaminfo;
  long lSize = 0; // in bytes

  if( AVIStreamInfo( m_streamVid, &streaminfo, sizeof(streaminfo)) ||
	  AVIStreamReadFormat(m_streamVid, AVIStreamStart(m_streamVid), NULL, &lSize))
  {
	error("GEM: pix_film: Unable to read file format: %s", filename);
  } else
  {
	m_pbmihRaw = (BITMAPINFOHEADER*) new char[lSize];

	if(AVIStreamReadFormat(m_streamVid, AVIStreamStart(m_streamVid), m_pbmihRaw, &lSize))
	{
		error("GEM: pix_film: Unable to read file format: %s", filename);
	} else
	{
		if ((8 == m_pbmihRaw->biBitCount)
			|| ((40 == m_pbmihRaw->biBitCount) && (mmioFOURCC('c','v','i','d') == m_pbmihRaw->biCompression)))
		{	// HACK: attempt to decompress 8 bit films or BW cinepak films to greyscale
			m_pbmihDst = (BITMAPINFOHEADER*) new char[sizeof(BITMAPINFOHEADER) + 256*3];
			
			post("GEM: pix_film: Loading as greyscale");

			*m_pbmihDst = *m_pbmihRaw;
			m_pbmihDst->biSize = sizeof(BITMAPINFOHEADER);

			m_decodedFrame.setCsizeByFormat(GL_LUMINANCE);
			
			m_pbmihDst->biBitCount			= 8;
			m_pbmihDst->biClrUsed			= 256;
			m_pbmihDst->biClrImportant		= 256;

			char* pClrPtr = ((char*)m_pbmihDst) + sizeof(BITMAPINFOHEADER);
			for (int i = 0; i < 256; i++)
			{
				*pClrPtr++ = i;
				*pClrPtr++ = i;
				*pClrPtr++ = i;
			}
		} else
		{
			m_pbmihDst = (BITMAPINFOHEADER*) new char[sizeof(BITMAPINFOHEADER)];

			*m_pbmihDst = *m_pbmihRaw;

			m_decodedFrame.setCsizeByFormat(GL_BGR_EXT);
			
			m_pbmihDst->biBitCount			= 24;
			m_pbmihDst->biClrUsed			= 0;
			m_pbmihDst->biClrImportant		= 0;
		}

		m_pbmihDst->biCompression		= BI_RGB;
		m_pbmihDst->biSizeImage			= 0;

		// Get the length of the movie
		m_numFrames = streaminfo.dwLength - 1;

		m_xsize = streaminfo.rcFrame.right - streaminfo.rcFrame.left;
		m_ysize = streaminfo.rcFrame.bottom - streaminfo.rcFrame.top;

		m_decodedFrame.xsize=m_xsize;
		m_decodedFrame.ysize=m_ysize;
		m_decodedFrame.reallocate();

		m_gemFrame.xsize=m_xsize;
		m_gemFrame.ysize=m_ysize;
		m_csize=m_gemFrame.setCsizeByFormat(m_format);
		m_gemFrame.reallocate();
		
		if (!(m_hic = ICLocate(ICTYPE_VIDEO, 0, m_pbmihRaw, m_pbmihDst, ICMODE_DECOMPRESS)))
		{
			error("GEM: pix_film: Could not find decompressor: %s", filename);
		} else
		{
			
			if (1 == m_csize)
			{
				if (ICERR_OK != ICDecompressSetPalette(m_hic, m_pbmihDst))
				{
					error("GEM: pix_film: Could not set palette: %s", filename);
				}
			}

			if (ICERR_OK != ICDecompressBegin(m_hic, m_pbmihRaw, m_pbmihDst))
			{
				error("GEM: pix_film: Could not begin decompression: %s", filename);
			} else
			{
				//if (!m_pbmihRaw->biSizeImage)
				//	m_pbmihRaw->biSizeImage = m_xsize * m_ysize * m_csize;

				//m_nRawBuffSize = min(streaminfo.dwSuggestedBufferSize, m_pbmihRaw->biSizeImage);
				m_nRawBuffSize = max(streaminfo.dwSuggestedBufferSize, m_pbmihRaw->biSizeImage);
				if (!m_nRawBuffSize)
					m_nRawBuffSize = m_xsize * m_ysize * m_csize;

				m_RawBuffer = new unsigned char[m_nRawBuffSize];

				m_haveMovie = GEM_MOVIE_AVI;
				m_reqFrame = 0;
				m_curFrame = -1;
			}
		}
	}
  }

  if (GEM_MOVIE_NONE == m_haveMovie)
  {	// clean up if we failed to open the clip
	if (m_streamVid)
	{
		AVIStreamRelease(m_streamVid);
		m_streamVid = NULL;
	}

	if (m_pbmihRaw)
	{
		delete[] m_pbmihRaw;
		m_pbmihRaw = NULL;
	}

	if (m_pbmihDst)
	{
		delete[] m_pbmihDst;
		m_pbmihDst = NULL;
	} 

	if (m_hic)
	{
		ICClose(m_hic);
		m_hic = NULL;
	}

	if (m_RawBuffer)
	{
		delete[] m_RawBuffer;
		m_RawBuffer = NULL;	
		m_nRawBuffSize = 0;
	}
  }
	
  return;
}

/////////////////////////////////////////////////////////
// render
//
/////////////////////////////////////////////////////////
void pix_filmNT :: getFrame()
{
//  unsigned char *pt=0;
  BITMAPINFOHEADER* pbmih;
  long lBytesWritten;
  switch (m_haveMovie) {
  case GEM_MOVIE_AVI:
    if (m_reqFrame > m_numFrames)m_reqFrame=m_numFrames;
	
	if (!AVIStreamRead(m_streamVid, m_reqFrame, 1, m_RawBuffer, m_nRawBuffSize, &lBytesWritten, 0))
	{
		m_pbmihRaw->biSize = lBytesWritten;
		ICDecompress(m_hic, 0, m_pbmihRaw, m_RawBuffer, m_pbmihDst, m_decodedFrame.data);
//		if (ICERR_OK == ICDecompress(m_hic, 0, m_pbmihRaw, m_RawBuffer, m_pbmihDst, m_frame))
//			m_frame = m_DstBuffer;
		switch (m_decodedFrame.format){
		case GL_LUMINANCE: m_gemFrame.fromGray(m_decodedFrame.data); break;
		default:
		case GL_BGR_EXT:   m_gemFrame.fromBGR(m_decodedFrame.data);
		}
		m_frame=m_gemFrame.data;
	}
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

  class_addmethod(classPtr, (t_method)&pix_filmNT::changeImageCallback,
		  gensym("img_num"), A_GIMME, A_NULL);
  class_addmethod(classPtr, (t_method)&pix_filmNT::autoCallback,
		  gensym("auto"), A_DEFFLOAT, A_NULL);
}
void pix_filmNT :: changeImageCallback(void *data, t_symbol *, int argc, t_atom *argv)
{
    GetMyClass(data)->changeImage((argc<1)?0:atom_getint(argv), (argc<2)?0:atom_getint(argv+1));
}

void pix_filmNT :: autoCallback(void *data, t_floatarg state)
{
  GetMyClass(data)->m_auto=!(!(int)state);
}
#endif // __WIN32__
