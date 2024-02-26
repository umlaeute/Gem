/* -----------------------------------------------------------------

GEM - Graphics Environment for Multimedia

Record a sequence of images to a single-file PNM stream.

Copyright (c) 2001-2011 IOhannes m zmölnig. forum::für::umläute. IEM. zmoelnig@iem.at
Copyright (c) 2022 Claude Heiland-Allen. claude@mathr.co.uk
For information on usage and redistribution, and for a DISCLAIMER OF ALL
WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

-----------------------------------------------------------------*/

#ifndef _INCLUDE_GEMPLUGIN__RECORDPNM_RECORDFFMPEG_H_
#define _INCLUDE_GEMPLUGIN__RECORDPNM_RECORDFFMPEG_H_

#include "plugins/record.h"

#include <map>

/*---------------------------------------------------------------
 -------------------------------------------------------------------
  CLASS
  recordPNM

  class for recording video-streams in PNM format (PPM, PGM)

  KEYWORDS
  pix record movie

  DESCRIPTION

  -----------------------------------------------------------------*/
namespace gem
{
namespace plugins
{
class GEM_EXPORT recordPNM : public record
{
public:

  //////////
  // Constructor
  recordPNM(void);

  ////////
  // Destructor
  virtual ~recordPNM(void);

  //////////
  // close the movie file
  // stop recording, close the file and clean up temporary things
  virtual void stop(void);

  //////////
  // open a movie up
  // open the recordPNM "filename" (think better about URIs ?)
  // returns TRUE if opening was successful, FALSE otherwise
  virtual bool start(const std::string&filename, gem::Properties&props);

  //////////
  // initialize the encoder
  // dummyImage provides meta-information (e.g. size) that must not change during the encoding cycle
  // fps is the number of frames per second
  //
  // returns TRUE if init was successful, FALSE otherwise
  virtual bool init(const imageStruct* dummyImage, const double fps);

  //////////
  // write the next frame
  virtual bool write(imageStruct*);

  //////////
  // codecs
  virtual bool setCodec(const std::string&name);
  virtual std::vector<std::string>getCodecs(void);
  virtual const std::string getCodecDescription(const std::string&codecname);

  //////////
  // properties
  virtual bool enumProperties(gem::Properties&props);

  virtual void getProperties(gem::Properties&props) {;};
  virtual bool dialog(void) { return false; }

private:

  /* output stream */
  FILE *m_file;

  /* 3 (rgb) or 1 (gray) */
  int m_channels;

  /* converted image */
  imageStruct m_image;
};
};
};

#endif  // for header file
