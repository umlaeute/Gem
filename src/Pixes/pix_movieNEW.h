/*-----------------------------------------------------------------

GEM - Graphics Environment for Multimedia

Load an digital video (like AVI, Mpeg, Quicktime) into a texture

Copyright (c) 1997-1999 Mark Danks. mark@danks.org
Copyright (c) G�nther Geiger. geiger@epy.co.at
Copyright (c) 2001-2002 IOhannes m zmoelnig. forum::f�r::uml�ute. IEM. zmoelnig@iem.kug.ac.at
Copyright (c) 2002 James Tittle & Chris Clepper
For information on usage and redistribution, and for a DISCLAIMER OF ALL
WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

-----------------------------------------------------------------*/

#ifndef INCLUDE_PIX_MOVIENEW_H_
#define INCLUDE_PIX_MOVIENEW_H_

#include "Pixes/pix_filmNEW.h"
#include "Pixes/pix_texture.h"

/*-----------------------------------------------------------------
  -------------------------------------------------------------------
  CLASS
  pix_movieNEW
    
  Loads in a movie
    
  KEYWORDS
  pix
    
  DESCRIPTION

  -----------------------------------------------------------------*/
class GEM_EXTERN pix_movieNEW : public pix_filmNEW
{
  CPPEXTERN_HEADER(pix_movieNEW, pix_filmNEW)
  
    public:

  //////////
  // Constructor
  pix_movieNEW(t_symbol *filename);

 protected:
    
  //////////
  // Destructor
  virtual ~pix_movieNEW();

  //////////
  // Do the rendering
  virtual void render(GemState *state);
  //////////
  // Clear the dirty flag on the pixBlock
  virtual void postrender(GemState *state);
  //////////
  virtual void startRendering();
  //////////
  // Delete texture object
  virtual void stopRendering();

  pix_texture    m_pixtexture;
  //////////
  // Set the texture quality
  // [in] type - if == 0, then GL_NEAREST, else GL_LINEAR
  void          textureQuality(int type){m_pixtexture.textureQuality(type);}
  void          repeatMess(int type){m_pixtexture.repeatMess(type);}
  void          modeMess(int mode){m_pixtexture.m_mode=mode;}

 protected:
	
  //////////
  // static member functions
  static void openMessCallback   (void *data, t_symbol *filename);
  static void changeImageCallback(void *data, t_symbol *, int argc, t_atom *argv);
  static void autoCallback       (void *data, t_floatarg state);

  static void 	textureMessCallback(void *data, t_floatarg n);
  static void 	modeCallback(void *data, t_floatarg n);
  static void 	repeatMessCallback(void *data, t_floatarg n);

};

#endif	// for header file
