#ifndef _INCLUDE_GEMPLUGIN__VIDEODECKLINK_VIDEODECKLINK_H_
#define _INCLUDE_GEMPLUGIN__VIDEODECKLINK_VIDEODECKLINK_H_

#include "plugins/video.h"
#include "Gem/Image.h"
#include "Utils/ThreadMutex.h"

#include <DeckLinkAPI.h>

namespace gem { namespace plugins {
    class GEM_EXPORT videoDECKLINK : public video {
 private:
   std::string m_name; // 'decklink'

   /* device selection by name/number */
   std::string m_devname; // "Mini Recorder #1"?
   int m_devnum; // selected device number (or -1)

   /* format selection by name/number */
   std::string m_formatname;
   int m_formatnum;

   gem::thread::Mutex m_mutex;

   pixBlock m_pixBlock;
   Properties m_props;

   IDeckLinkIterator*m_dlIterator;
   IDeckLink*m_dl;
   IDeckLinkInput* m_dlInput;
   IDeckLinkDisplayMode*m_displayMode;
   IDeckLinkConfiguration *m_dlConfig;
   BMDVideoConnection m_connectionType;
   IDeckLinkInputCallback*m_dlCallback;

 public:
   videoDECKLINK(void);

   virtual ~videoDECKLINK(void);

   virtual bool open(gem::Properties&props);
   virtual void close(void);

   virtual bool start(void);
   virtual bool stop(void);

   virtual pixBlock *getFrame(void);
   virtual void releaseFrame(void);

   /* to be called from the capturing thread */
   void setFrame(unsigned int w, unsigned int h, GLenum format, unsigned char*data);

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

   // get's the name of the backend (e.g. "decklink")
   virtual const std::string getName(void);

   virtual bool isThreadable(void) {return false; return true;}
   virtual bool reset(void) {return true;}
   virtual bool grabAsynchronous(bool) {return false; return true;}
   virtual bool	dialog(std::vector<std::string>names=std::vector<std::string>()) {return false;}
   virtual bool	    	setColor(int) {return false;}
};
};}; // namespace

#endif	// for header file
