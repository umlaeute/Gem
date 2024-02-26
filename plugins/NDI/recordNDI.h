/* -----------------------------------------------------------------

GEM - Graphics Environment for Multimedia

Load an digital video (like AVI, Mpeg, Quicktime) into a pix block
(OS-independent parent-class)

Copyright (c) 1997-1999 Mark Danks. mark@danks.org
Copyright (c) Günther Geiger. geiger@epy.co.at
Copyright (c) 2001-2011 IOhannes m zmölnig. forum::für::umläute. IEM. zmoelnig@iem.at
For information on usage and redistribution, and for a DISCLAIMER OF ALL
WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

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
