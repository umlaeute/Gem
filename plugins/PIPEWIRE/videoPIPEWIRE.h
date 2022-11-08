/*-----------------------------------------------------------------

    GEM - Graphics Environment for Multimedia

    video backend for Gem

    Copyright (c) 2022 IOhannes m zmölnig. forum::für::umläute. IEM. zmoelnig@iem.at
    For information on usage and redistribution, and for a DISCLAIMER OF ALL
    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

-----------------------------------------------------------------*/

#ifndef _INCLUDE_GEMPLUGIN__VIDEOPIPEWIRE_VIDEOPIPEWIRE_H_
#define _INCLUDE_GEMPLUGIN__VIDEOPIPEWIRE_VIDEOPIPEWIRE_H_

#include "plugins/video.h"
#include "Gem/Image.h"
#include "Utils/ThreadMutex.h"
#include <pipewire/pipewire.h>
#include <spa/param/video/type-info.h>

namespace gem
{
namespace plugins
{
class GEM_EXPORT videoPIPEWIRE : public video
{
private:
  std::string m_name;
  pixBlock m_pixBlock;
  Properties m_props;
  gem::thread::Mutex m_mutex;
  struct pw_stream *m_stream;
  spa_video_format m_format;
public:
  videoPIPEWIRE(void);

  virtual ~videoPIPEWIRE(void);

  virtual bool open(gem::Properties&props);
  virtual pixBlock *getFrame(void);

  virtual std::vector<std::string>enumerate(void);
  virtual bool setDevice(int ID);
  virtual bool setDevice(const std::string&);


  virtual bool enumProperties(gem::Properties&readable,
                              gem::Properties&writeable);
  virtual void setProperties(gem::Properties&props);
  virtual void getProperties(gem::Properties&props);

  virtual std::vector<std::string>dialogs(void);
  // for pix_video: query whether this backend provides access to this class of devices
  // (e.g. "dv")
  virtual bool provides(const std::string&);
  // get a list of all provided devices
  virtual std::vector<std::string>provides(void);

  // get's the name of the backend (e.g. "v4l")
  virtual const std::string getName(void);

  virtual bool isThreadable(void)
  {
    return true;
  }
  virtual bool reset(void)
  {
    return true;
  }
  virtual void releaseFrame(void);
  virtual bool grabAsynchronous(bool)
  {
    return true;
  }
  virtual bool dialog(std::vector<std::string>names=
                        std::vector<std::string>())
  {
    return false;
  }
  virtual bool         setColor(int)
  {
    return false;
  }


  virtual void close(void);
  virtual bool start(void);
  virtual bool stop(void);
  void on_process(void);
  void on_param_changed(uint32_t id, const struct spa_pod *param);
private:
  static void process_cb(void*);
  static void param_changed_cb(void*, uint32_t id, const struct spa_pod *param);
  struct pw_stream_events m_stream_events;
};
};
}; // namespace

#endif  // for header file
