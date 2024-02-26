/* -----------------------------------------------------------------

GEM - Graphics Environment for Multimedia

Record a sequence of images to a single-file PIPEWIRE stream.

Copyright (c) 2022 IOhannes m zmölnig. forum::für::umläute. IEM. zmoelnig@iem.at
For information on usage and redistribution, and for a DISCLAIMER OF ALL
WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

-----------------------------------------------------------------*/

#ifndef _INCLUDE_GEMPLUGIN__RECORDPIPEWIRE_RECORDFFMPEG_H_
#define _INCLUDE_GEMPLUGIN__RECORDPIPEWIRE_RECORDFFMPEG_H_

#include "plugins/record.h"
#include "Gem/Image.h"
#include "Utils/ThreadMutex.h"
#include <pipewire/pipewire.h>
#include <spa/param/video/type-info.h>


/*---------------------------------------------------------------
 -------------------------------------------------------------------
  CLASS
  recordPIPEWIRE

  class for outputting video-streams to PIPEWIRE

  KEYWORDS
  pix record movie

  DESCRIPTION

  -----------------------------------------------------------------*/
namespace gem
{
namespace plugins
{
class GEM_EXPORT recordPIPEWIRE : public record
{
public:

  //////////
  // Constructor
  recordPIPEWIRE(void);

  ////////
  // Destructor
  virtual ~recordPIPEWIRE(void);

  //////////
  // close the movie file
  // stop recording, close the file and clean up temporary things
  virtual void stop(void);

  //////////
  // prepare the stream
  // this doesn't do much, as we need an image to setup our pwprops
  // returns TRUE if opening could be successful, FALSE otherwise
  virtual bool start(const std::string&filename, gem::Properties&props);

  //////////
  // start the stream
  virtual bool init(imageStruct*);

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
  void on_process(void);
  void on_param_changed(uint32_t id, const struct spa_pod *param);
private:
  static void process_cb(void*);
  static void param_changed_cb(void*, uint32_t id, const struct spa_pod *param);
  std::string m_filename;

  /* converted image */
  imageStruct m_image;
  struct pw_stream *m_stream;
  struct pw_stream_events m_stream_events;

  gem::thread::Mutex m_mutex;
  spa_video_format m_format;
};
};
};

#endif  // for header file
