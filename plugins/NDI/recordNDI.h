/* ------------------------------------------------------------------
 * GEM - Graphics Environment for Multimedia
 *
 * SPDX-FileCopyrightText: © 2020, IOhannes m zmölnig and the GEM contributors
 * SPDX-License-Identifier: GPL-3.0-or-later WITH LicenseRef-GPL3-linking-exception-NDI
 *
 * ------------------------------------------------------------------
 */

/* -----------------------------------------------------------------

   video output backend for GEM using NDI

-----------------------------------------------------------------*/

#ifndef _INCLUDE_GEMPLUGIN__RECORDNDI_RECORDNDI_H_
#define _INCLUDE_GEMPLUGIN__RECORDNDI_RECORDNDI_H_

#include "plugins/record.h"

#include "Processing.NDI.Lib.h"

/*---------------------------------------------------------------
 -------------------------------------------------------------------
  CLASS
  recordNDI

  class for outputting video using a v4l2 loopback device

  KEYWORDS
  pix record movie

  DESCRIPTION

  -----------------------------------------------------------------*/
namespace gem
{
namespace plugins
{
class GEM_EXPORT recordNDI : public record
{
public:

  //////////
  // Constructor

  recordNDI(void);

  ////////
  // Destructor
  /* free what is appropriate */
  virtual ~recordNDI(void);

  //////////
  // close the movie file
  // stop recording, close the file and clean up temporary things
  virtual void stop(void);

  //////////
  // reserve a stream-name for sending
  // open the recordNDI "filename"
  // returns TRUE if opening was successful, FALSE otherwise
  virtual bool start(const std::string&filename, gem::Properties&);


  //////////
  // initialize the encoder
  // dummyImage provides meta-information.
  // for NDI, we can safely ignore this.
  // framedur is the duration of one frame in [ms]
  // for NDI, i don't know yet
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
   * get a list of supported codecs (short-form names, e.g. "ndi")
   */
  virtual std::vector<std::string>getCodecs(void);
  virtual const std::string getCodecDescription(const std::string&);
  virtual bool enumProperties(gem::Properties&);

  virtual void getProperties(gem::Properties&props) {;};
  virtual bool dialog(void) { return false; }


private:
  NDIlib_send_instance_t m_ndi_send;
};
};
};

#endif  // for header file
