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
#ifdef __linux__

#include "Pixes/pix_filmLinux.h"

CPPEXTERN_NEW_WITH_ONE_ARG(pix_filmLinux, t_symbol *, A_DEFSYM)

/////////////////////////////////////////////////////////
//
// pix_filmLinux
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////

pix_filmLinux :: pix_filmLinux(t_symbol *filename) :
  pix_film(filename)
{
  static bool first_time=true;
  if (first_time) {
#ifdef HAVE_LIBQUICKTIME
  post("pix_film:: quicktime4linux support");
#endif
#ifdef HAVE_LIBMPEG3
  post("pix_film:: libmpeg3 support");
#else 
#ifdef HAVE_LIBMPEG
  post("pix_film:: libmpeg support");
#endif 
#endif
#ifdef HAVE_LIBAVIPLAY
  post("pix_film:: libaviplay support");
#endif
  first_time = false;
  }


#ifdef HAVE_LIBAVIPLAY
  m_avifile=0;
  m_avistream=0;
  m_aviimage=0;
#endif

  if (filename->s_name[0]) openMess(filename);
}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
pix_filmLinux :: ~pix_filmLinux()
{
  closeMess();
  deleteBuffer();

  outlet_free(m_outNumFrames);
  outlet_free(m_outEnd);
}

void pix_filmLinux :: closeMess(void)
{
  switch (m_haveMovie) {
#ifdef HAVE_LIBQUICKTIME
  case GEM_MOVIE_MOV:
    quicktime_close(quick_file);
    break;
#endif
  case GEM_MOVIE_MPG:
#ifdef HAVE_LIBMPEG3
    mpeg3_close(mpeg_file);
#else
#ifdef HAVE_LIBMPEG
    if (m_streamfile)fclose(m_streamfile);
#endif
#endif
    break;
#ifdef HAVE_LIBAVIPLAY
  case GEM_MOVIE_AVI:
    if (m_avistream)(*m_avistream).StopStreaming();
    break;
#endif
  case GEM_MOVIE_NONE:
  default:
    break;
  }
  m_haveMovie = GEM_MOVIE_NONE;
}
/////////////////////////////////////////////////////////
// really open the file ! (OS dependent)
//
/////////////////////////////////////////////////////////
void pix_filmLinux :: realOpen(char *filename)
{
  if (!(m_streamfile = fopen (filename, "rb"))) {
#ifndef HAVE_LIBAVIPLAY
    post("GEM: pix_film:: unable to open file %s", filename);
    return;
#endif
  }
#ifdef HAVE_LIBQUICKTIME
  else if (quicktime_check_sig(filename)){ /* ok, this is quicktime */
    fclose(m_streamfile);
    if (!(quick_file = quicktime_open(filename, 1, 0))){
      post("GEM: pix_film: Unable to open file: %s", filename);
      return;
    }
    m_haveMovie = GEM_MOVIE_MOV;
	
    m_track = 0;
    m_reqFrame = 0;
    m_curFrame = -1;
	
    // Get the number of tracks
    m_numTracks = quicktime_video_tracks(quick_file);
    // Get the length of the movie (on track 0)
    m_numFrames = quicktime_video_length(quick_file, 0) - 1;
    
    // Get the video dimensions
    m_xsize = quicktime_video_width (quick_file, 0);
    m_ysize = quicktime_video_height(quick_file, 0);
    
    char *codec = quicktime_video_compressor(quick_file, 0);
    
    if (!quicktime_supported_video(quick_file, 0)){
      post("GEM: pix_film: unsupported CODEC '%s'!", codec);
      quicktime_close(quick_file);
      m_haveMovie = GEM_MOVIE_NONE;
      return;
    }
    m_format=(quicktime_reads_cmodel(quick_file, BC_RGBA8888, 0))?GL_RGBA:GL_RGB;
    m_csize =(m_format==GL_RGBA)?4:(m_format==GL_RGB)?3:1;
  }
#endif /* QUICKTIME */
#ifdef HAVE_LIBMPEG3
  else if (mpeg3_check_sig(filename)){/* ok, this is mpeg */
    fclose(m_streamfile);
    mpeg_file= mpeg3_open(filename);
    if (!mpeg3_has_video(mpeg_file)){
      post("GEM: pix_film: this file %s does not seem to hold any video data", filename);
      mpeg3_close(mpeg_file);
      return;
    }

    m_numTracks = mpeg3_total_vstreams(mpeg_file);
    m_numFrames = mpeg3_video_frames(mpeg_file, 0);

    m_xsize=mpeg3_video_width(mpeg_file, 0);
    m_ysize=mpeg3_video_height(mpeg_file, 0);
	  
    m_haveMovie = GEM_MOVIE_MPG;
    m_reqFrame = 0;
    m_curFrame = -1;

    m_csize  = 4;
    m_format = GL_RGBA;
    
    if (!(m_xsize*m_ysize))goto unsupported;
  }
#else
#ifdef HAVE_LIBMPEG
  else if (OpenMPEG (m_streamfile, &m_streamVid)) { /* let's hope it's MPEG */
    SetMPEGOption (MPEG_DITHER, 
		   (1) ? (int) FULL_COLOR_DITHER : 
		   (int) ORDERED_DITHER);
    
    m_haveMovie = GEM_MOVIE_MPG;
    m_reqFrame = 0;
    m_curFrame = -1;
    
    // Unfortunately there is no way to get the length of an MPEG-Stream
    m_numFrames = -1;
    
    m_xsize  = m_streamVid.Width;
    m_ysize  = m_streamVid.Height;
    m_csize  = 4;
    m_format = GL_RGBA;
    
    if (!(m_xsize*m_ysize))goto unsupported;
  }
#endif
#endif /* MPEG */
  else {
#ifdef HAVE_LIBAVIPLAY
    // how do we close the avifile ??? automagically ?
    if (!(m_avifile = CreateIAviReadFile(filename)))goto unsupported;
    while(!(*m_avifile).IsOpened());
    if (!(*m_avifile).IsValid())goto unsupported;
    m_numTracks = (*m_avifile).VideoStreamCount();
    if (m_numTracks<1)return;
    if (m_track>=m_numTracks)m_track = 0;   
    try {
      m_avistream=(*m_avifile).GetStream(m_track, avm::IStream::StreamType(1));
    } catch (const char* string) {
      m_avistream = 0;
    }
    if (!m_avistream)goto unsupported;
    if ((*m_avistream).StartStreaming()==-1)goto unsupported;

    m_numFrames = (*m_avistream).GetLength();
    m_haveMovie = GEM_MOVIE_AVI;
    m_reqFrame = 0;
    m_curFrame = -1;
    if (1){
      StreamInfo *m_info = (*m_avistream).GetStreamInfo();
      m_xsize = (*m_info).GetVideoWidth();
      m_ysize = (*m_info).GetVideoHeight();
      m_csize = (*m_avistream).GetFrameSize()/(m_xsize*m_ysize);
      int format = (*m_avistream).GetVideoFormat();
      post("opened format = %x", format);
      m_format= 0;
    }
    if (!(m_xsize*m_ysize*m_csize))goto unsupported;
    return;
#endif
    
  unsupported:
    if(m_streamfile)fclose(m_streamfile);
    error("GEM: pix_film: '%s' does not appear to be a supported movie", filename);
    m_haveMovie = GEM_MOVIE_NONE;
  }
}

/////////////////////////////////////////////////////////
// render
//
/////////////////////////////////////////////////////////
void pix_filmLinux :: getFrame()
{
  unsigned char *rows[m_ysize];
  unsigned char **dummy=rows;
  int i;

  switch (m_haveMovie) {
  case GEM_MOVIE_MOV:
#ifdef HAVE_LIBQUICKTIME
    i=m_ysize;
    if (quicktime_set_video_position(quick_file, m_reqFrame, 0)){ } // couldn't set to position
    while(i--)rows[i]=m_frame+m_xsize*m_csize*(m_ysize-i-1);

    if (m_format==GL_RGBA){
      if (quicktime_decode_scaled(quick_file, 
				  0, 0, m_xsize, m_ysize, m_xsize, m_ysize,
				  BC_RGBA8888,
				  &m_frame, 0)) {
	post("GEM: pix_film:: couldn't decode video !");
      }
    } else { // RGB
      if (quicktime_decode_video(quick_file, rows, 0)) {
	post("GEM: pix_film:: couldn't decode video !");
      }
    }
#endif /* QUICKTIME */
    break;
  case GEM_MOVIE_MPG:
#ifdef HAVE_LIBMPEG3
    if (mpeg3_set_frame(mpeg_file, m_curFrame, 0)){ } // couldn't set to position

    i=m_ysize;
    while(i--)*dummy++=(unsigned char *)(m_frame+i*m_xsize*m_csize);
    if (mpeg3_read_frame(mpeg_file, rows,
			 0, 0, m_xsize, m_ysize, m_xsize, m_ysize,
			 MPEG3_RGBA8888, 0)) {
      //      post("GEM: pix_film:: could not read frame !");
    }
#else
#ifdef HAVE_LIBMPEG
    if (m_curFrame<=0)RewindMPEG(m_streamfile,&m_streamVid);
    if (!GetMPEGFrame ((char*)m_frame)){
      RewindMPEG(m_streamfile,&m_streamVid);
      GetMPEGFrame ((char*)m_frame);
      outlet_bang(m_outEnd);
    }
#endif
#endif /* MPEG */
    break;
  case GEM_MOVIE_AVI:
#ifdef HAVE_LIBAVIPLAY
    if (!m_avistream)break;
    (*m_avistream).Seek(m_reqFrame);
    if(m_aviimage)(*m_aviimage).Release();
    m_aviimage = (*m_avistream).GetFrame(true);
    if (m_aviimage){
      int format = (*m_aviimage).Format();
      switch (format){
      case IMG_FMT_RGB24: format = GL_RGB; break;
      case IMG_FMT_RGB32: format = GL_RGBA; break;
      case IMG_FMT_BGR24: format = GL_BGR; break;
      case IMG_FMT_BGR32: format = GL_BGRA; break;
      case IMG_FMT_Y422:
      case IMG_FMT_UYVY:
      case IMG_FMT_CPLA:
      case IMG_FMT_UYNV:
      case 0x32315659: // this is NOT YUV422...
	format = GL_YCBCR_422_GEM;
	break;
      default:
	//	post("format conversion...");
	CImage buf(m_frame, m_xsize, m_ysize);
	//post("...");
	buf.Convert(m_aviimage);
	//post("0x%X to format 0x%X", (*m_aviimage).Format(), buf.Format());
	format = GL_RGB;
	//post("unknown format 0x%X", format);
	//format = 0;
	m_aviimage = &buf;
      }
      m_format = format;
      m_pixBlock.image.format= m_format;
      m_frame = (*m_aviimage).Data();
    }
#endif
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
void pix_filmLinux :: obj_setupCallback(t_class *classPtr)
{
  class_addcreator((t_newmethod)_classpix_filmLinux, gensym("pix_film"), A_DEFSYM, A_NULL);
  pix_film::real_obj_setupCallback(classPtr);
}

#endif // __linux__
