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
#ifdef HAVE_QUICKTIME
  post("pix_film:: quicktime4linux support");
#endif
#ifdef HAVE_MPEG3
  post("pix_film:: libmpeg3 support");
#else 
#ifdef HAVE_MPEG
  post("pix_film:: libmpeg support");
#endif 
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
#ifdef HAVE_QUICKTIME
  case GEM_MOVIE_MOV:
    quicktime_close(quick_file);
    m_haveMovie = GEM_MOVIE_NONE;
    break;
#endif
  case GEM_MOVIE_MPG:
#ifdef HAVE_MPEG3
    mpeg3_close(mpeg_file);
#else
#ifdef HAVE_MPEG
    if (m_streamfile)fclose(m_streamfile);
#endif
#endif
    m_haveMovie = GEM_MOVIE_NONE;
    break;
  case GEM_MOVIE_NONE:
  default:
    break;
  }
}
/////////////////////////////////////////////////////////
// really open the file ! (OS dependent)
//
/////////////////////////////////////////////////////////
void pix_filmLinux :: realOpen(char *filename)
{
  if (!(m_streamfile = fopen (filename, "rb"))) {
    post("GEM: pix_film:: unable to open file %s", filename);
    return;
  } 
#ifdef HAVE_QUICKTIME
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
#ifdef HAVE_MPEG3
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
#ifdef HAVE_MPEG
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
  unsupported:
    error("GEM: pix_film: '%s' does not appear to be a supported movie", filename);
    m_haveMovie = GEM_MOVIE_NONE;
    return;
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
#ifdef HAVE_QUICKTIME
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
    } else {
      if (quicktime_decode_video(quick_file, rows, 0)) {
	post("GEM: pix_film:: couldn't decode video !");
      }
    }
#endif /* QUICKTIME */
    break;
  case GEM_MOVIE_MPG:
#ifdef HAVE_MPEG3
    if (mpeg3_set_frame(mpeg_file, m_curFrame, 0)){ } // couldn't set to position

    i=m_ysize;
    while(i--)*dummy++=(unsigned char *)(m_frame+i*m_xsize*m_csize);
    if (mpeg3_read_frame(mpeg_file, rows,
			 0, 0, m_xsize, m_ysize, m_xsize, m_ysize,
			 MPEG3_RGBA8888, 0)) {
      //      post("GEM: pix_film:: could not read frame !");
    }
#else
#ifdef HAVE_MPEG
    if (m_curFrame<=0)RewindMPEG(m_streamfile,&m_streamVid);
    if (!GetMPEGFrame ((char*)m_frame)){
      RewindMPEG(m_streamfile,&m_streamVid);
      GetMPEGFrame ((char*)m_frame);
      outlet_bang(m_outEnd);
    }
#endif
#endif /* MPEG */
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

  class_addmethod(classPtr, (t_method)&pix_filmLinux::openMessCallback,
		  gensym("open"), A_SYMBOL, A_NULL);
  class_addmethod(classPtr, (t_method)&pix_filmLinux::changeImageCallback,
		  gensym("img_num"), A_GIMME, A_NULL);
  class_addmethod(classPtr, (t_method)&pix_filmLinux::autoCallback,
		  gensym("auto"), A_DEFFLOAT, A_NULL);
}

void pix_filmLinux :: openMessCallback(void *data, t_symbol *filename)
{
  GetMyClass(data)->openMess(filename);
}

void pix_filmLinux :: changeImageCallback(void *data, t_symbol *, int argc, t_atom *argv)
{
    GetMyClass(data)->changeImage((argc<1)?0:atom_getint(argv), (argc<2)?0:atom_getint(argv+1));
}

void pix_filmLinux :: autoCallback(void *data, t_floatarg state)
{
  GetMyClass(data)->m_auto=!(!(int)state);
}
#endif // __linux__
