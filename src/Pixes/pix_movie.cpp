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
//
//    For information on usage and redistribution, and for a DISCLAIMER OF ALL
//    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.
//
/////////////////////////////////////////////////////////

#include "pix_movie.h"


static inline int powerOfTwo(int value)
{
  int x = 1;
  while(x < value) x <<= 1;
  return(x);
}

CPPEXTERN_NEW_WITH_ONE_ARG(pix_movie, t_symbol *, A_DEFSYM)

/////////////////////////////////////////////////////////
//
// pix_movie
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
pix_movie :: pix_movie(t_symbol *filename) :
#ifdef _WINDOWS
  pix_filmNT(filename)
#elif __linux__
  pix_filmFFMPEG(filename)
#else
#error define pix_film for your OS
#endif
{
  m_film=false;
}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
pix_movie :: ~pix_movie()
{
  // Clean up the movie
  closeMess();
  deleteBuffer();
}

/////////////////////////////////////////////////////////
// Buffer for Frames
//
/////////////////////////////////////////////////////////
void pix_movie :: createBuffer()
{
  int neededXSize = powerOfTwo(m_xsize);
  int neededYSize = powerOfTwo(m_ysize);
  
  deleteBuffer();
  int dataSize = neededXSize * neededYSize * m_csize;
  m_pixBlock.image.data = new unsigned char[dataSize];
  m_data=m_pixBlock.image.data; // ??????????????
  memset(m_pixBlock.image.data, 0, dataSize);
  m_frame =/*(char*)*/m_pixBlock.image.data;

  m_pixBlock.image.xsize  = neededXSize;
  m_pixBlock.image.ysize  = neededYSize;
  m_pixBlock.image.csize  = m_csize;
  m_pixBlock.image.format = m_format;
}

/////////////////////////////////////////////////////////
// on opening a file, prepare for texturing
//
/////////////////////////////////////////////////////////
void pix_movie :: prepareTexture()
{
  int neededXSize = m_pixBlock.image.xsize;
  int neededYSize = m_pixBlock.image.ysize;

  // ratio for the texture map coordinates
  m_xRatio = (float)m_xsize / (float)neededXSize;
  m_yRatio = (float)m_ysize / (float)neededYSize;
   
  m_coords[0].s = 0.f;
  m_coords[0].t = 0.f;
    
  m_coords[1].s = m_xRatio;
  m_coords[1].t = 0.f;
    
  m_coords[2].s = m_xRatio;
  m_coords[2].t = m_yRatio;
    
  m_coords[3].s = 0.f;
  m_coords[3].t = m_yRatio;
}

/////////////////////////////////////////////////////////
// render
//
/////////////////////////////////////////////////////////
void pix_movie :: texFrame(GemState *state, int doit)
{
  state->texture = 1;
  state->texCoords = m_coords;
  state->numTexCoords = 4;
  // enable to texture binding
  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, m_textureObj);
  
  if (doit) {
    // if the size changed, then reset the texture
    if (m_pixBlock.image.csize != m_dataSize[0] ||
	m_pixBlock.image.xsize != m_dataSize[1] ||
	m_pixBlock.image.ysize != m_dataSize[2]) {
      m_dataSize[0] = m_pixBlock.image.csize;
      m_dataSize[1] = m_pixBlock.image.xsize;
      m_dataSize[2] = m_pixBlock.image.ysize;
      
      glTexImage2D(GL_TEXTURE_2D, 0,
		   m_pixBlock.image.csize,
		   m_pixBlock.image.xsize,
		   m_pixBlock.image.ysize, 0,
		   m_pixBlock.image.format,
		   m_pixBlock.image.type,
		   m_pixBlock.image.data);
    }

    // okay, load in the actual pixel data
    glTexSubImage2D(GL_TEXTURE_2D, 0,
		    0, 0,			// position
		    m_xsize,			// the x size of the data
		    m_ysize,			// the y size of the data
		    m_pixBlock.image.format,	// the format
		    m_pixBlock.image.type,	// the type
		    m_frame);		// the data + header offset

  }
}

/////////////////////////////////////////////////////////
// postrender
//
/////////////////////////////////////////////////////////
void pix_movie :: postrender(GemState *state)
{
  m_pixBlock.newimage = 0;
  state->image = NULL;
  state->texture = 0;
  if (m_numFrames>0 && m_reqFrame>m_numFrames){
    m_reqFrame = m_numFrames;
    outlet_bang(m_outEnd);
  }

  glDisable(GL_TEXTURE_2D);
}


/////////////////////////////////////////////////////////
// startRendering
//
/////////////////////////////////////////////////////////
void pix_movie :: startRendering()
{
  glGenTextures(1, &m_textureObj);
  glBindTexture(GL_TEXTURE_2D, m_textureObj);
  setUpTextureState();
    
  m_pixBlock.newimage = 1;
  m_dataSize[0] = m_dataSize[1] = m_dataSize[2] = 0;
}

/////////////////////////////////////////////////////////
// stopRendering
//
/////////////////////////////////////////////////////////
void pix_movie :: stopRendering()
{
  if (m_textureObj) glDeleteTextures(1, &m_textureObj);
  m_textureObj = 0;
  m_dataSize[0] = m_dataSize[1] = m_dataSize[2] = 0;
}

/////////////////////////////////////////////////////////
// setUpTextureState
//
/////////////////////////////////////////////////////////
void pix_movie :: setUpTextureState()
{
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
}

/////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////
void pix_movie :: obj_setupCallback(t_class *classPtr)
{
  class_addmethod(classPtr, (t_method)&pix_movie::openMessCallback,
		  gensym("open"), A_SYMBOL, A_NULL);
  class_addmethod(classPtr, (t_method)&pix_movie::changeImageCallback,
		  gensym("img_num"), A_GIMME, A_NULL);
  class_addmethod(classPtr, (t_method)&pix_movie::autoCallback,
		  gensym("auto"), A_DEFFLOAT, A_NULL);
}

void pix_movie :: openMessCallback(void *data, t_symbol *filename)
{
    GetMyClass(data)->openMess(filename);
}

void pix_movie :: changeImageCallback(void *data, t_symbol *, int argc, t_atom *argv)
{
  //  GetMyClass(data)->changeImage((int)imgNum);
    GetMyClass(data)->changeImage((argc<1)?0:atom_getint(argv), (argc<2)?0:atom_getint(argv+1));
}

void pix_movie :: autoCallback(void *data, t_floatarg state)
{
  GetMyClass(data)->m_auto=!(!(int)state);
}
