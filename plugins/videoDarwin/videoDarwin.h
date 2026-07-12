/* ------------------------------------------------------------------
 * GEM - Graphics Environment for Multimedia
 *
 * SPDX-FileCopyrightText: © 2002, James Tittle II and the GEM contributors
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * ------------------------------------------------------------------
 */

/*-----------------------------------------------------------------

  Darwin video backend for Gem

-----------------------------------------------------------------*/

#ifndef _INCLUDE_GEMPLUGIN__VIDEODARWIN_VIDEODARWIN_H_
#define _INCLUDE_GEMPLUGIN__VIDEODARWIN_VIDEODARWIN_H_

#include "plugins/videoBase.h"

#ifdef HAVE_CARBON
# include <Carbon/Carbon.h>
#endif

#ifdef HAVE_QUICKTIME
# include <QuickTime/QuickTime.h>
#endif

/*-----------------------------------------------------------------
  -------------------------------------------------------------------
  CLASS
  pix_video

  captures a video on Apple machines

  KEYWORDS
  pix

  -----------------------------------------------------------------*/
namespace gem
{
namespace plugins
{
class GEM_EXPORT videoDarwin : public videoBase
{
public:
  //////////
  // Constructor
  videoDarwin(void);

  //////////
  // Destructor
  virtual ~videoDarwin(void);

  ////////
  // open the video-device
  virtual bool           openDevice(gem::Properties&props);
  virtual void          closeDevice(void);

  //////////
  // Start up the video device
  // [out] int - returns 0 if bad
  bool         startTransfer(void);
  //////////
  // Stop the video device
  // [out] int - returns 0 if bad
  bool         stopTransfer(void);

  //////////
  // get the next frame
  virtual bool grabFrame(void);
  //////////
  // Set the video dimensions
  virtual bool setColor(int d);
  virtual bool dialog(std::vector<std::string>dlg);
  virtual std::vector<std::string>dialogs(void);

  virtual bool enumProperties(gem::Properties&readable,
                              gem::Properties&writeable);
  virtual void setProperties(gem::Properties&props);
  virtual void getProperties(gem::Properties&props);

protected:
  bool initSeqGrabber(void);
  void resetSeqGrabber(void);
  void destroySeqGrabber(void);
  virtual void    setupCapture(void);

  int          m_newFrame;
  SeqGrabComponent     m_sg;           // Sequence Grabber Component
  SGChannel                    m_vc;                   // Video Channel
  GWorldPtr                    m_srcGWorld;    // Capture Destination
  unsigned long                m_rowBytes;             // Row bytes in a row
  long                         m_quality;
  int                          m_colorspace;

  int                          m_inputDevice;
  int                          m_inputDeviceChannel;
  VideoDigitizerComponent      m_vdig; //gotta have the vdig
  DigitizerInfo                m_vdigInfo; //the info about the VDIG

  virtual std::vector<std::string>enumerate(void);
  std::vector<std::string>m_devices;

  bool setIIDCProperty(OSType specifier, double value);
  bool applyProperties(gem::Properties&props);

  imageStruct m_img;
};
};
};

#endif  // for header file
