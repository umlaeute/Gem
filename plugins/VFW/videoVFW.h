/* ------------------------------------------------------------------
 * GEM - Graphics Environment for Multimedia
 *
 * SPDX-FileCopyrightText: © 1997, Mark Danks and the GEM contributors
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * ------------------------------------------------------------------
 */

/*-----------------------------------------------------------------

  video capture backend for GEM using Video-For-Windows

-----------------------------------------------------------------*/

#ifndef _INCLUDE_GEMPLUGIN__VIDEOVFW_VIDEOVFW_H_
#define _INCLUDE_GEMPLUGIN__VIDEOVFW_VIDEOVFW_H_

#include "plugins/videoBase.h"
#include <windows.h>
#include <vfw.h>

/*-----------------------------------------------------------------
  -------------------------------------------------------------------
  CLASS
        pix_video

  captures a video on NT machines

  KEYWORDS
  pix

  -----------------------------------------------------------------*/
namespace gem
{
namespace plugins
{
class GEM_EXPORT videoVFW : public videoBase
{
public:
  //////////
  // Constructor
  videoVFW(void);

  //////////
  // Destructor
  virtual ~videoVFW(void);

  ////////
  // open the video-device
  virtual bool           openDevice(gem::Properties&);
  virtual void          closeDevice(void);

  //////////
  // Start up the video device
  // [out] int - returns 0 if bad
  bool startTransfer(void);
  //////////
  // Stop the video device
  // [out] int - returns 0 if bad
  bool stopTransfer(void);

  //////////
  // get the next frame
  bool grabFrame(void);

  //////////
  // Set the video dimensions
  virtual bool                setColor(int);

  virtual bool enumProperties(gem::Properties&readable,
                              gem::Properties&writeable);
  virtual void setProperties(gem::Properties&);
  virtual void getProperties(gem::Properties&);

protected:
  HWND                m_hWndC;
  void                videoFrame(LPVIDEOHDR lpVHdr);
private:
  static void videoFrameCallback(HWND hWnd, LPVIDEOHDR lpVHdr);
};
};
};

#endif  // for header file
