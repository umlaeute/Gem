/*-----------------------------------------------------------------

GEM - Graphics Environment for Multimedia

Load a video into a pix block

Copyright (c) 1997-1999 Mark Danks. mark@danks.org
Copyright (c) Günther Geiger. geiger@epy.co.at
Copyright (c) 2001-2011 IOhannes m zmölnig. forum::für::umläute. IEM. zmoelnig@iem.at
For information on usage and redistribution, and for a DISCLAIMER OF ALL
WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

-----------------------------------------------------------------*/

#ifndef _INCLUDE__GEM_PIXES_PIX_VIDEO_H_
#define _INCLUDE__GEM_PIXES_PIX_VIDEO_H_

#include "Base/GemBase.h"
#include "plugins/video.h"

#include "Gem/Properties.h"

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
  CPPEXTERN_HEADER(pix_video, GemBase);

public:

  //////////
  // Constructor
  pix_video(int, t_atom*);

protected:

  //////////
  // Destructor
  virtual ~pix_video();

  //////////
  // Do the rendering
  virtual void  render(GemState *state);
  //////////
  // Clear the dirty flag on the pixBlock
  virtual void  postrender(GemState *state);
  //////////
  virtual void  startRendering();
  //////////
  // If you care about the stop of rendering
  virtual void  stopRendering();


  //////////
  // closes the currently running backend (if there was one)
  // then starts a new backend, based on selected driver and/or device
  // returns true, if a new backend could be found
  virtual bool restart(void);

  virtual bool  deviceMess(std::string);
  virtual bool  deviceMess(int dev);
  virtual bool  deviceMess(t_symbol*, int, t_atom*);

  // Set the driver architecture
  virtual bool  driverMess(int dev);
  virtual bool  driverMess(std::string);
  virtual bool  driverMess(t_symbol*, int, t_atom*);
  virtual void  enumDrivers(const char*selector);

  virtual void  openMess(t_symbol*, int, t_atom*);
  virtual void  closeMess(void);
  virtual void  resetMess(void);

  // List the available devices
  virtual void  enumerateMess();

  // fire the format dialogs
  virtual void  dialogMess(int,t_atom*);


  virtual void  asynchronousMess(bool);

  virtual void  colorMess(t_atom*);
  // Set the device

  //////////
  // Set the video dimensions
  virtual void  dimenMess(int x, int y, int leftmargin = 0,
                          int rightmargin = 0,
                          int topmargin = 0, int bottommargin = 0);
  // Set the channel of the capturing device
  virtual void  channelMess(int channel, t_float freq=0);
  // Set the channel of the capturing device
  virtual void  normMess(std::string);
  // Set the color-space

  // Set the quality for DV decoding
  virtual void  qualityMess(int dev);

  gem::Properties m_readprops, m_writeprops;
  virtual void  setPropertyMess(int argc, t_atom*argv);
  virtual void  getPropertyMess(int argc, t_atom*argv);
  virtual void  enumPropertyMess(void);

  virtual void  setPropertiesMess(int argc, t_atom*argv);
  virtual void  applyPropertiesMess(void);
  virtual void  clearPropertiesMess(void);


  //-----------------------------------
  // GROUP:     Video data
  //-----------------------------------

  gem::plugins::video *m_videoHandle;
  std::vector<std::string>m_ids;
  std::vector<gem::plugins::video*>m_videoHandles;

  virtual bool addHandle(std::vector<std::string>available_ids,
                         std::string id=std::string(""));

  int    m_driver;

  enum runningState {
    UNKNOWN=-1,
    STOPPED=0,
    STARTED=1
  };
  runningState m_running;
  virtual void  runningMess(bool);


  /* an outlet for status messages */
  t_outlet *m_infoOut;

private:

  //////////
  // static member functions


  static void dialogMessCallback(void *data, t_symbol*,int,t_atom*);

  static void getPropertyMessCallback(void *data, t_symbol*,int, t_atom*);
  static void setPropertyMessCallback(void *data, t_symbol*,int, t_atom*);
  static void setPropertiesMessCallback(void *data, t_symbol*,int, t_atom*);

  static void dimenMessCallback(void *data, t_symbol* s, int ac, t_atom *av);
  static void channelMessCallback(void *data, t_symbol*,int,t_atom*);
  static void modeMessCallback(void *data, t_symbol*,int,t_atom*);
  static void colorMessCallback(void *data, t_symbol*,int,t_atom*);
  static void qualityMessCallback(void *data, t_float dev);
};

#endif  // for header file
