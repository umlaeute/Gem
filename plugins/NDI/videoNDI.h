/* ------------------------------------------------------------------
 * GEM - Graphics Environment for Multimedia
 *
 * SPDX-FileCopyrightText: © 2020, IOhannes m zmölnig and the GEM contributors
 * SPDX-License-Identifier: GPL-3.0-or-later WITH GPL-3.0-linking-exception
 *
 * ------------------------------------------------------------------
 */

/*-----------------------------------------------------------------

  video capture backend for GEM using NDI

-----------------------------------------------------------------*/

#ifndef _INCLUDE_GEMPLUGIN__VIDEONDI_VIDEONDI_H_
#define _INCLUDE_GEMPLUGIN__VIDEONDI_VIDEONDI_H_

#include "plugins/video.h"
#include "Gem/Image.h"

#include "Processing.NDI.Lib.h"
#if 1
# define MARK()
#else
# define MARK() std::cerr << "!!! " << __func__ << "[" << __LINE__ << "]" << std::endl
#endif

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
        videoNDI

    access video-devices through the NDI API (Network Video Streams)

KEYWORDS
    pix

-----------------------------------------------------------------*/
namespace gem
{
namespace plugins
{
class GEM_EXPORT videoNDI : public video
{
public:
  //////////
  // Constructor
  videoNDI(void);

  //////////
  // Destructor
  virtual ~videoNDI(void);

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
  virtual bool isThreadable(void)
  {
    MARK();
    return false;
  }
  virtual bool grabAsynchronous(bool);

  // color conversion
  virtual bool         setColor(int);

protected:
  NDIlib_recv_instance_t m_ndi_recv;
  NDIlib_video_frame_v2_t m_ndi_frame;
  NDIlib_find_instance_t m_ndi_find;

  std::string m_name;
  std::vector<std::string>m_provides;
  /* device selection by name/number */
  std::string m_devicename; // "Mini Recorder #1"?
  int m_devicenum; // selected device number (or -1)

  pixBlock m_pixBlock;

  bool m_async;

  /* we grabbed a frame (release it) */
  bool m_gotone;
};
};
};

#endif  // for header file
