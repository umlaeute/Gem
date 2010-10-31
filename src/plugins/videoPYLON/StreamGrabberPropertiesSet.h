// set StreamGrabber attributes

static bool StreamGrabberPropertiesSet(Pylon::CBaslerGigEStreamGrabber*device, 
                                       std::string key,
                                       gem::Properties&props)
{
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
