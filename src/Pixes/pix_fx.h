/*-----------------------------------------------------------------
LOG
    GEM - Graphics Environment for Multimedia

    effect-processing for pixels

    Copyleft  (l) 2002 IOhannes m zmölnig. zmoelnig@iem.kug.ac.at
    For information on usage and redistribution, and for a DISCLAIMER OF ALL
    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

-----------------------------------------------------------------*/

#ifndef INCLUDE_PIX_FX_H_
#define INCLUDE_PIX_FX_H_

#include "Base/GemPixObj.h"

/*-----------------------------------------------------------------
  -------------------------------------------------------------------
  CLASS
  pix_fx
    
  parent class for pix-effects

  KEYWORDS
  pix
    
  DESCRIPTION
   
  -----------------------------------------------------------------*/
class GEM_EXTERN pix_fx : public GemPixObj
{
  CPPEXTERN_HEADER(pix_fx, GemPixObj)

    public:

  //////////
  // Constructor
  pix_fx();
    	
 protected:
    	
  //////////
  // Destructor
  virtual ~pix_fx();

  //////////
  // insert FX into the chain
  virtual void processImage(imageStruct &image);

  //////////
  // insert FX into the chain
  virtual void processGrayImage(imageStruct &image);

  //////////
  // turn the pointer back to the old data after rendering
  virtual void postrender(GemState *state);


  //////////
  // Do the processing
  virtual void processFX(imageStruct &image){ }

  //////////
  // turn this FX on/off
  virtual void 	activate(int state);
  //////////
  // is this FX on/off
  int m_active;



  //////////
  // the original image
  imageStruct  *oldImage;
  int           old_xsize;
  int           old_ysize;
  int           old_csize;
  int           old_format;    
  unsigned char*old_data;

 private:
  static void stateMessCallback(void *data, t_floatarg state);
};
#endif	// for header file
