/*-----------------------------------------------------------------

GEM - Graphics Environment for Multimedia

Copyright (c) 2012 IOhannes m zmölnig. forum::für::umläute. IEM. zmoelnig@iem.at
For information on usage and redistribution, and for a DISCLAIMER OF ALL
WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

-----------------------------------------------------------------*/

#ifndef _INCLUDE_GEMPLUGIN__VIDEOOPTITRACK_VIDEOOPTITRACK_H_
#define _INCLUDE_GEMPLUGIN__VIDEOOPTITRACK_VIDEOOPTITRACK_H_

#include "plugins/video.h"
#include "Gem/Image.h"
#include "Gem/Properties.h"

/* Natural Point's Camera SDK */
/* ... they really like generic names */
#include <cameralibrary.h>

/*-----------------------------------------------------------------
  -------------------------------------------------------------------
  CLASS
  pix_video

  captures a video using NaturalPoints OptiTrack cameras

  KEYWORDS
  pix

  -----------------------------------------------------------------*/
namespace gem
{
namespace plugins
{
class GEM_EXPORT videoOptiTrack : public video
{
public:
  //////////
  // Constructor
  videoOptiTrack(void);

  //////////
  // Destructor
  virtual ~videoOptiTrack(void);

  virtual bool open(gem::Properties&props);
  virtual pixBlock *getFrame(void);

  virtual std::vector<std::string>enumerate(void);
  virtual bool setDevice(int ID);
  virtual bool setDevice(const std::string&);

  virtual bool enumProperties(gem::Properties&readable,
                              gem::Properties&writeable);
  virtual void setProperties(gem::Properties&props);
  virtual void getProperties(gem::Properties&props);

  virtual std::vector<std::string>dialogs(void);
  // for pix_video: query whether this backend provides access to this class of devices
  // (e.g. "dv")
  virtual bool provides(const std::string&);
  // get a list of all provided devices
  virtual std::vector<std::string>provides(void);

  // get's the name of the backend (e.g. "v4l")
  virtual const std::string getName(void);

  virtual bool isThreadable(void)
  {
    return true;
  }
  virtual bool reset(void);
  virtual void releaseFrame(void);
  virtual bool grabAsynchronous(bool)
  {
    return true;
  }
  virtual bool dialog(std::vector<std::string>names=
                        std::vector<std::string>())
  {
    return false;
  }
  virtual bool setColor(int)
  {
    return false;
  }

  virtual void close(void);
  virtual bool start(void);
  virtual bool stop(void);

protected:
  gem::Properties m_props;
  std::string m_devname;
  int m_devnum;
  int m_quality;

  CameraLibrary::Camera*m_camera;
  CameraLibrary::Frame *m_frame;

  bool m_resize;
  pixBlock m_pixBlock;

};
};
};


#endif  // for header file
