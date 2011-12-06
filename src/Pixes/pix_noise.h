/*-----------------------------------------------------------------

 GEM - Graphics Environment for Multimedia

 generate a noise pix texture

 Copyright (c) 1997-1999 Mark Danks. mark@danks.org
 Copyright (c) Günther Geiger. geiger@epy.co.at
 Copyright (c) 2001-2011 IOhannes m zmölnig. forum::für::umläute. IEM. zmoelnig@iem.at
 For information on usage and redistribution, and for a DISCLAIMER OF ALL
 WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

-----------------------------------------------------------------*/

/*-----------------------------------------------------------------
pix_noise

  2011
  Nicolas Montgermont
  mailto:nicolas_montgermont@yahoo.fr
-----------------------------------------------------------------*/

#ifndef _INCLUDE__GEM_PIXES_PIX_NOISE_H_
#define _INCLUDE__GEM_PIXES_PIX_NOISE_H_

#include "Base/GemPixObj.h"

#include "Base/GemBase.h"
#include "Gem/Image.h"
#include "Gem/Cache.h"

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
pix_noise

 set the pixels via a float-package

  KEYWORDS
  pix

   DESCRIPTION

    "RGB"
    "RGBA"
    "GREY"

-----------------------------------------------------------------*/
class GEM_EXTERN pix_noise : public GemPixObj
{
    CPPEXTERN_HEADER(pix_noise, GemPixObj);

public:

	//////////
	// Constructor
	pix_noise(t_floatarg xsize, t_floatarg ysize);

protected:

	//////////
	// Destructor
	virtual ~pix_noise();

	//////////
	// Do the rendering
	virtual void 	render(GemState *state);

	//////////
	// Clear the dirty flag on the pixBlock
	virtual void 	postrender(GemState *state);

	//////////
	virtual void	startRendering();

	//////////
	// Clean up the pixBlock
	void	    	cleanPixBlock();
	
	//////////
	// new texture
	void	    	bangMess();
	
	//////////
	// compute random
	float	    	pix_random();
	
	//////////
	// Set to RGBA-mode
	void	    	RGBAMess();
	//////////
	// Set to RGB-mode
	void	    	RGBMess();
	//////////
	// Set to GREYSCALE-mode
	void	    	GREYMess();
	//////////
	// Set a new image size
	void	    	SETMess(int xsize, int ysize);
	//////////
	// Pass the data
	void	    	DATAMess(int argc, t_atom *argv);


	//-----------------------------------
	// GROUP:	Paint data
	//-----------------------------------

	//////////
	// paint mode
	int 	    	m_mode;
	int				randNow;

	//////////
	// The pixBlock with the current image
	pixBlock    	m_pixBlock;
	imageStruct     m_imageStruct;

private:

	//////////
	// static member functions
	static void bangMessCallback(void *data);
	static void RGBAMessCallback(void *data);
	static void RGBMessCallback(void *data);
	static void GREYMessCallback(void *data);
	static void SETMessCallback(void *data, t_float x, t_float y);
	static void YUVMessCallback(void *data);


	static void DATAMessCallback(void *data, t_symbol *, int argc, t_atom *argv);
};

#endif	// for header file
