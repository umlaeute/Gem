/*-----------------------------------------------------------------

GEM - Graphics Environment for Multimedia

Load a video into a pix block

Copyright (c) 1997-1999 Mark Danks. mark@danks.org
Copyright (c) Günther Geiger. geiger@epy.co.at
Copyright (c) 2001-2002 IOhannes m zmoelnig. forum::für::umläute. IEM. zmoelnig@iem.kug.ac.at
For information on usage and redistribution, and for a DISCLAIMER OF ALL
WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

Linux version by Millier Puckette. msp@ucsd.edu

-----------------------------------------------------------------*/

#ifndef INCLUDE_PIX_VIDEO_H_
#define INCLUDE_PIX_VIDEO_H_

#include "Base/GemBase.h"
#include "plugins/video.h"

/*-----------------------------------------------------------------
  -------------------------------------------------------------------
  CLASS
  pix_video
    
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
class GEM_EXTERN pix_video : public GemBase
{
  CPPEXTERN_HEADER(pix_video, GemBase)
    
    public:

  //////////
  // Constructor
  pix_video();
  
 protected:
    	
  //////////
  // Destructor
  virtual ~pix_video();

  //////////
  // Do the rendering
  virtual void 	render(GemState *state);
  //////////
  // Clear the dirty flag on the pixBlock
  virtual void 	postrender(GemState *state);
  //////////
  virtual void	startRendering();
  //////////
  // If you care about the stop of rendering
  virtual void	stopRendering();


  //////////
  // closes the currently running backend (if there was one)
  // then starts a new backend, based on selected driver and/or device
  // returns true, if a new backend could be found
  virtual bool restart(void);
  
  //////////
  // Set the video dimensions
  virtual void	dimenMess(int x, int y, int leftmargin = 0, int rightmargin = 0 ,
                          int topmargin = 0 , int bottommargin = 0);
  // Set the channel of the capturing device 
  virtual void	channelMess(int channel, t_float freq=0);
  // Set the channel of the capturing device 
  virtual void	normMess(t_symbol *s);
  // Set the color-space
  virtual void	colorMess(t_atom*);
  // Set the device
  virtual void	deviceMess(t_symbol*dev);
  virtual void	deviceMess(int dev);

  virtual void	closeMess(void);


  // Set the driver architecture; (probably this makes only sense under linux right now: you can choose between video4linux(0) and video1394(1))
  virtual void	driverMess(int dev);
  virtual void	driverMess(t_symbol*s);
  virtual void	driverMess(void);

  // List the available devices
  virtual void 	enumerateMess();

  // fire the format dialogs
  virtual void	dialogMess(int,t_atom*);

  // Set the quality for DV decoding
  virtual void	qualityMess(int dev);
        
  //-----------------------------------
  // GROUP:	Video data
  //-----------------------------------
    
  gem::video *m_videoHandle;
  std::vector<std::string>m_ids;
  std::vector<gem::video*>m_videoHandles;
  
  virtual bool addHandle(std::vector<std::string>available_ids, std::string id=std::string(""));

  int    m_driver;



  bool m_running;
  virtual void	runningMess(bool);

 private:
    	
  //////////
  // static member functions

  static void dimenMessCallback(void *data, t_symbol *s, int ac, t_atom *av);
  static void channelMessCallback(void *data, t_symbol*,int,t_atom*);
  static void normMessCallback(void *data, t_symbol*format);
  static void modeMessCallback(void *data, t_symbol*,int,t_atom*);
  static void colorMessCallback(void *data, t_symbol*,int,t_atom*);
  static void deviceMessCallback(void *data, t_symbol*,int,t_atom*);
  static void driverMessCallback(void *data, t_symbol*,int,t_atom*);
  static void dialogMessCallback(void *data, t_symbol*,int,t_atom*);
  static void enumerateMessCallback(void *data);
  static void qualityMessCallback(void *data, t_floatarg dev);

  static void closeMessCallback(void *data);
  static void openMessCallback(void *data);
  static void runningMessCallback(void *data, t_floatarg dev);

};

#endif	// for header file
