////////////////////////////////////////////////////////
//
// GEM - Graphics Environment for Multimedia
//
// cclep@artic.edu & tigital@mac.com
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

#include "yuv_movie.h"
#include "Base/GemMan.h"
#include <OpenGL/glu.h>

static inline int powerOfTwo(int value)
{
  int x = 1;
  while(x < value) x <<= 1;
  return(x);
}

CPPEXTERN_NEW_WITH_ONE_ARG(yuv_movie, t_symbol *, A_DEFSYM)

/////////////////////////////////////////////////////////
//
// yuv_movie
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
yuv_movie :: yuv_movie(t_symbol *filename) :
#ifdef _WINDOWS
  pix_filmNT(filename)
#elif __linux__
  pix_filmLinux(filename)
#elif MACOSX
  yuv_filmDarwin(filename)
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
yuv_movie :: ~yuv_movie()
{
  // Clean up the movie
  closeMess();
  deleteBuffer();
}

/////////////////////////////////////////////////////////
// Buffer for Frames
//
/////////////////////////////////////////////////////////
void yuv_movie :: createBuffer()
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
void yuv_movie :: prepareTexture()
{
    if (!GemMan::texture_rectangle_supported)
    {
        int neededXSize = m_pixBlock.image.xsize;
        int neededYSize = m_pixBlock.image.ysize;
        post("pix_movie: prepareTexture: x : %d, y : %d", neededXSize, neededYSize );

        // ratio for the texture map coordinates
        m_xRatio = (float)m_xsize / (float)neededXSize;
        m_yRatio = (float)m_ysize / (float)neededYSize;
#ifndef MACOSX   
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
#ifndef MACOSX
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
void yuv_movie :: texFrame(GemState *state, int doit)
{
  state->texture = 1;
  state->texCoords = m_coords;
  state->numTexCoords = 4;
  // enable to texture binding
  if (!GemMan::texture_rectangle_supported)		//tigital
  {
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, m_textureObj);
  } else {
    glEnable(GL_TEXTURE_RECTANGLE_EXT);
    glBindTexture(GL_TEXTURE_RECTANGLE_EXT, m_textureObj);
  }
  
  if (doit) {
    // if the size changed, then reset the texture
    if (m_pixBlock.image.csize != m_dataSize[0] ||
	m_pixBlock.image.xsize != m_dataSize[1] ||
	m_pixBlock.image.ysize != m_dataSize[2]) {
      m_dataSize[0] = m_pixBlock.image.csize;
      m_dataSize[1] = m_pixBlock.image.xsize;
      m_dataSize[2] = m_pixBlock.image.ysize;
      
        if (!GemMan::texture_rectangle_supported)	//tigital
        {
            glTexImage2D(GL_TEXTURE_2D, 0,
		   m_pixBlock.image.csize,
		   m_pixBlock.image.xsize,
		   m_pixBlock.image.ysize, 0,
		   m_pixBlock.image.format,
		   m_pixBlock.image.type,
		   m_pixBlock.image.data);
        } else {
            glTexImage2D(GL_TEXTURE_RECTANGLE_EXT, 0,
		   m_pixBlock.image.csize,
		   m_pixBlock.image.xsize,
		   m_pixBlock.image.ysize, 0,
		   m_pixBlock.image.format,
		   m_pixBlock.image.type,
		   m_pixBlock.image.data);
        }
    }
    // okay, load in the actual pixel data
    if ( !GemMan::texture_rectangle_supported)
    {
        glTexSubImage2D(GL_TEXTURE_2D, 0,
		    0, 0,			// position
		    m_xsize,			// the x size of the data
		    m_ysize,			// the y size of the data
		    m_pixBlock.image.format,	// the format
		    m_pixBlock.image.type,	// the type
		    m_frame);		// the data + header offset
    }else{
        glTexSubImage2D(GL_TEXTURE_RECTANGLE_EXT, 0,
		    0, 0,			// position
		    m_xsize,			// the x size of the data
		    m_ysize,			// the y size of the data
		    m_pixBlock.image.format,	// the format
		    m_pixBlock.image.type,	// the type
		    m_frame);		// the data + header offset
    }
  }
}

/////////////////////////////////////////////////////////
// postrender
//
/////////////////////////////////////////////////////////
void yuv_movie :: postrender(GemState *state)
{
  m_pixBlock.newimage = 0;
  state->image = NULL;
  state->texture = 0;
  if (m_numFrames>0 && m_reqFrame>m_numFrames){
    m_reqFrame = m_numFrames;
    outlet_bang(m_outEnd);
  }
  if ( !GemMan::texture_rectangle_supported )
    glDisable(GL_TEXTURE_2D);
  else
    glDisable(GL_TEXTURE_RECTANGLE_EXT);
}


/////////////////////////////////////////////////////////
// startRendering
//
/////////////////////////////////////////////////////////
void yuv_movie :: startRendering()
{
    glGenTextures(1, &m_textureObj);
    if ( ! GemMan::texture_rectangle_supported )
        glBindTexture(GL_TEXTURE_2D, m_textureObj);
    else
        glBindTexture(GL_TEXTURE_RECTANGLE_EXT, m_textureObj);
  
    setUpTextureState();
    
    m_pixBlock.newimage = 1;
    m_dataSize[0] = m_dataSize[1] = m_dataSize[2] = -1;
}

/////////////////////////////////////////////////////////
// stopRendering
//
/////////////////////////////////////////////////////////
void yuv_movie :: stopRendering()
{
  if (m_textureObj) glDeleteTextures(1, &m_textureObj);
  m_textureObj = 0;
  m_dataSize[0] = m_dataSize[1] = m_dataSize[2] = 0;
}

/////////////////////////////////////////////////////////
// setUpTextureState
//
/////////////////////////////////////////////////////////
void yuv_movie :: setUpTextureState()
{
    if ( !GemMan::texture_rectangle_supported )				//tigital
    {
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    } else {
        glTexParameterf(GL_TEXTURE_RECTANGLE_EXT, GL_TEXTURE_PRIORITY, 0.0);
        if (GemMan::client_storage_supported)
            glPixelStorei(GL_UNPACK_CLIENT_STORAGE_APPLE, 1);
        else
            glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        glTexParameteri(GL_TEXTURE_RECTANGLE_EXT, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_RECTANGLE_EXT, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_RECTANGLE_EXT, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_RECTANGLE_EXT, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    }
}

/////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////
void yuv_movie :: obj_setupCallback(t_class *classPtr)
{
  class_addmethod(classPtr, (t_method)&yuv_movie::openMessCallback,
		  gensym("open"), A_SYMBOL, A_NULL);
  class_addmethod(classPtr, (t_method)&yuv_movie::changeImageCallback,
		  gensym("img_num"), A_GIMME, A_NULL);
  class_addmethod(classPtr, (t_method)&yuv_movie::autoCallback,
		  gensym("auto"), A_DEFFLOAT, A_NULL);
}

void yuv_movie :: openMessCallback(void *data, t_symbol *filename)
{
    GetMyClass(data)->openMess(filename);
}

void yuv_movie :: changeImageCallback(void *data, t_symbol *, int argc, t_atom *argv)
{
  //  GetMyClass(data)->changeImage((int)imgNum);
    GetMyClass(data)->changeImage((argc<1)?0:atom_getint(argv), (argc<2)?0:atom_getint(argv+1));
}

void yuv_movie :: autoCallback(void *data, t_floatarg state)
{
  GetMyClass(data)->m_auto=!(!(int)state);
}
