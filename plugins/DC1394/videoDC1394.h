/* ------------------------------------------------------------------
 * GEM - Graphics Environment for Multimedia
 *
 * SPDX-FileCopyrightText: © 2003, IOhannes m zmölnig and the GEM contributors
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * ------------------------------------------------------------------
 */

/*-----------------------------------------------------------------

  video capture backend for GEM using libdc1394

-----------------------------------------------------------------*/

#ifndef _INCLUDE_GEMPLUGIN__VIDEODC1394_VIDEODC1394_H_
#define _INCLUDE_GEMPLUGIN__VIDEODC1394_VIDEODC1394_H_
#include "plugins/videoBase.h"


#ifdef HAVE_LIBDC1394
#include "dc1394/dc1394.h"
#endif

#ifdef HAVE_PTHREAD
# include <pthread.h>
#endif


/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
        pix_video

    Loads in a video

KEYWORDS
    pix

DESCRIPTION

-----------------------------------------------------------------*/
namespace gem
{
namespace plugins
{
class GEM_EXPORT videoDC1394 : public videoBase
{
public:
  //////////
  // Constructor
  videoDC1394(void);

  //////////
  // Destructor
  virtual ~videoDC1394(void);
#ifdef HAVE_LIBDC1394
  ////////
  // open the video-device
  virtual bool           openDevice(gem::Properties&);
  virtual void          closeDevice(void);

  //////////
  // Start up the video device
  // [out] int - returns 0 if bad
  bool            startTransfer(void);
  //////////
  // Stop the video device
  // [out] int - returns 0 if bad
  bool            stopTransfer(void);

  //////////
  // get the next frame
  bool    grabFrame(void);

  //////////
  // Set the video dimensions
  virtual bool            setColor(int);
  virtual bool       enumProperties(gem::Properties&, gem::Properties&);
  virtual void        getProperties(gem::Properties&);
  virtual void        setProperties(gem::Properties&);

  //////////
  // get available devices
  virtual std::vector<std::string>enumerate(void);

protected:

  //-----------------------------------
  // GROUP:     Linux specific video data
  //-----------------------------------

  dc1394camera_t * m_dccamera;

  dc1394video_frame_t * m_dcframe;
  dc1394_t * m_dc;

  imageStruct m_frame;

#endif /* HAVE_LIBDC1394 */
};
};
};

#endif  // for header file
