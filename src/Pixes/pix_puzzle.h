/*-----------------------------------------------------------------
LOG
    GEM - Graphics Environment for Multimedia

    play puzzle with a sequence of pixBufs

    Copyright (c) 1997-1999 Mark Danks. mark@danks.org
    Copyleft  (l) 2001 IOhannes m zmölnig. zmoelnig@iem.kug.ac.at
    For information on usage and redistribution, and for a DISCLAIMER OF ALL
    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

-----------------------------------------------------------------*/

#ifndef INCLUDE_PIX_PUZZLE_H_
#define INCLUDE_PIX_PUZZLE_H_

#include "Pixes/pix_fx.h"

/*-----------------------------------------------------------------
  -------------------------------------------------------------------
  CLASS
  pix_puzzle
  
  shuffle areas of the pixBuf
  
  KEYWORDS
  pix
    
  DESCRIPTION
   
  -----------------------------------------------------------------*/
class GEM_EXTERN pix_puzzle : public pix_fx
{
  CPPEXTERN_HEADER(pix_puzzle, GemPixObj)

    public:

  //////////
  // Constructor
  pix_puzzle();
    	
 protected:
    	
  //////////
  // Destructor
  virtual ~pix_puzzle();

  //////////
  // Do the processing
  virtual void 	processFX(imageStruct &image);

  imageStruct    myImage;

  //////////
  // Make a puzzle
  virtual void 	makePuzzleBlocks(int xsize, int ysize, int csize);
  virtual void  shuffle();
  virtual void  sizeMess(int width, int height);
  virtual void  moveMess(int direction);
 
  int blocksize, blockxsize,blockysize,  blocknum, spacepos;
  int blockw, blockh;
  int *blockpos;
  int *blockoffset;
  int marginw, marginh;

  int m_force;

  //////////
  // anyone wants to play the famous puzzle game ?
  int m_game;

 private:
  
  //////////
  // static member functions
  static void bangMessCallback(void *data);
  static void sizeMessCallback(void *data, t_floatarg width, t_floatarg height);
  static void stateMessCallback(void *data, t_floatarg state);
  static void moveMessCallback(void *data, t_floatarg state);
};

#endif	// for header file
