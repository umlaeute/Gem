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
//    Copyright (c) 2002 James Tittle & Chris Clepper
//    For information on usage and redistribution, and for a DISCLAIMER OF ALL
//    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.
//
/////////////////////////////////////////////////////////

#if defined NT && defined __APPLE__ && defined __linux__
// on this OSs we surely have child-classes for the real function
# define NO_AUTO_REGISTER_CLASS
#endif

#include "pix_film.h"
#include <ctype.h>

CPPEXTERN_NEW_WITH_ONE_ARG(pix_film, t_symbol *, A_DEFSYM)

/////////////////////////////////////////////////////////
//
// pix_film
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
pix_film :: pix_film(t_symbol *filename) :
  m_haveMovie(0), m_auto(0), 
  m_numFrames(0), m_reqFrame(0), m_curFrame(0),
  m_numTracks(0), m_track(0), m_frame(NULL), m_data(NULL), m_film(true),
  m_newFilm(0),
#ifdef __APPLE__
  m_colorspace(GL_BGRA_EXT), m_format(GL_BGRA_EXT)
#else
  m_colorspace(GL_RGBA), m_format(GL_RGBA)
#endif
{
 // setting the current frame
 inlet_new(this->x_obj, &this->x_obj->ob_pd, gensym("float"), gensym("img_num"));
 // create an outlet to send out how many frames are in the movie + bang when we reached the end
 m_outNumFrames = outlet_new(this->x_obj, 0);
 m_outEnd       = outlet_new(this->x_obj, 0);

 // initialize the pix block data
 m_pixBlock.image=m_imageStruct;
 m_pixBlock.image.setCsizeByFormat(m_format);

 // make sure that there are some characters
 x_filename=gensym("");
}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
pix_film :: ~pix_film()
{
  // Clean up the movie
  closeMess();
  deleteBuffer();
}

void pix_film :: deleteBuffer()
{
  //post("deleting buffer %x", m_data);
  if (m_data){
    delete [] m_data;
    //post("deleted");
  }
  
  m_pixBlock.image.data=NULL;
  m_frame=m_data=NULL;
  m_pixBlock.image.xsize=m_pixBlock.image.ysize=m_pixBlock.image.csize=0;
}

void pix_film :: createBuffer()
{
  const int neededXSize = m_xsize;
  const int neededYSize = m_ysize;
  int	oldx, oldy;
  
oldx = 0;
oldy = 0;

    if (neededXSize != oldx || neededYSize != oldy)
    {
deleteBuffer();

  m_pixBlock.image.xsize = neededXSize;
  m_pixBlock.image.ysize = neededYSize;
  m_pixBlock.image.csize = m_csize;
  m_pixBlock.image.format= m_format;
#ifdef __APPLE__
  int dataSize = m_pixBlock.image.xsize * m_pixBlock.image.ysize * m_pixBlock.image.csize;
#else
  int dataSize = m_pixBlock.image.xsize * m_pixBlock.image.ysize * m_pixBlock.image.csize+4; /* +4 from MPEG */
#endif
  m_data = new unsigned char[dataSize];
 // memset(m_data, 0, dataSize);

  m_pixBlock.image.data = m_data;
  m_frame =  m_data;

  m_pixBlock.image.csize = m_csize;
  m_pixBlock.image.format= m_format;

    oldx = m_pixBlock.image.xsize;
    oldy = m_pixBlock.image.ysize;
}
  //post("created buffer @ %x", m_data);
}

/////////////////////////////////////////////////////////
// openMess
//
/////////////////////////////////////////////////////////

void pix_film :: openMess(t_symbol *filename, int format)
{
  //  if (filename==x_filename)return;
  x_filename=filename;
  if (format)m_colorspace=format;

  char buf[MAXPDSTRING];
  canvas_makefilename(getCanvas(), filename->s_name, buf, MAXPDSTRING);

  // Clean up any open files
  closeMess();

  m_haveMovie = GEM_MOVIE_NONE;
  realOpen(buf);
  if (m_haveMovie == GEM_MOVIE_NONE)return;
  
#ifndef __APPLE__
  createBuffer();
  prepareTexture();
#endif

  t_atom ap[3];
  SETFLOAT(ap, m_numFrames);
  SETFLOAT(ap+1, m_xsize);
  SETFLOAT(ap+2, m_ysize);

    m_newFilm = 1;
  //outlet_float(m_outNumFrames, (float)m_numFrames);
  post("GEM: pix_film: Loaded file: %s with %d frames (%dx%d)", buf, m_numFrames, m_xsize, m_ysize);
  outlet_list(m_outNumFrames, 0, 3, ap);
}

/////////////////////////////////////////////////////////
// render
//
/////////////////////////////////////////////////////////
void pix_film :: startRendering()
{
  m_pixBlock.newimage = 1;
  m_pixBlock.newfilm = 0;
  
}
void pix_film :: render(GemState *state)
{
  /* get the current frame from the file */
  
  newImage = 0;
  if (!m_haveMovie || !m_pixBlock.image.data)return;
  // do we actually need to get a new frame from the movie ?
  
  if (m_reqFrame != m_curFrame) {
    //newImage = 1;
    getFrame();
    m_curFrame = m_reqFrame;
    if (m_film)m_pixBlock.image.data = m_frame; // this is mainly for windows
  }
  else
  {
  newImage = 0;
  }
   
  if (m_newFilm){
    m_pixBlock.newfilm = 1;
    m_newFilm = 0;
   // post("pix_film: new film set");
  }
  state->image = &m_pixBlock;
  
#ifdef __APPLE__
  if (m_reqFrame == m_curFrame)
        
      //  ::MoviesTask(NULL, 0);
#endif
  /* texture it, if needed */
  texFrame(state, newImage);
  m_pixBlock.newimage = newImage;
  // automatic proceeding
  if (m_auto)m_reqFrame++;
}

/////////////////////////////////////////////////////////
// postrender
//
/////////////////////////////////////////////////////////
void pix_film :: postrender(GemState *state)
{
    
    m_pixBlock.newimage = 0;
  if (m_numFrames>0 && m_reqFrame>m_numFrames){
    m_reqFrame = m_numFrames;
    outlet_bang(m_outEnd);
  }
  
  m_newFilm = 0;
  m_pixBlock.newfilm = m_newFilm;
}

/////////////////////////////////////////////////////////
// changeImage
//
/////////////////////////////////////////////////////////
void pix_film :: changeImage(int imgNum, int trackNum)
{
  if (imgNum < 0){
    error("GEM: pix_film: selection number must be > 0");
    imgNum=0;
  }
  if (trackNum < 0){
    error("GEM: pix_film: track number must be > 0");
    trackNum=0;
  }


  switch (m_haveMovie){
  case GEM_MOVIE_MPG:
#ifdef HAVE_LIBMPEG3
#else
#ifdef HAVE_LIBMPEG
    m_reqFrame=(imgNum)?(m_curFrame==1)?2:1:0;
    break;
#endif
#endif
  case GEM_MOVIE_MOV:
    if (trackNum > m_numTracks-1) error("GEM: pix_film: track %d number too high (max %d) ", trackNum, m_numTracks-1);
    else m_track = trackNum;
  case GEM_MOVIE_AVI:
  default:
    if (imgNum > m_numFrames) {
      if (m_numFrames<0) m_reqFrame = imgNum;
      else m_reqFrame=m_numFrames;
      //      else error("GEM: pix_film: frame %d exceeds max (%d)", imgNum, m_numFrames);
      //m_reqFrame = imgNum;
      return;
    } else m_reqFrame = imgNum;
  }
}

/////////////////////////////////////////////////////////
// changeImage
//
/////////////////////////////////////////////////////////
void pix_film :: csMess(int format){
	if(format && format != m_colorspace){
		m_colorspace=format;
		post("pix_film: colorspace change will take effect the next time you load a film");
	}
}




/////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////
void pix_film :: obj_setupCallback(t_class *classPtr)
{
  class_addmethod(classPtr, (t_method)&pix_film::openMessCallback,
		  gensym("open"), A_GIMME, A_NULL);
  class_addmethod(classPtr, (t_method)&pix_film::changeImageCallback,
		  gensym("img_num"), A_GIMME, A_NULL);
  class_addmethod(classPtr, (t_method)&pix_film::autoCallback,
		  gensym("auto"), A_DEFFLOAT, A_NULL);
  class_addmethod(classPtr, (t_method)&pix_film::colorspaceCallback,
		  gensym("colorspace"), A_SYMBOL, A_NULL);
  class_addmethod(classPtr, (t_method)&pix_film::colorspaceCallback,
		  gensym("colourspace"), A_SYMBOL, A_NULL);
}
void pix_film :: openMessCallback(void *data, t_symbol*, int argc, t_atom*argv)
{
	int format=0;
	switch(argc){
	case 2:
		format=getPixFormat(atom_getsymbol(argv+1)->s_name);
	case 1:
	    GetMyClass(data)->openMess(atom_getsymbol(argv), format);
		break;
	default:
		error("pix_film: open <filename> [<format>]");
	}
}

void pix_film :: changeImageCallback(void *data, t_symbol *, int argc, t_atom *argv)
{
    GetMyClass(data)->changeImage((argc<1)?0:atom_getint(argv), (argc<2)?0:atom_getint(argv+1));
}

void pix_film :: autoCallback(void *data, t_floatarg state)
{
  GetMyClass(data)->m_auto=!(!(int)state);
}

void pix_film :: colorspaceCallback(void *data, t_symbol *state)
{
  GetMyClass(data)->csMess(getPixFormat(state->s_name));
}
