/* ------------------------------------------------------------------
 * GEM - Graphics Environment for Multimedia
 *
 * SPDX-FileCopyrightText: © 2006, IOhannes m zmölnig and the GEM contributors
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * ------------------------------------------------------------------
 */

/* -----------------------------------------------------------------

   video recording backend for GEM using V4L2

-----------------------------------------------------------------*/

#ifndef _INCLUDE_GEMPLUGIN__RECORDV4L2_RECORDV4L2_H_
#define _INCLUDE_GEMPLUGIN__RECORDV4L2_RECORDV4L2_H_

#include "plugins/record.h"

#if defined HAVE_LINUX_VIDEODEV2_H
# define HAVE_VIDEO4LINUX2
# include <linux/videodev2.h>
#endif

#include <string>
#include <map>

/*---------------------------------------------------------------
 -------------------------------------------------------------------
  CLASS
  recordV4L2

  class for outputting video using a v4l2 loopback device

  KEYWORDS
  pix record movie

  DESCRIPTION

  -----------------------------------------------------------------*/
#if defined HAVE_VIDEO4LINUX2

namespace gem
{
namespace plugins
{
class GEM_EXPORT recordV4L2 : public record
{
public:

  //////////
  // Constructor

  recordV4L2(void);

  ////////
  // Destructor
  /* free what is appropriate */
  virtual ~recordV4L2(void);

  //////////
  // close the movie file
  // stop recording, close the file and clean up temporary things
  virtual void stop(void);

  //////////
  // open a movie up
  // open the recordV4L2 "filename" (or URI)
  // returns TRUE if opening was successful, FALSE otherwise
  virtual bool start(const std::string&filename, gem::Properties&);


  //////////
  // initialize the encoder
  // dummyImage provides meta-information (e.g. size) that must not change during the encoding cycle
  // (if it does, abort the recording session)
  // framedur is the duration of one frame in [ms]
  //
  // returns TRUE if init was successful, FALSE otherwise
  virtual bool init(const imageStruct* dummyImage, const int framedur);

  //////////
  // compress and write the next frame
  /* this is the core-function of this class !!!!
   * when called it returns TRUE on success
   * if FALSE is returned, recording must be stopped
   */
  virtual bool write(imageStruct*);

  virtual bool setCodec(const std::string&name);

  /**
   * get a list of supported codecs (short-form names, e.g. "mjpa")
   */
  virtual std::vector<std::string>getCodecs(void);
  virtual const std::string getCodecDescription(const std::string&);
  virtual bool enumProperties(gem::Properties&);

  virtual void getProperties(gem::Properties&props) {;};
  virtual bool dialog(void) { return false; }


private:
  int m_fd;
  imageStruct m_image;
  bool m_init;
  int m_palette;
};
};
};
#endif /* V4L2 */

#endif  // for header file
