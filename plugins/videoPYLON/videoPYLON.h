/*-----------------------------------------------------------------

    GEM - Graphics Environment for Multimedia

    Load an video into a pix block

    Copyright (c) 1997-1999 Mark Danks. mark@danks.org
    Copyright (c) Günther Geiger. geiger@epy.co.at
    Copyright (c) 2001-2011 IOhannes m zmölnig. forum::für::umläute. IEM. zmoelnig@iem.at
    For information on usage and redistribution, and for a DISCLAIMER OF ALL
    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

    Linux version by Miller Puckette. msp@ucsd.edu

-----------------------------------------------------------------*/

#ifndef _INCLUDE_GEMPLUGIN__VIDEOPYLON_VIDEOPYLON_H_
#define _INCLUDE_GEMPLUGIN__VIDEOPYLON_VIDEOPYLON_H_

#include "plugins/video.h"
#include "Gem/Image.h"

#include <map>

#ifdef Status
/* ouch: Xlib.h defines "Status" as "int", but Pylon uses "Status" as a
 * variable name
 */
# undef Status
#endif
#ifdef None
# undef None
#endif

#include "pylon/PylonIncludes.h"
#if 1
# define MARK()
#else
# define MARK() std::cerr << "!!! " << __func__ << "[" << __LINE__ << "]" << std::endl
#endif

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
        videoPYLON

    access video-devices through the Pylon API (Industrial grade cameras, e.g. GigE)

KEYWORDS
    pix

-----------------------------------------------------------------*/
namespace gem
{
namespace plugins
{
class GEM_EXPORT videoPYLON : public video
{
public:
  //////////
  // Constructor
  videoPYLON(void);

  //////////
  // Destructor
  virtual ~videoPYLON(void);

  /* information about capabilities */
  virtual const std::string getName(void);
  virtual bool provides(const std::string&);
  virtual std::vector<std::string>provides(void);

  /* dialog handling: none */
  virtual std::vector<std::string>dialogs(void)
  {
    return std::vector<std::string>();
  }
  virtual bool dialog(std::vector<std::string>names=
                      std::vector<std::string>())
  {
    return false;
  }

  /* property handling */
  virtual bool enumProperties(gem::Properties&readable,
                              gem::Properties&writeable);
  virtual void setProperties(gem::Properties&writeprops);
  virtual void getProperties(gem::Properties&readprops);

  virtual std::vector<std::string>enumerate(void);
  virtual bool setDevice(int ID);
  virtual bool setDevice(const std::string&);

  ////////
  // open the video-device
  virtual bool open(gem::Properties&writeprops);
  virtual void close(void);

  //////////
  // Start/Stop capturing
  virtual bool start(void);
  virtual bool stop(void);

  virtual bool reset(void);

  //////////
  // get the next frame
  virtual pixBlock *getFrame(void);
  virtual void releaseFrame(void);

  // threading
  virtual bool isThreadable(void) { MARK(); return false; }
  virtual bool grabAsynchronous(bool);

  // color conversion
  virtual bool         setColor(int);

protected:
  class CGrabBuffer;

  Pylon::PylonAutoInitTerm autoInitTerm;
  Pylon::CTlFactory*m_factory;
  Pylon::CInstantCamera m_camera;

  std::string m_name;
  std::vector<std::string>m_provides;
  /* device selection by name/number */
  std::string m_devicename; // "Mini Recorder #1"?
  int m_devicenum; // selected device number (or -1)

  pixBlock m_pixBlock;
  unsigned int m_width, m_height;
  bool m_async;
  Pylon::EGrabLoop m_grabloop;
};
};
};

#endif  // for header file
