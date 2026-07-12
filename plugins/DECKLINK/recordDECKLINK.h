/* ------------------------------------------------------------------
 * GEM - Graphics Environment for Multimedia
 *
 * SPDX-FileCopyrightText: © 2021, IOhannes m zmölnig and the GEM contributors
 * SPDX-License-Identifier: GPL-3.0-or-later WITH GPL-3.0-linking-exception
 *
 * ------------------------------------------------------------------
 */

/* -----------------------------------------------------------------

   video output plugin for GEM using DECKLINK

-----------------------------------------------------------------*/

#ifndef _INCLUDE_GEMPLUGIN__RECORDDECKLINK_RECORDDECKLINK_H_
#define _INCLUDE_GEMPLUGIN__RECORDDECKLINK_RECORDDECKLINK_H_

#include "plugins/record.h"
#include "Gem/Image.h"
#include "Utils/ThreadMutex.h"

#include <DeckLinkAPI.h>

/*---------------------------------------------------------------
 -------------------------------------------------------------------
  CLASS
  recordDECKLINK

  class for outputting video using a v4l2 loopback device

  KEYWORDS
  pix record movie

  DESCRIPTION

  -----------------------------------------------------------------*/
namespace gem
{
namespace plugins
{
class GEM_EXPORT recordDECKLINK : public record
{
public:

  //////////
  // Constructor

  recordDECKLINK(void);

  ////////
  // Destructor
  /* free what is appropriate */
  virtual ~recordDECKLINK(void);

  //////////
  // close the movie file
  // stop recording, close the file and clean up temporary things
  virtual void stop(void);

  //////////
  // reserve a stream-name for sending
  // open the Decklink device "filename"
  // returns TRUE if opening was successful, FALSE otherwise
  virtual bool start(const std::string&filename, gem::Properties&);


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
  pixBlock m_pixBlock;

  IDeckLink*m_dl;
  IDeckLinkOutput* m_dlOutput;
  IDeckLinkDisplayMode*m_displayMode;
  IDeckLinkConfiguration *m_dlConfig;
  BMDVideoConnection m_connectionType;
  IDeckLinkVideoOutputCallback*m_dlCallback;
  BMDPixelFormat m_pixelFormat;
};
};
};

#endif  // for header file
