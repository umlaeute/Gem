#ifndef _INCLUDE_GEMPLUGIN__VIDEOVLC_VIDEOVLC_H_
#define _INCLUDE_GEMPLUGIN__VIDEOVLC_VIDEOVLC_H_

#include "plugins/video.h"
#include "Gem/Image.h"
#include "Gem/ThreadMutex.h"

#include <vlc/vlc.h>
#include <stdarg.h>

#ifndef VLC_VAR_TIME
typedef int64_t mtime_t;
#endif /* VLC_VAR_TIME */

namespace gem { namespace plugins {
    class GEM_EXTERN videoVLC : public video {
 private:
   std::string m_name;
   std::string m_devname;
   gem::thread::Mutex m_mutex;

   pixBlock m_pixBlock;
   Properties m_props;
   unsigned int m_type;

  public:
   videoVLC(void);

   virtual ~videoVLC(void);

   virtual bool open(gem::Properties&props);
   virtual pixBlock *getFrame(void);

   virtual std::vector<std::string>enumerate(void);
   virtual bool	setDevice(int ID);
   virtual bool	setDevice(const std::string);


   virtual bool enumProperties(gem::Properties&readable,
			       gem::Properties&writeable);
   virtual void setProperties(gem::Properties&props);
   virtual void getProperties(gem::Properties&props);

   virtual std::vector<std::string>dialogs(void);
   // for pix_video: query whether this backend provides access to this class of devices
   // (e.g. "dv")
   virtual bool provides(const std::string);
   // get a list of all provided devices
   virtual std::vector<std::string>provides(void);
   
   // get's the name of the backend (e.g. "v4l")
   virtual const std::string getName(void);

   virtual bool isThreadable(void) {return true;}
   virtual bool reset(void) {return true;}
   virtual void releaseFrame(void);
   virtual bool grabAsynchronous(bool) {return true;}
   virtual bool	dialog(std::vector<std::string>names=std::vector<std::string>()) {return false;}
   virtual bool	    	setColor(int) {return false;}

   virtual void close(void);
   virtual bool start(void);
   virtual bool stop(void);


   virtual void*  lockFrame(void**plane);
   virtual void unlockFrame(void*picture, void*const*plane);
                  private:


   static void  *lockCB(void*, void**plane);
   static void unlockCB(void*, void*picture, void*const*plane);
   static void displayCB(void*, void*picture);

   libvlc_instance_t*m_instance;
   libvlc_media_player_t*m_mediaplayer;
};
};}; // namespace

#endif	// for header file
