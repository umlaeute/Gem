/*-----------------------------------------------------------------
LOG
    GEM - Graphics Environment for Multimedia

    change pixBuf into dots

    Copyright (c) 1997-1999 Mark Danks. mark@danks.org
    Copyleft  (l) 2001 IOhannes m zmölnig. zmoelnig@iem.kug.ac.at
    Copyleft (l) 2003 James Tittle
    For information on usage and redistribution, and for a DISCLAIMER OF ALL
    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

-----------------------------------------------------------------*/

#ifndef INCLUDE_PIX_DOT_H_
#define INCLUDE_PIX_DOT_H_

#include "Base/GemPixObj.h"

/*-----------------------------------------------------------------
  -------------------------------------------------------------------
  CLASS
  pix_dot
  
  change the pixBuf into dots
  
  KEYWORDS
  pix
    
  DESCRIPTION
   
  -----------------------------------------------------------------*/
class GEM_EXTERN pix_dot : public GemPixObj
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
  virtual void 	processRGBAImage(imageStruct &image);
  virtual void 	processYUVImage( imageStruct &image );
  virtual void	drawDot( int xx, int yy, unsigned char c, unsigned int *dest );
  virtual void  dot_init();
  virtual void  sampxy_table_init();
  virtual void  yuv_init();
  virtual void  makePattern();
  virtual void  sizeMess(int width, int height);
  int	sharedbuffer_init();
  void	sharedbuffer_reset();
  unsigned char *sharedbuffer_alloc(int size);

  imageStruct    myImage;

  //////////
  // Make dots
    
    int m_xsize, m_ysize;
    unsigned char *sharedbuffer;
    int sharedbuffer_length;

    int tail;
    int alreadyInit;
    int dotw, doth;
    int DOTDEPTH, DOTMAX;
    int dots_width, dots_height;
    int dot_size, dot_hsize;
    int *sampx, *sampy;
    int state;
    unsigned int *pattern;
    unsigned int *heart_pattern;
    int mode;

 private:
  
  //////////
  // static member functions
  static void bangMessCallback(void *data);
  static void sizeMessCallback(void *data, t_floatarg width, t_floatarg height);
  static void stateMessCallback(void *data, t_floatarg state);
};

#endif	// for header file
