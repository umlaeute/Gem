/*-----------------------------------------------------------------
LOG
    GEM - Graphics Environment for Multimedia

    play puzzle with a sequence of pixBufs

    Copyright (c) 1997-1999 Mark Danks. mark@danks.org
    Copyleft  (l) 2001 IOhannes m zmölnig. zmoelnig@iem.kug.ac.at
    Copyleft (l) 2002 James Tittle & Chris Clepper
    For information on usage and redistribution, and for a DISCLAIMER OF ALL
    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

-----------------------------------------------------------------*/

#ifndef INCLUDE_PIX_DOT_H_
#define INCLUDE_PIX_DOT_H_

#include "Pixes/pix_fx.h"

/*-----------------------------------------------------------------
  -------------------------------------------------------------------
  CLASS
  pix_dot
  
  change the pixBuf into dots
  
  KEYWORDS
  pix
    
  DESCRIPTION
   
  -----------------------------------------------------------------*/
class GEM_EXTERN pix_dot : public pix_fx
{
  CPPEXTERN_HEADER(pix_dot, GemPixObj)

    public:

  //////////
  // Constructor
  pix_dot();
    	
 protected:
    	
  //////////
  // Destructor
  virtual ~pix_dot();

  //////////
  // Do the processing
  virtual void 	processFX(imageStruct &image);
  virtual void 	processYUVfx(imageStruct &image);

  imageStruct    myImage;

  //////////
  // Make a puzzle
  //virtual void 	makePuzzleBlocks(int xsize, int ysize, int csize);
  virtual void  makePattern();
  virtual void  sizeMess(int width, int height);
 
  int blocksize, blockxsize,blockysize,  blocknum, spacepos;
  int blockw, blockh;
  int *blockpos;
  int *blockoffset;
  int marginw, marginh;

  int m_force;

 private:
  
  //////////
  // static member functions
  static void bangMessCallback(void *data);
  static void sizeMessCallback(void *data, t_floatarg width, t_floatarg height);
  static void stateMessCallback(void *data, t_floatarg state);
};

#endif	// for header file
