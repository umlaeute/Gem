/*-----------------------------------------------------------------
LOG
    GEM - Graphics Environment for Multimedia

    age an image

    Copyright (c) 1997-1999 Mark Danks. mark@danks.org
    Copyright (c) 2001-2002 IOhannes m zmoelnig. forum::für::umläute. IEM. zmoelnig@iem.kug.ac.at
    Copyright (c) 2002 James Tittle & Chris Clepper
    For information on usage and redistribution, and for a DISCLAIMER OF ALL
    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

    this is based on EffecTV by Fukuchi Kentauro
    * AgingTV - film-aging effect.
    * Copyright (C) 2001 FUKUCHI Kentarou

-----------------------------------------------------------------*/

#ifndef INCLUDE_PIX_AGING_H_
#define INCLUDE_PIX_AGING_H_

#include "Pixes/pix_fx.h"

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
    pix_aging
    
    Make pix look old

KEYWORDS
    pix
    
DESCRIPTION
   
-----------------------------------------------------------------*/
typedef struct _scratch
{
	int life;
	int x;
	int dx;
	int init;
} t_scratch;

class GEM_EXTERN pix_aging : public pix_fx
{
    CPPEXTERN_HEADER(pix_aging, GemPixObj)

    public:

	    //////////
	    // Constructor
    	pix_aging();
    	
    protected:
    	
    	//////////
    	// Destructor
    	virtual ~pix_aging();

    	//////////
    	// Do the processing
    	virtual void 	processFX(imageStruct &image);
        
    	//////////
    	// Do the processing
    	virtual void 	processYUVfx(imageStruct &image);
#if 1
    	//////////
    	// Do the processing - this is a no-op
    	virtual void 	processGrayFX(imageStruct &image){
	  processFX(image);
	}
#endif

	void scratchMess(int scratchlines);

	////////
	// which FX do we want ?
	int m_coloraging;
	int m_scratching;
	int m_pits;
	int m_dusts;


	////////
	// scratching
	t_scratch *m_scratch;
	int m_scratchlines;

	////////
	// pits
	int m_pitinterval;
	int m_areascale;

	////////
	// dust
	int m_dustinterval;

 private:
	
	//////////
	// Static member callbacks
	static void colorMessCallback(void *dump, t_float state);
	static void scratchMessCallback(void *dump, t_float state);
	static void pitsMessCallback(void *dump, t_float state);
	static void dustMessCallback(void *dump, t_float state);
};

#endif	// for header file
