////////////////////////////////////////////////////////
//
// GEM - Graphics Environment for Multimedia
//
// tigital@mac.com & cclep@artic.edu
//
// Implementation file
//
//    Copyright (c) 1997-1999 Mark Danks.
//    Copyright (c) 2002 James Tittle & Chris Clepper
//
//    For information on usage and redistribution, and for a DISCLAIMER OF ALL
//    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.
//
/////////////////////////////////////////////////////////

#include "pix_movieYUV.h"
#include "Base/GemMan.h"

CPPEXTERN_NEW_WITH_ONE_ARG(pix_movieYUV, t_symbol *, A_DEFSYM)

/////////////////////////////////////////////////////////
//
// pix_movie
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
pix_movieYUV :: pix_movieYUV(t_symbol *filename) :
#ifdef __WIN32__
  pix_filmNT(filename)
#elif __linux__
  pix_filmLinux(filename)
#elif __APPLE__
  pix_filmDarwinYUV(filename)
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
pix_movieYUV :: ~pix_movieYUV()
{
  // Clean up the movie
  closeMess();
  deleteBuffer();
}

/////////////////////////////////////////////////////////
// Buffer for Frames
//
/////////////////////////////////////////////////////////
void pix_movieYUV :: createBuffer()
{
  int neededXSize = powerOfTwo(m_xsize);
  int neededYSize = powerOfTwo(m_ysize);

  deleteBuffer();
  int dataSize = neededXSize * neededYSize * m_csize;
  m_pixBlock.image.data = new unsigned char[dataSize];
  m_data=m_pixBlock.image.data; // ??????????????
  memset(m_pixBlock.image.data, 0, dataSize);
  m_frame =/*(char*)*/m_pixBlock.image.data;
  
  m_pixBlock.image.xsize = neededXSize;
  m_pixBlock.image.ysize = neededYSize;
  m_pixBlock.image.csize = m_csize;
  m_pixBlock.image.format= m_format;
}

/////////////////////////////////////////////////////////
// on opening a file, prepare for texturing
//
/////////////////////////////////////////////////////////
void pix_movieYUV :: prepareTexture()
{
    if (!GemMan::texture_rectangle_supported)
    {
        int neededXSize = m_pixBlock.image.xsize;
        int neededYSize = m_pixBlock.image.ysize;
        post("pix_movie: prepareTexture: x : %d, y : %d", neededXSize, neededYSize );

        // ratio for the texture map coordinates
        m_xRatio = (float)m_xsize / (float)neededXSize;
        m_yRatio = (float)m_ysize / (float)neededYSize;
#ifndef __APPLE__   
        m_coords[0].s = 0.f;
        m_coords[0].t = 0.f;
    
        m_coords[1].s = m_xRatio;
        m_coords[1].t = 0.f;
    
        m_coords[2].s = m_xRatio;
        m_coords[2].t = m_yRatio;
    
        m_coords[3].s = 0.f;
        m_coords[3].t = m_yRatio;
#else
        m_coords[3].s = 0.f;
        m_coords[3].t = 0.f;
    
        m_coords[2].s = m_xRatio;
        m_coords[2].t = 0.f;
    
        m_coords[1].s = m_xRatio;
        m_coords[1].t = m_yRatio;
    
        m_coords[0].s = 0.f;
        m_coords[0].t = m_yRatio;
#endif
    } else {
#ifndef __APPLE__
        m_coords[0].s = 0.f;
        m_coords[0].t = 0.f;
    
        m_coords[1].s = m_pixBlock.image.xsize;
        m_coords[1].t = 0.f;
    
        m_coords[2].s = m_pixBlock.image.xsize;
        m_coords[2].t = m_pixBlock.image.ysize;
    
        m_coords[3].s = 0.f;
        m_coords[3].t = m_pixBlock.image.ysize;
#else
        m_coords[3].s = 0.f;
        m_coords[3].t = 0.f;
    
        m_coords[2].s = m_pixBlock.image.xsize;
        m_coords[2].t = 0.f;
    
        m_coords[1].s = m_pixBlock.image.xsize;
        m_coords[1].t = m_pixBlock.image.ysize;
    
        m_coords[0].s = 0.f;
        m_coords[0].t = m_pixBlock.image.ysize;
#endif
    }
}

/////////////////////////////////////////////////////////
// render
//
/////////////////////////////////////////////////////////
void pix_movieYUV :: texFrame(GemState *state, int doit)
{
  state->texture = 1;
  state->texCoords = m_coords;
  state->numTexCoords = 4;
  // enable to texture binding
#ifdef GL_TEXTURE_RECTANGLE_EXT
  if (!GemMan::texture_rectangle_supported)		//tigital
  {
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, m_textureObj);
  } else {
    glEnable(GL_TEXTURE_RECTANGLE_EXT);
    glBindTexture(GL_TEXTURE_RECTANGLE_EXT, m_textureObj);
  }
#else
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, m_textureObj);
#endif
  
  if (doit) {
    // if the size changed, then reset the texture
    if (m_pixBlock.image.csize != m_dataSize[0] ||
	m_pixBlock.image.xsize != m_dataSize[1] ||
	m_pixBlock.image.ysize != m_dataSize[2]) {
      m_dataSize[0] = m_pixBlock.image.csize;
      m_dataSize[1] = m_pixBlock.image.xsize;
      m_dataSize[2] = m_pixBlock.image.ysize;
#ifdef GL_TEXTURE_RECTANGLE_EXT     
      if (GemMan::texture_rectangle_supported)	//tigital
	glTexImage2D(GL_TEXTURE_RECTANGLE_EXT, 0,
		     m_pixBlock.image.csize,
		     m_pixBlock.image.xsize,
		     m_pixBlock.image.ysize, 0,
		     m_pixBlock.image.format,
		     m_pixBlock.image.type,
		     m_pixBlock.image.data);
      else
#endif
	glTexImage2D(GL_TEXTURE_2D, 0,
		     m_pixBlock.image.csize,
		     m_pixBlock.image.xsize,
		     m_pixBlock.image.ysize, 0,
		     m_pixBlock.image.format,
		     m_pixBlock.image.type,
		     m_pixBlock.image.data);
    }
    // okay, load in the actual pixel data
#ifdef GL_TEXTURE_RECTANGLE_EXT     
    if ( !GemMan::texture_rectangle_supported)
        glTexSubImage2D(GL_TEXTURE_RECTANGLE_EXT, 0,
		    0, 0,			// position
		    m_xsize,			// the x size of the data
		    m_ysize,			// the y size of the data
		    m_pixBlock.image.format,	// the format
		    m_pixBlock.image.type,	// the type
		    m_frame);		// the data + header offset
    else
#endif
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
void pix_movieYUV :: postrender(GemState *state)
{
  m_pixBlock.newimage = 0;
  state->image = NULL;
  state->texture = 0;
  if (m_numFrames>0 && m_reqFrame>m_numFrames){
    m_reqFrame = m_numFrames;
    outlet_bang(m_outEnd);
  }
#ifdef GL_TEXTURE_RECTANGLE_EXT
  if ( !GemMan::texture_rectangle_supported )
    glDisable(GL_TEXTURE_2D);
  else
    glDisable(GL_TEXTURE_RECTANGLE_EXT);
#else
    glDisable(GL_TEXTURE_2D);
#endif
}


/////////////////////////////////////////////////////////
// startRendering
//
/////////////////////////////////////////////////////////
void pix_movieYUV :: startRendering()
{
    glGenTextures(1, &m_textureObj);
#ifdef GL_TEXTURE_RECTANGLE_EXT
    if ( ! GemMan::texture_rectangle_supported )
        glBindTexture(GL_TEXTURE_2D, m_textureObj);
    else
        glBindTexture(GL_TEXTURE_RECTANGLE_EXT, m_textureObj);
#else
    glBindTexture(GL_TEXTURE_2D, m_textureObj);
#endif
  
    setUpTextureState();
    
    m_pixBlock.newimage = 1;
    m_dataSize[0] = m_dataSize[1] = m_dataSize[2] = -1;
}

/////////////////////////////////////////////////////////
// stopRendering
//
/////////////////////////////////////////////////////////
void pix_movieYUV :: stopRendering()
{
  if (m_textureObj) glDeleteTextures(1, &m_textureObj);
  m_textureObj = 0;
  m_dataSize[0] = m_dataSize[1] = m_dataSize[2] = 0;
}

/////////////////////////////////////////////////////////
// setUpTextureState
//
/////////////////////////////////////////////////////////
void pix_movieYUV :: setUpTextureState()
{
#ifdef GL_TEXTURE_RECTANGLE_EXT
    if ( !GemMan::texture_rectangle_supported )				//tigital
{
        glTexParameterf(GL_TEXTURE_RECTANGLE_EXT, GL_TEXTURE_PRIORITY, 0.0);
#ifdef GL_UNPACK_CLIENT_STORAGE_APPLE
        if (GemMan::client_storage_supported)
            glPixelStorei(GL_UNPACK_CLIENT_STORAGE_APPLE, 1);
        else
#endif
            glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        glTexParameteri(GL_TEXTURE_RECTANGLE_EXT, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_RECTANGLE_EXT, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_RECTANGLE_EXT, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_RECTANGLE_EXT, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    } else
#endif
    {
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    } 
}

/////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////
void pix_movieYUV :: obj_setupCallback(t_class *classPtr)
{
  class_addmethod(classPtr, (t_method)&pix_movieYUV::openMessCallback,
		  gensym("open"), A_SYMBOL, A_NULL);
  class_addmethod(classPtr, (t_method)&pix_movieYUV::changeImageCallback,
		  gensym("img_num"), A_GIMME, A_NULL);
  class_addmethod(classPtr, (t_method)&pix_movieYUV::autoCallback,
		  gensym("auto"), A_DEFFLOAT, A_NULL);
}

void pix_movieYUV :: openMessCallback(void *data, t_symbol *filename)
{
    GetMyClass(data)->openMess(filename);
}

void pix_movieYUV :: changeImageCallback(void *data, t_symbol *, int argc, t_atom *argv)
{
  //  GetMyClass(data)->changeImage((int)imgNum);
    GetMyClass(data)->changeImage((argc<1)?0:atom_getint(argv), (argc<2)?0:atom_getint(argv+1));
}

void pix_movieYUV :: autoCallback(void *data, t_floatarg state)
{
  GetMyClass(data)->m_auto=!(!(int)state);
}
