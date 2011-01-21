////////////////////////////////////////////////////////
//
// GEM - Graphics Environment for Multimedia
//
// zmoelnig@iem.kug.ac.at
//
// Implementation file
//
//    Copyright (c) 1997-1998 Mark Danks.
//    Copyright (c) Günther Geiger.
//    Copyright (c) 2001-2002 IOhannes m zmoelnig. forum::für::umläute. IEM
//    For information on usage and redistribution, and for a DISCLAIMER OF ALL
//    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.
//
/////////////////////////////////////////////////////////
#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include "videoUNICAP.h"

using namespace gem;

// for debugging...
#include "Gem/RTE.h"

/* debugging helpers  */
#define debugPost
#define debugThread

#if 0
# undef debugPost
# define debugPost ::startpost("%s:%s[%d]", __FILE__, __FUNCTION__, __LINE__); ::post
#endif

#if 0
# undef debugThread
# define debugThread ::startpost("%s:%s[%d]", __FILE__, __FUNCTION__, __LINE__); ::post
#endif

/////////////////////////////////////////////////////////
//
// videoUNICAP
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
#ifdef HAVE_UNICAP

#include <sys/stat.h>

REGISTER_VIDEOFACTORY("unicap", videoUNICAP);

videoUNICAP :: videoUNICAP() : video("unicap", 0)
			     , m_handle(NULL)
{
  m_capturing=false;
#warning UNICAP
  provide("analog");
}
  
////////////////////////////////////////////////////////
// Destructor
//
////////////////////////////////////////////////////////
videoUNICAP :: ~videoUNICAP()
{
  close();
}

//////////////////
// this reads the data that was captured by capturing() and returns it within a pixBlock
pixBlock *videoUNICAP :: getFrame(){
  return &m_image;
}

bool videoUNICAP :: openDevice(gem::Properties&props) {
  std::vector<unsigned int>ds;
  if(m_devicename.empty()) {
    ds.push_back(m_devicenum);
  } else {
    ds=m_name2devices[m_devicename];
  }
  unicap_status_t status = -1;
  int i=0;
  for(i=0; i<ds.size(); i++) {
    unicap_device_t device;
    status = unicap_enumerate_devices (NULL, &device, ds[i]); // (1)
    if(SUCCESS(status)) {
      status = unicap_open (&m_handle, &device);  
      if(SUCCESS(status)) {
	post("successfully opened device#%d '%s'", ds[i], m_devicename.c_str());
	break;
      }
    }
  }
  if(!SUCCESS(status))
    return false;
  
  #warning setprops
  //setProperties();

  return true;
}
void videoUNICAP :: closeDevice() {
  if(m_handle) {
    unicap_close(m_handle);
    m_handle=NULL;
  }
}

void videoUNICAP::gotFrameCB (unicap_event_t event, 
			  unicap_handle_t handle, 
			  unicap_data_buffer_t * buffer, 
			  void *usr_data)
{
  videoUNICAP*v=(videoUNICAP*)usr_data;
  v->gotFrame(event, handle, buffer);
}

void videoUNICAP::gotFrame (unicap_event_t event, 
			    unicap_handle_t handle, 
			    unicap_data_buffer_t * buffer) {
#warning implement gotFrame
  post("got frame %d", m_frameCount++);
}


/////////////////////////////////////////////////////////
// startTransfer
//
/////////////////////////////////////////////////////////
bool videoUNICAP :: startTransfer()
{
  unicap_format_t format;
  if (!SUCCESS (unicap_get_format (m_handle, &format))) {
    verbose(1, "failed to query format");
    return false;
  }

  format.buffer_type = UNICAP_BUFFER_TYPE_SYSTEM; 
  if (!SUCCESS (unicap_set_format (m_handle, &format))) {
    verbose(1, "failed to set format (sysbuf)");

    return false;
  }

  unicap_register_callback (m_handle, 
			   UNICAP_EVENT_NEW_FRAME, 
			   (unicap_callback_t) gotFrameCB,
			   (void *) this);
  m_frameCount=0;

  unicap_start_capture (m_handle);

  return true;
}

/////////////////////////////////////////////////////////
// stopTransfer
//
/////////////////////////////////////////////////////////
bool videoUNICAP :: stopTransfer()
{
  unicap_stop_capture (m_handle);        // (3)

  return true;
}

bool videoUNICAP :: setColor(int format)
{
  if (format<=0 || format==m_reqFormat)return -1;
  m_reqFormat=format;
  restartTransfer();
  return true;
}

std::vector<std::string> videoUNICAP::enumerate() {
  std::vector<std::string> result;
  int devcount=0;
  unicap_status_t status = unicap_reenumerate_devices(&devcount);
  int i=0;

  post("UNICAP: enum");

  if(!SUCCESS(status))
    return result;

  post("UNICAP: %d devices", devcount);

  m_devices.clear();
  m_name2devices.clear();


  for(i=0; i<devcount; i++) {
    unicap_device_t device;
    post("unumerating %d", i);
    status = unicap_enumerate_devices (NULL, &device, i); // (1)
    post("enum %d returned %d", i, status);


    if(SUCCESS(status)) {
      const unsigned int cur=m_devices.size();
      post("ID='%s'\tmodel='%s'\tvendor='%s'\tdevice='%s'",
	   device.identifier,
	   device.model_name,
	   device.vendor_name,
	   device.device);
	   
      m_devices.push_back(device);

      m_name2devices[device.identifier ].push_back(cur);
      m_name2devices[device.model_name ].push_back(cur);
      m_name2devices[device.vendor_name].push_back(cur);
      if(device.device[0])
	m_name2devices[device.device].push_back(cur);

      result.push_back(device.identifier);
    }
  }
  


  return result;
}

bool videoUNICAP :: enumProperties(gem::Properties&readable,
				 gem::Properties&writeable) {
  readable.clear();
  writeable.clear();

  return true;
}
void videoUNICAP :: getProperties(gem::Properties&props) {

}
void videoUNICAP :: setProperties(gem::Properties&props) {

}


#else
videoUNICAP ::  videoUNICAP() : video("") {}
videoUNICAP :: ~videoUNICAP() {}
#endif /* HAVE_UNICAP */
