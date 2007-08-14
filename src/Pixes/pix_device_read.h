////////////////////////////////////////////////////////
//
// GEM - Graphics Environment for Multimedia
//
// Header file
//
// Copyright (c) 2007, Thomas Holzmann
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.
//
/////////////////////////////////////////////////////////

#ifndef INCLUDE_PIX_DEVICE_READ_H_
#define INCLUDE_PIX_DEVICE_READ_H_

#include "Base/GemBase.h"
#include "Base/GemPixUtil.h"
#include "../videoIO/VIOKernel.h"
#include "../videoIO/VIOFrame.h"


/*-----------------------------------------------------------------
  -------------------------------------------------------------------
  CLASS
  pix_device_read
    
  Loads in an video
    
  KEYWORDS
  pix
    
  DESCRIPTION

  "dimen" (int, int) - set the x,y dimensions
  "zoom" (int, int) - the zoom factor (1.0 is nominal) (num / denom)
  "bright" (int) - the brightnes
  "contrast" (int) - the contrast
  "hue" (int) - the hue
  "sat" (int) - the saturation
    
  -----------------------------------------------------------------*/
class GEM_EXTERN pix_device_read : public GemBase
{
  CPPEXTERN_HEADER(pix_device_read, GemBase)
    
  public:

  //////////
  // Constructor
  pix_device_read();
  
  protected:
    	
  //////////
  // Destructor
  virtual ~pix_device_read();

  //////////
  // Do the rendering
  virtual void 	render(GemState *state);
  //////////
  // Clear the dirty flag on the pixBlock
  virtual void 	postrender(GemState *state);
  //////////
  // Opens the specified device
  virtual void	openDevice(t_symbol *dev);
  virtual void	openDevice(int dev);
  //////////
  // If you care about the stop of rendering
  virtual void	closeDevice();
  // Set the channel of the capturing device 
  virtual void	channelMess(int channel, t_float freq=0);
  // Set the color norm of the capturing device (e.g. PAL or NTSC) 
  virtual void	normMess(t_symbol *s);
  // Set the color-space
  virtual void	forceColorspace(t_symbol *cs);


  // List the available devices
  virtual void 	enumerateMess();

  // Set the quality for DV decoding
  virtual void	qualityMess(int dev);
        
  //-----------------------------------
  // GROUP:	Video data
  //-----------------------------------
    
  pixBlock m_image;
  
   // the device reader
  VideoIO_::DeviceRead *m_deviceReader;
  /// TODO kernel should be used only once
  VideoIO_::VIOKernel m_kernel;
  
 private:
   
  // reallocate frame data
  void reallocate_m_image();
    	
  //////////
  // static member functions

  static void dimenMessCallback(void *data, t_symbol *s, int ac, t_atom *av);
  static void channelMessCallback(void *data, t_symbol*,int,t_atom*);
  static void normMessCallback(void *data, t_symbol*format);
  static void csCallback(void *data, t_symbol*cs);
  static void deviceMessCallback(void *data, t_symbol*,int,t_atom*);
  static void enumerateMessCallback(void *data);
  static void qualityMessCallback(void *data, t_floatarg dev);

};

#endif	// for header file
