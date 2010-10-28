// get StreamGrabber attributes

static void StreamGrabberPropertiesGet(Pylon::CBaslerGigEStreamGrabber*device, 
                                           std::string key,
                                           gem::any&result)
{
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
