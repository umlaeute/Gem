#ifndef _INCLUDE_GEMPLUGIN__VIDEOVLC_VIDEOVLC_H_
#define _INCLUDE_GEMPLUGIN__VIDEOVLC_VIDEOVLC_H_

#include "plugins/video.h"
#include "Gem/Image.h"
#include <stdarg.h>

#ifndef VLC_VAR_TIME
typedef int64_t mtime_t;
#endif /* VLC_VAR_TIME */

namespace gem
{
namespace plugins
{
class GEM_EXPORT videoVIDS : public video
{

public:
  videoVIDS(void);
  virtual ~videoVIDS(void);

  virtual const std::string getName(void);
  virtual bool provides(const std::string);
  virtual std::vector<std::string>provides(void);

  virtual bool open(gem::Properties&props);
  virtual bool start(void);
  virtual bool stop(void);
  virtual void close(void);
  virtual bool reset(void);

  virtual pixBlock *getFrame(void);
  virtual void releaseFrame(void);

  virtual std::vector<std::string>enumerate(void);
  virtual bool	setDevice(int ID);
  virtual bool	setDevice(const std::string);

  virtual bool enumProperties(gem::Properties&readable,
                              gem::Properties&writeable);
  virtual void setProperties(gem::Properties&props);
  virtual void getProperties(gem::Properties&props);

  virtual bool isThreadable(void);
  virtual bool grabAsynchronous(bool);

  virtual std::vector<std::string>dialogs(void);
  virtual bool dialog(std::vector<std::string>names=
                        std::vector<std::string>());

  virtual bool	    	setColor(int);

private:
  std::string m_name;
  pixBlock m_pixBlock;
  gem::Properties m_props;
};
};
}; // namespace

#endif	// for header file
