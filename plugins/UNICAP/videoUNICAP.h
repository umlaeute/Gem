/*-----------------------------------------------------------------

GEM - Graphics Environment for Multimedia

capture an image using unicap

Copyright (c) 2001-2012 IOhannes m zmölnig. forum::für::umläute. IEM. zmoelnig@iem.at
For information on usage and redistribution, and for a DISCLAIMER OF ALL
WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

-----------------------------------------------------------------*/

#ifndef _INCLUDE_GEMPLUGIN__VIDEOUNICAP_VIDEOUNICAP_H_
#define _INCLUDE_GEMPLUGIN__VIDEOUNICAP_VIDEOUNICAP_H_

#include "plugins/video.h"

#include "Gem/Image.h"
#include "Gem/Properties.h"

#include <map>
#define private unicap_private
#include <unicap.h>
#undef private
#include "Utils/ThreadMutex.h"
/*-----------------------------------------------------------------
  -------------------------------------------------------------------
  CLASS
  pix_video

  grabs a video via UNICAP (http://unicap-imaging.org/)

  KEYWORDS
  pix

  -----------------------------------------------------------------*/
namespace gem
{
namespace plugins
{
class GEM_EXPORT videoUNICAP : public video
{
public:
  //////////
  // Constructor
  videoUNICAP(void);

  //////////
  // Destructor
  virtual ~videoUNICAP(void);

  ////////
  // open the video-device
  virtual bool           open(gem::Properties&writeprops);
  virtual void          close(void);

  //////////
  // Start up the video device
  // [out] int - returns false if starting failed
  virtual bool          start(void);
  //////////
  // Stop the video device
  // [out] int - returns 0 if bad
  virtual bool          stop(void);

  virtual bool reset(void);

  //////////
  // get the next frame
  virtual pixBlock    *getFrame(void);
  virtual void releaseFrame(void);

  //////////
  // Set the video properties
  virtual bool          setColor(int);

  virtual bool defaultFormat(void);

  virtual std::vector<std::string>enumerate(void);
  virtual bool          setDevice(int ID);
  virtual bool          setDevice(const std::string&);


  virtual bool enumProperties(gem::Properties&readable,
                              gem::Properties&writeable);
  virtual void setProperties(gem::Properties&writeprops);
  virtual void getProperties(gem::Properties&readprops);

  virtual const std::string getName(void);

  virtual std::vector<std::string>provides(void);
  virtual bool provides(const std::string&);

  virtual bool          dialog(std::vector<std::string>)
  {
    return false;
  }
  virtual std::vector<std::string>dialogs(void)
  {
    std::vector<std::string>result;
    return result;
  }
  virtual bool isThreadable(void)
  {
    return false;
  }
  virtual bool grabAsynchronous(bool)
  {
    return false;
  }

protected:
  // list of backends we provide:
  std::vector<std::string>m_providers;
  virtual void provide(const std::string&);

  std::vector<unicap_device_t>m_devices;
  std::map<std::string, std::vector<unsigned int> >m_name2devices;


  unsigned int m_width, m_height;
  unicap_handle_t m_handle;

  pixBlock m_pix;
  std::string m_devicename;
  int m_devicenum;

  int m_reqFormat;

  void newFrame (unicap_handle_t handle,
                 unicap_data_buffer_t * buffer);

  gem::thread::Mutex mutex;
  gem::Properties m_props;

private:
  static void newFrameCB (unicap_event_t event,
                          unicap_handle_t handle,
                          unicap_data_buffer_t * buffer,
                          void *usr_data);

  /* are we currently rendering? */
  bool m_running;

};
};
};

#endif  // for header file
