// get StreamGrabber attributes
#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#ifdef HAVE_PYLON
#include "StreamGrabberProperties.h"

using namespace gem::pylon::streamgrabberproperties;

void gem::pylon::streamgrabberproperties::init() {
  static bool initialized=false;
  if(initialized)return;
  initialized=true;

}
std::vector<std::string>gem::pylon::streamgrabberproperties::getKeys(void) {
  gem::pylon::streamgrabberproperties::init();
  std::vector<std::string>result;
  return result;
}
std::vector<std::string>gem::pylon::streamgrabberproperties::setKeys(void) {
  gem::pylon::streamgrabberproperties::init();
  std::vector<std::string>result;
  return result;
}

void gem::pylon::streamgrabberproperties::get(Pylon::CBaslerGigEStreamGrabber*device, 
                std::string key,
                gem::any&result)
{
  gem::pylon::streamgrabberproperties::init();
  if(0) {;

  } else if (key=="MaxNumBuffer") {
    result=static_cast<double>(device->MaxNumBuffer.GetValue());
    return;

  } else if (key=="MaxBufferSize") {
    result=static_cast<double>(device->MaxBufferSize.GetValue());
    return;

  } else if (key=="EnableResend") {
    result=static_cast<double>(device->EnableResend.GetValue());
    return;

  } else if (key=="PacketTimeout") {
    result=static_cast<double>(device->PacketTimeout.GetValue());
    return;

  } else if (key=="ReceiveWindowSize") {
    result=static_cast<double>(device->ReceiveWindowSize.GetValue());
    return;

  } else if (key=="ResendRequestThreshold") {
    result=static_cast<double>(device->ResendRequestThreshold.GetValue());
    return;

  } else if (key=="ResendRequestBatching") {
    result=static_cast<double>(device->ResendRequestBatching.GetValue());
    return;

  } else if (key=="ResendTimeout") {
    result=static_cast<double>(device->ResendTimeout.GetValue());
    return;

  } else if (key=="ResendRequestResponseTimeout") {
    result=static_cast<double>(device->ResendRequestResponseTimeout.GetValue());
    return;

  } else if (key=="MaximumNumberResendRequests") {
    result=static_cast<double>(device->MaximumNumberResendRequests.GetValue());
    return;

  } else if (key=="FrameRetention") {
    result=static_cast<double>(device->FrameRetention.GetValue());
    return;

  } else if (key=="ReceiveThreadPriorityOverride") {
    result=static_cast<double>(device->ReceiveThreadPriorityOverride.GetValue());
    return;

  } else if (key=="ReceiveThreadPriority") {
    result=static_cast<double>(device->ReceiveThreadPriority.GetValue());
    return;

  } else if (key=="SocketBufferSize") {
    result=static_cast<double>(device->SocketBufferSize.GetValue());
    return;

  } else if (key=="Status") {
    result=static_cast<double>(device->Status.GetValue()) /* enumeration */;
    return;

  } else if (key=="AccessMode") {
    result=static_cast<double>(device->AccessMode.GetValue()) /* enumeration */;
    return;

  } else if (key=="TypeIsWindowsIntelPerformanceDriverAvailable") {
    result=static_cast<double>(device->TypeIsWindowsIntelPerformanceDriverAvailable.GetValue());
    return;

  } else if (key=="TypeIsWindowsFilterDriverAvailable") {
    result=static_cast<double>(device->TypeIsWindowsFilterDriverAvailable.GetValue());
    return;

  } else if (key=="TypeIsSocketDriverAvailable") {
    result=static_cast<double>(device->TypeIsSocketDriverAvailable.GetValue());
    return;

  } else if (key=="Statistic_Total_Buffer_Count") {
    result=static_cast<double>(device->Statistic_Total_Buffer_Count.GetValue());
    return;

  } else if (key=="Statistic_Failed_Buffer_Count") {
    result=static_cast<double>(device->Statistic_Failed_Buffer_Count.GetValue());
    return;

  } else if (key=="Statistic_Buffer_Underrun_Count") {
    result=static_cast<double>(device->Statistic_Buffer_Underrun_Count.GetValue());
    return;

  } else if (key=="Statistic_Total_Packet_Count") {
    result=static_cast<double>(device->Statistic_Total_Packet_Count.GetValue());
    return;

  } else if (key=="Statistic_Failed_Packet_Count") {
    result=static_cast<double>(device->Statistic_Failed_Packet_Count.GetValue());
    return;

  } else if (key=="Statistic_Resend_Request_Count") {
    result=static_cast<double>(device->Statistic_Resend_Request_Count.GetValue());
    return;

  } else if (key=="Statistic_Resend_Packet_Count") {
    result=static_cast<double>(device->Statistic_Resend_Packet_Count.GetValue());
    return;

  } else if (key=="TransmissionType") {
    result=static_cast<double>(device->TransmissionType.GetValue()) /* enumeration */;
    return;

  } else if (key=="DestinationAddr") {
    result=std::string(device->DestinationAddr.GetValue().c_str());
    return;

  } else if (key=="DestinationPort") {
    result=static_cast<double>(device->DestinationPort.GetValue());
    return;

  }
}
// set StreamGrabber attributes

bool gem::pylon::streamgrabberproperties::set(Pylon::CBaslerGigEStreamGrabber*device, 
                std::string key,
                gem::Properties&props)
{
  gem::pylon::streamgrabberproperties::init();
  if(0) {;
  } else if (key=="MaxNumBuffer") {
    double v; 
    if(props.get(key, v)) {
      device->MaxNumBuffer.SetValue(v);
    }
    return true;
  } else if (key=="MaxBufferSize") {
    double v; 
    if(props.get(key, v)) {
      device->MaxBufferSize.SetValue(v);
    }
    return true;
  } else if (key=="EnableResend") {
    double v; 
    if(props.get(key, v)) {
      device->EnableResend.SetValue(v);
    }
    return true;
  } else if (key=="PacketTimeout") {
    double v; 
    if(props.get(key, v)) {
      device->PacketTimeout.SetValue(v);
    }
    return true;
  } else if (key=="ReceiveWindowSize") {
    double v; 
    if(props.get(key, v)) {
      device->ReceiveWindowSize.SetValue(v);
    }
    return true;
  } else if (key=="ResendRequestThreshold") {
    double v; 
    if(props.get(key, v)) {
      device->ResendRequestThreshold.SetValue(v);
    }
    return true;
  } else if (key=="ResendRequestBatching") {
    double v; 
    if(props.get(key, v)) {
      device->ResendRequestBatching.SetValue(v);
    }
    return true;
  } else if (key=="ResendTimeout") {
    double v; 
    if(props.get(key, v)) {
      device->ResendTimeout.SetValue(v);
    }
    return true;
  } else if (key=="ResendRequestResponseTimeout") {
    double v; 
    if(props.get(key, v)) {
      device->ResendRequestResponseTimeout.SetValue(v);
    }
    return true;
  } else if (key=="MaximumNumberResendRequests") {
    double v; 
    if(props.get(key, v)) {
      device->MaximumNumberResendRequests.SetValue(v);
    }
    return true;
  } else if (key=="FrameRetention") {
    double v; 
    if(props.get(key, v)) {
      device->FrameRetention.SetValue(v);
    }
    return true;
  } else if (key=="ReceiveThreadPriorityOverride") {
    double v; 
    if(props.get(key, v)) {
      device->ReceiveThreadPriorityOverride.SetValue(v);
    }
    return true;
  } else if (key=="ReceiveThreadPriority") {
    double v; 
    if(props.get(key, v)) {
      device->ReceiveThreadPriority.SetValue(v);
    }
    return true;
  } else if (key=="SocketBufferSize") {
    double v; 
    if(props.get(key, v)) {
      device->SocketBufferSize.SetValue(v);
    }
    return true;
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
  } else if (key=="TypeIsWindowsIntelPerformanceDriverAvailable") {
    double v; 
    if(props.get(key, v)) {
      device->TypeIsWindowsIntelPerformanceDriverAvailable.SetValue(v);
    }
    return true;
  } else if (key=="TypeIsWindowsFilterDriverAvailable") {
    double v; 
    if(props.get(key, v)) {
      device->TypeIsWindowsFilterDriverAvailable.SetValue(v);
    }
    return true;
  } else if (key=="TypeIsSocketDriverAvailable") {
    double v; 
    if(props.get(key, v)) {
      device->TypeIsSocketDriverAvailable.SetValue(v);
    }
    return true;
  } else if (key=="Statistic_Total_Buffer_Count") {
    double v; 
    if(props.get(key, v)) {
      device->Statistic_Total_Buffer_Count.SetValue(v);
    }
    return true;
  } else if (key=="Statistic_Failed_Buffer_Count") {
    double v; 
    if(props.get(key, v)) {
      device->Statistic_Failed_Buffer_Count.SetValue(v);
    }
    return true;
  } else if (key=="Statistic_Buffer_Underrun_Count") {
    double v; 
    if(props.get(key, v)) {
      device->Statistic_Buffer_Underrun_Count.SetValue(v);
    }
    return true;
  } else if (key=="Statistic_Total_Packet_Count") {
    double v; 
    if(props.get(key, v)) {
      device->Statistic_Total_Packet_Count.SetValue(v);
    }
    return true;
  } else if (key=="Statistic_Failed_Packet_Count") {
    double v; 
    if(props.get(key, v)) {
      device->Statistic_Failed_Packet_Count.SetValue(v);
    }
    return true;
  } else if (key=="Statistic_Resend_Request_Count") {
    double v; 
    if(props.get(key, v)) {
      device->Statistic_Resend_Request_Count.SetValue(v);
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
  } else if (key=="DestinationAddr") {
    std::string v; 
    if(props.get(key, v)) { 
      device->DestinationAddr.SetValue(v.c_str());
    }
    return true;
  } else if (key=="DestinationPort") {
    double v; 
    if(props.get(key, v)) {
      device->DestinationPort.SetValue(v);
    }
    return true;
  }
  return false;
}


#endif /* HAVE_PYLON */
