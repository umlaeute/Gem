/*-----------------------------------------------------------------

GEM - Graphics Environment for Multimedia

Load an digital video (like AVI, Mpeg, Quicktime) into a texture

Copyright (c) 1997-1999 Mark Danks. mark@danks.org
Copyright (c) 2002 James Tittle & Chris Clepper
For information on usage and redistribution, and for a DISCLAIMER OF ALL
WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

-----------------------------------------------------------------*/

#ifndef INCLUDE_PIX_MOVIEYUV_H_
#define INCLUDE_PIX_MOVIEYUV_H_

#ifdef _WINDOWS
#include "Pixes/pix_filmNT.h"
#elif __linux__
#include "Pixes/pix_filmLinux.h"
#elif MACOSX
#include "pix_filmDarwinYUV.h"
#else
#error Define pix_film for this OS
#endif

/*-----------------------------------------------------------------
  -------------------------------------------------------------------
  CLASS
  pix_movie
    
  Loads in a movie
    
  KEYWORDS
  pix
    
  DESCRIPTION

  -----------------------------------------------------------------*/
#ifdef _WINDOWS
class GEM_EXTERN pix_movieYUV : public pix_filmNT
{
  CPPEXTERN_HEADER(pix_movieYUV, pix_filmNT)
#elif __linux__
class GEM_EXTERN pix_movieYUV : public pix_filmLinux
{
  CPPEXTERN_HEADER(pix_movieYUV, pix_filmLinux)
#elif MACOSX
class GEM_EXTERN pix_movieYUV : public pix_filmDarwinYUV
{
    CPPEXTERN_HEADER(pix_movieYUV, pix_filmDarwinYUV)
#endif
   
    public:
  
  //////////
  // Constructor
  pix_movieYUV(t_symbol *filename);

 protected:
    
  //////////
  // Destructor
  virtual ~pix_movieYUV();

  //////////
  // create and delete buffers
  virtual void createBuffer();

  //////////
  // prepare for texturing (on open)
  virtual void prepareTexture();

  //////////
  // Do the rendering
  virtual void texFrame(GemState *state, int doit);

  //////////
  // Clear the dirty flag on the pixBlock
  virtual void postrender(GemState *state);

  //////////
  virtual void startRendering();

  //////////
  // Delete texture object
  virtual void stopRendering();
    	
  //////////
  virtual void setUpTextureState();

  //-----------------------------------
  // GROUP:	Texture data
  //-----------------------------------

  //////////
  // The texture coordinates
  TexCoord    	m_coords[4];
  //////////
  // The size of the texture (so we can use sub image)
  int		m_dataSize[3];

  GLuint	m_textureObj;	
  float		m_xRatio;
  float		m_yRatio;

 protected:
	
  //////////
  // static member functions
  static void openMessCallback   (void *data, t_symbol *filename);
  static void changeImageCallback(void *data, t_symbol *, int argc, t_atom *argv);
  static void autoCallback       (void *data, t_floatarg state);
};

#endif	// for header file
