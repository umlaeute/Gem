// get StreamGrabber attributes
#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#ifdef HAVE_PYLON
#include "StreamGrabberProperties.h"
#include "map"

namespace gem{namespace pylon{namespace streamgrabberproperties{
  static gem::Properties writeprops, readprops;
  typedef bool (*t_getbool)(Pylon::CBaslerGigEStreamGrabber*device);
  std::map<std::string, t_getbool>map_getbool;

  static bool getEnableResend(Pylon::CBaslerGigEStreamGrabber*device) { return device->EnableResend.GetValue(); }
  static bool getReceiveThreadPriorityOverride(Pylon::CBaslerGigEStreamGrabber*device) { return device->ReceiveThreadPriorityOverride.GetValue(); }

  typedef void (*t_setbool)(Pylon::CBaslerGigEStreamGrabber*device, const bool);
  std::map<std::string, t_setbool>map_setbool;

  static void setEnableResend(Pylon::CBaslerGigEStreamGrabber*device, const bool v) { device->EnableResend.SetValue(v); }
  static void setReceiveThreadPriorityOverride(Pylon::CBaslerGigEStreamGrabber*device, const bool v) { device->ReceiveThreadPriorityOverride.SetValue(v); }


  typedef int64_t (*t_getint)(Pylon::CBaslerGigEStreamGrabber*device);
  std::map<std::string, t_getint>map_getint;

  static int64_t getMaxNumBuffer(Pylon::CBaslerGigEStreamGrabber*device) { return device->MaxNumBuffer.GetValue(); }
  static int64_t getMaxBufferSize(Pylon::CBaslerGigEStreamGrabber*device) { return device->MaxBufferSize.GetValue(); }
  static int64_t getPacketTimeout(Pylon::CBaslerGigEStreamGrabber*device) { return device->PacketTimeout.GetValue(); }
  static int64_t getReceiveWindowSize(Pylon::CBaslerGigEStreamGrabber*device) { return device->ReceiveWindowSize.GetValue(); }
  static int64_t getResendRequestThreshold(Pylon::CBaslerGigEStreamGrabber*device) { return device->ResendRequestThreshold.GetValue(); }
  static int64_t getResendRequestBatching(Pylon::CBaslerGigEStreamGrabber*device) { return device->ResendRequestBatching.GetValue(); }
  static int64_t getResendTimeout(Pylon::CBaslerGigEStreamGrabber*device) { return device->ResendTimeout.GetValue(); }
  static int64_t getResendRequestResponseTimeout(Pylon::CBaslerGigEStreamGrabber*device) { return device->ResendRequestResponseTimeout.GetValue(); }
  static int64_t getMaximumNumberResendRequests(Pylon::CBaslerGigEStreamGrabber*device) { return device->MaximumNumberResendRequests.GetValue(); }
  static int64_t getFrameRetention(Pylon::CBaslerGigEStreamGrabber*device) { return device->FrameRetention.GetValue(); }
  static int64_t getReceiveThreadPriority(Pylon::CBaslerGigEStreamGrabber*device) { return device->ReceiveThreadPriority.GetValue(); }
  static int64_t getSocketBufferSize(Pylon::CBaslerGigEStreamGrabber*device) { return device->SocketBufferSize.GetValue(); }
  static int64_t getTypeIsWindowsIntelPerformanceDriverAvailable(Pylon::CBaslerGigEStreamGrabber*device) { return device->TypeIsWindowsIntelPerformanceDriverAvailable.GetValue(); }
  static int64_t getTypeIsWindowsFilterDriverAvailable(Pylon::CBaslerGigEStreamGrabber*device) { return device->TypeIsWindowsFilterDriverAvailable.GetValue(); }
  static int64_t getTypeIsSocketDriverAvailable(Pylon::CBaslerGigEStreamGrabber*device) { return device->TypeIsSocketDriverAvailable.GetValue(); }
  static int64_t getStatistic_Total_Buffer_Count(Pylon::CBaslerGigEStreamGrabber*device) { return device->Statistic_Total_Buffer_Count.GetValue(); }
  static int64_t getStatistic_Failed_Buffer_Count(Pylon::CBaslerGigEStreamGrabber*device) { return device->Statistic_Failed_Buffer_Count.GetValue(); }
  static int64_t getStatistic_Buffer_Underrun_Count(Pylon::CBaslerGigEStreamGrabber*device) { return device->Statistic_Buffer_Underrun_Count.GetValue(); }
  static int64_t getStatistic_Total_Packet_Count(Pylon::CBaslerGigEStreamGrabber*device) { return device->Statistic_Total_Packet_Count.GetValue(); }
  static int64_t getStatistic_Failed_Packet_Count(Pylon::CBaslerGigEStreamGrabber*device) { return device->Statistic_Failed_Packet_Count.GetValue(); }
  static int64_t getStatistic_Resend_Request_Count(Pylon::CBaslerGigEStreamGrabber*device) { return device->Statistic_Resend_Request_Count.GetValue(); }
  static int64_t getStatistic_Resend_Packet_Count(Pylon::CBaslerGigEStreamGrabber*device) { return device->Statistic_Resend_Packet_Count.GetValue(); }
  static int64_t getDestinationPort(Pylon::CBaslerGigEStreamGrabber*device) { return device->DestinationPort.GetValue(); }

  typedef void (*t_setint)(Pylon::CBaslerGigEStreamGrabber*device, const int64_t);
  std::map<std::string, t_setint>map_setint;

  static void setMaxNumBuffer(Pylon::CBaslerGigEStreamGrabber*device, const int64_t v) { device->MaxNumBuffer.SetValue(v); }
  static void setMaxBufferSize(Pylon::CBaslerGigEStreamGrabber*device, const int64_t v) { device->MaxBufferSize.SetValue(v); }
  static void setPacketTimeout(Pylon::CBaslerGigEStreamGrabber*device, const int64_t v) { device->PacketTimeout.SetValue(v); }
  static void setReceiveWindowSize(Pylon::CBaslerGigEStreamGrabber*device, const int64_t v) { device->ReceiveWindowSize.SetValue(v); }
  static void setResendRequestThreshold(Pylon::CBaslerGigEStreamGrabber*device, const int64_t v) { device->ResendRequestThreshold.SetValue(v); }
  static void setResendRequestBatching(Pylon::CBaslerGigEStreamGrabber*device, const int64_t v) { device->ResendRequestBatching.SetValue(v); }
  static void setResendTimeout(Pylon::CBaslerGigEStreamGrabber*device, const int64_t v) { device->ResendTimeout.SetValue(v); }
  static void setResendRequestResponseTimeout(Pylon::CBaslerGigEStreamGrabber*device, const int64_t v) { device->ResendRequestResponseTimeout.SetValue(v); }
  static void setMaximumNumberResendRequests(Pylon::CBaslerGigEStreamGrabber*device, const int64_t v) { device->MaximumNumberResendRequests.SetValue(v); }
  static void setFrameRetention(Pylon::CBaslerGigEStreamGrabber*device, const int64_t v) { device->FrameRetention.SetValue(v); }
  static void setReceiveThreadPriority(Pylon::CBaslerGigEStreamGrabber*device, const int64_t v) { device->ReceiveThreadPriority.SetValue(v); }
  static void setSocketBufferSize(Pylon::CBaslerGigEStreamGrabber*device, const int64_t v) { device->SocketBufferSize.SetValue(v); }
  static void setTypeIsWindowsIntelPerformanceDriverAvailable(Pylon::CBaslerGigEStreamGrabber*device, const int64_t v) { device->TypeIsWindowsIntelPerformanceDriverAvailable.SetValue(v); }
  static void setTypeIsWindowsFilterDriverAvailable(Pylon::CBaslerGigEStreamGrabber*device, const int64_t v) { device->TypeIsWindowsFilterDriverAvailable.SetValue(v); }
  static void setTypeIsSocketDriverAvailable(Pylon::CBaslerGigEStreamGrabber*device, const int64_t v) { device->TypeIsSocketDriverAvailable.SetValue(v); }
  static void setStatistic_Total_Buffer_Count(Pylon::CBaslerGigEStreamGrabber*device, const int64_t v) { device->Statistic_Total_Buffer_Count.SetValue(v); }
  static void setStatistic_Failed_Buffer_Count(Pylon::CBaslerGigEStreamGrabber*device, const int64_t v) { device->Statistic_Failed_Buffer_Count.SetValue(v); }
  static void setStatistic_Buffer_Underrun_Count(Pylon::CBaslerGigEStreamGrabber*device, const int64_t v) { device->Statistic_Buffer_Underrun_Count.SetValue(v); }
  static void setStatistic_Total_Packet_Count(Pylon::CBaslerGigEStreamGrabber*device, const int64_t v) { device->Statistic_Total_Packet_Count.SetValue(v); }
  static void setStatistic_Failed_Packet_Count(Pylon::CBaslerGigEStreamGrabber*device, const int64_t v) { device->Statistic_Failed_Packet_Count.SetValue(v); }
  static void setStatistic_Resend_Request_Count(Pylon::CBaslerGigEStreamGrabber*device, const int64_t v) { device->Statistic_Resend_Request_Count.SetValue(v); }
  static void setStatistic_Resend_Packet_Count(Pylon::CBaslerGigEStreamGrabber*device, const int64_t v) { device->Statistic_Resend_Packet_Count.SetValue(v); }
  static void setDestinationPort(Pylon::CBaslerGigEStreamGrabber*device, const int64_t v) { device->DestinationPort.SetValue(v); }


  typedef GenICam::gcstring (*t_getstring)(Pylon::CBaslerGigEStreamGrabber*device);
  std::map<std::string, t_getstring>map_getstring;
  static GenICam::gcstring getDestinationAddr(Pylon::CBaslerGigEStreamGrabber*device) { return device->DestinationAddr.GetValue(); }

  typedef void (*t_setstring)(Pylon::CBaslerGigEStreamGrabber*device, GenICam::gcstring);
  std::map<std::string, t_setstring>map_setstring;
  static void setDestinationAddr(Pylon::CBaslerGigEStreamGrabber*device, const GenICam::gcstring v) { device->DestinationAddr.SetValue(v); }

};};};



void gem::pylon::streamgrabberproperties::init() {
  static bool initialized=false;
  if(initialized)return;
  initialized=true;

  map_getbool.clear();
  map_getbool["EnableResend"]=getEnableResend;
  map_getbool["ReceiveThreadPriorityOverride"]=getReceiveThreadPriorityOverride;

  map_setbool.clear();
  map_setbool["EnableResend"]=setEnableResend;
  map_setbool["ReceiveThreadPriorityOverride"]=setReceiveThreadPriorityOverride;


  map_getint.clear();
  map_getint["MaxNumBuffer"]=getMaxNumBuffer;
  map_getint["MaxBufferSize"]=getMaxBufferSize;
  map_getint["PacketTimeout"]=getPacketTimeout;
  map_getint["ReceiveWindowSize"]=getReceiveWindowSize;
  map_getint["ResendRequestThreshold"]=getResendRequestThreshold;
  map_getint["ResendRequestBatching"]=getResendRequestBatching;
  map_getint["ResendTimeout"]=getResendTimeout;
  map_getint["ResendRequestResponseTimeout"]=getResendRequestResponseTimeout;
  map_getint["MaximumNumberResendRequests"]=getMaximumNumberResendRequests;
  map_getint["FrameRetention"]=getFrameRetention;
  map_getint["ReceiveThreadPriority"]=getReceiveThreadPriority;
  map_getint["SocketBufferSize"]=getSocketBufferSize;
  map_getint["TypeIsWindowsIntelPerformanceDriverAvailable"]=getTypeIsWindowsIntelPerformanceDriverAvailable;
  map_getint["TypeIsWindowsFilterDriverAvailable"]=getTypeIsWindowsFilterDriverAvailable;
  map_getint["TypeIsSocketDriverAvailable"]=getTypeIsSocketDriverAvailable;
  map_getint["Statistic_Total_Buffer_Count"]=getStatistic_Total_Buffer_Count;
  map_getint["Statistic_Failed_Buffer_Count"]=getStatistic_Failed_Buffer_Count;
  map_getint["Statistic_Buffer_Underrun_Count"]=getStatistic_Buffer_Underrun_Count;
  map_getint["Statistic_Total_Packet_Count"]=getStatistic_Total_Packet_Count;
  map_getint["Statistic_Failed_Packet_Count"]=getStatistic_Failed_Packet_Count;
  map_getint["Statistic_Resend_Request_Count"]=getStatistic_Resend_Request_Count;
  map_getint["Statistic_Resend_Packet_Count"]=getStatistic_Resend_Packet_Count;
  map_getint["DestinationPort"]=getDestinationPort;

  map_setint.clear();
  map_setint["MaxNumBuffer"]=setMaxNumBuffer;
  map_setint["MaxBufferSize"]=setMaxBufferSize;
  map_setint["PacketTimeout"]=setPacketTimeout;
  map_setint["ReceiveWindowSize"]=setReceiveWindowSize;
  map_setint["ResendRequestThreshold"]=setResendRequestThreshold;
  map_setint["ResendRequestBatching"]=setResendRequestBatching;
  map_setint["ResendTimeout"]=setResendTimeout;
  map_setint["ResendRequestResponseTimeout"]=setResendRequestResponseTimeout;
  map_setint["MaximumNumberResendRequests"]=setMaximumNumberResendRequests;
  map_setint["FrameRetention"]=setFrameRetention;
  map_setint["ReceiveThreadPriority"]=setReceiveThreadPriority;
  map_setint["SocketBufferSize"]=setSocketBufferSize;
  map_setint["TypeIsWindowsIntelPerformanceDriverAvailable"]=setTypeIsWindowsIntelPerformanceDriverAvailable;
  map_setint["TypeIsWindowsFilterDriverAvailable"]=setTypeIsWindowsFilterDriverAvailable;
  map_setint["TypeIsSocketDriverAvailable"]=setTypeIsSocketDriverAvailable;
  map_setint["Statistic_Total_Buffer_Count"]=setStatistic_Total_Buffer_Count;
  map_setint["Statistic_Failed_Buffer_Count"]=setStatistic_Failed_Buffer_Count;
  map_setint["Statistic_Buffer_Underrun_Count"]=setStatistic_Buffer_Underrun_Count;
  map_setint["Statistic_Total_Packet_Count"]=setStatistic_Total_Packet_Count;
  map_setint["Statistic_Failed_Packet_Count"]=setStatistic_Failed_Packet_Count;
  map_setint["Statistic_Resend_Request_Count"]=setStatistic_Resend_Request_Count;
  map_setint["Statistic_Resend_Packet_Count"]=setStatistic_Resend_Packet_Count;
  map_setint["DestinationPort"]=setDestinationPort;
  
  map_getstring.clear();
  map_getstring["DestinationAddr"]=getDestinationAddr;
  map_setstring.clear();
  map_setstring["DestinationAddr"]=setDestinationAddr;
}


gem::Properties&gem::pylon::streamgrabberproperties::getKeys(void) {
  gem::Properties&result=readprops; result.clear();

  gem::pylon::streamgrabberproperties::init();
  
  do {
    gem::any typevalue=1;
    std::map<std::string, t_getbool>::iterator it;
    for(it=map_getbool.begin(); it!=map_getbool.end(); ++it) {
      result.set(it->first, typevalue);
    }
  } while(0);
  
  do {
    gem::any typevalue=0;
    std::map<std::string, t_getint>::iterator it;
    for(it=map_getint.begin(); it!=map_getint.end(); ++it) {
      result.set(it->first, typevalue);
    }
  } while(0);
  
  do {
    gem::any typevalue=std::string("symbol");
    std::map<std::string, t_getstring>::iterator it;
    for(it=map_getstring.begin(); it!=map_getstring.end(); ++it) {
      result.set(it->first, typevalue);
    }
  } while(0);
  
  /* enumeration */
  gem::any typevalue=0;
  result.set("Status", typevalue);
  result.set("AccessMode", typevalue);
  result.set("TransmissionType", typevalue);
  return result;
}
gem::Properties&gem::pylon::streamgrabberproperties::setKeys(void) {
  gem::Properties&result=writeprops; result.clear();
  gem::pylon::streamgrabberproperties::init();

  do {
    gem::any typevalue=1;
    std::map<std::string, t_setbool>::iterator it;
    for(it=map_setbool.begin(); it!=map_setbool.end(); ++it) {
      result.set(it->first, typevalue);
    }
  } while(0);

  do {
    gem::any typevalue=0;
    std::map<std::string, t_setint>::iterator it;
    for(it=map_setint.begin(); it!=map_setint.end(); ++it) {
      result.set(it->first, typevalue);
    }
  } while(0);

  do {
    gem::any typevalue=std::string("symbol");
    std::map<std::string, t_setstring>::iterator it;
    for(it=map_setstring.begin(); it!=map_setstring.end(); ++it) {
      result.set(it->first, typevalue);
    }
  } while(0);

  /* enumeration */
  gem::any typevalue=0;
  result.set("Status", typevalue);
  result.set("AccessMode", typevalue);
  result.set("TransmissionType", typevalue);

  return result;
}

void gem::pylon::streamgrabberproperties::get(Pylon::CBaslerGigEStreamGrabber*device, 
                std::string key,
                gem::any&result)
{
  gem::pylon::streamgrabberproperties::init();

  std::map<std::string, t_getbool>::iterator it_b=map_getbool.find(key);
  if(it_b != map_getbool.end()) {
    result=static_cast<double>(it_b->second(device));
    return;
  }
  std::map<std::string, t_getint>::iterator it_i=map_getint.find(key);
  if(it_i != map_getint.end()) {
    result=static_cast<double>(it_i->second(device));
    return;
  }
  std::map<std::string, t_getstring>::iterator it_s=map_getstring.find(key);
  if(it_s != map_getstring.end()) {
    result=std::string((it_s->second(device)).c_str());
    return;
  }

  if (0) {
  } else if (key=="Status") {
    result=static_cast<double>(device->Status.GetValue()) /* enumeration */;
    return;
    
  } else if (key=="AccessMode") {
    result=static_cast<double>(device->AccessMode.GetValue()) /* enumeration */;
    return;

  } else if (key=="TransmissionType") {
    result=static_cast<double>(device->TransmissionType.GetValue()) /* enumeration */;
    return;

  }
}
// set StreamGrabber attributes

bool gem::pylon::streamgrabberproperties::set(Pylon::CBaslerGigEStreamGrabber*device, 
                std::string key,
                gem::Properties&props)
{
  double d;
  std::string s;

  gem::pylon::streamgrabberproperties::init();


 std::map<std::string, t_setbool>::iterator it_b=map_setbool.find(key);
  if(it_b != map_setbool.end()) {
    if(props.get(key, d)) {
      it_b->second(device, static_cast<bool>(d));
      return true;
    } else {
      return false;
    }
  }
  std::map<std::string, t_setint>::iterator it_i=map_setint.find(key);
  if(it_i != map_setint.end()) {
    if(props.get(key, d)) {
      it_i->second(device, static_cast<int64_t>(d));
      return true;
    } else {
      return false;
    }
  }
  std::map<std::string, t_setstring>::iterator it_s=map_setstring.find(key);
  if(it_s != map_setstring.end()) {
    if(props.get(key, s)) {
      GenICam::gcstring gs(s.c_str());
      it_s->second(device, gs);
      return true;
    } else {
      return false;
    }
  }

  if(0) {;
  } else if (key=="Status") {
    double d; 
    std::string s; 
    Basler_GigEStreamParams::StatusEnums  e;
    if(props.get(key, d)) { 
      if(d<0)return true;
      e=static_cast< Basler_GigEStreamParams::StatusEnums >(d);
      device->Status.SetValue(e); 
    } else if(props.get(key, s)) { 
      /* enumeration */ 
      //device->Status.SetValue(e); 
    }
    return true;
  } else if (key=="AccessMode") {
    double d; 
    std::string s; 
    Basler_GigEStreamParams::AccessModeEnums  e;
    if(props.get(key, d)) { 
      if(d<0)return true;
      e=static_cast< Basler_GigEStreamParams::AccessModeEnums >(d);
      device->AccessMode.SetValue(e); 
    } else if(props.get(key, s)) { 
      /* enumeration */ 
      //device->AccessMode.SetValue(e); 
    }
    return true;
  } else if (key=="Statistic_Resend_Packet_Count") {
    double v; 
    if(props.get(key, v)) {
      device->Statistic_Resend_Packet_Count.SetValue(v);
    }
    return true;
  } else if (key=="TransmissionType") {
    double d; 
    std::string s; 
    Basler_GigEStreamParams::TransmissionTypeEnums  e;
    if(props.get(key, d)) { 
      if(d<0)return true;
      e=static_cast< Basler_GigEStreamParams::TransmissionTypeEnums >(d);
      device->TransmissionType.SetValue(e); 
    } else if(props.get(key, s)) { 
      /* enumeration */ 
      //device->TransmissionType.SetValue(e); 
    }
    return true;
  }
  return false;
}


#endif /* HAVE_PYLON */
