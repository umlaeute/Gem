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

  Generate a noise texture

  KEYWORDS
  pix

  DESCRIPTION
  send a bang to generate a noise texture

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
	virtual ~pix_noise(void);

	//////////
	// Do the rendering
	virtual void 	render(GemState *state);

	//////////
	// Clear the dirty flag on the pixBlock
	virtual void 	postrender(GemState *state);

	//////////
	virtual void	startRendering(void);

	//////////
	// Clean up the pixBlock
	void	    	cleanPixBlock(void);
	
	//////////
	// Generate noise texture
	void	    	generateNoise(void);
	//////////
	// initialize random generator
	void	    	initRandom(int seed);	
	//////////
	// compute random
	unsigned char	random(void);

	//////////
	// trigger new images
	void	    	bang(void);
	void	    	autoMess(bool);
	void	    	debug(void);
	
	//////////
	// Set color mode
	void	    	RGBAMess(void);
	void	    	RGBMess(void);
	void	    	GREYMess(void);
	//////////
	// Set a new image size
	void	    	SETMess(int xsize, int ysize);

	//////////
	// Manual writing
	bool            m_banged;
	
	//////////
	// Automatic writing
	bool            m_automatic;
	
	//////////
	// paint mode
	int 	    	m_mode;
	
	//////////
	// internal random value
	int				m_rand[55];
	int				m_rand_p,m_rand_k;
	
	//////////
	// The pixBlock with the current image
	pixBlock    	m_pixBlock;
};

#endif	// for header file
