// get GigE-Camera attributes

static void CameraPropertiesGet(Pylon::CBaslerGigECamera*device, 
                                    std::string key,
                                    gem::any&result)
{
  if(0) {;
  } else if (key=="GainAuto") {
    result=static_cast<double>(device->GainAuto.GetValue()) /* enumeration */;
    return;

  } else if (key=="GainSelector") {
    result=static_cast<double>(device->GainSelector.GetValue()) /* enumeration */;
    return;

  } else if (key=="GainRaw") {
    result=static_cast<double>(device->GainRaw.GetValue());
    return;

  } else if (key=="GainAbs") {
    result=static_cast<double>(device->GainAbs.GetValue());
    return;

  } else if (key=="BlackLevelSelector") {
    result=static_cast<double>(device->BlackLevelSelector.GetValue()) /* enumeration */;
    return;

  } else if (key=="BlackLevelRaw") {
    result=static_cast<double>(device->BlackLevelRaw.GetValue());
    return;

  } else if (key=="BlackLevelAbs") {
    result=static_cast<double>(device->BlackLevelAbs.GetValue());
    return;

  } else if (key=="BalanceWhiteAuto") {
    result=static_cast<double>(device->BalanceWhiteAuto.GetValue()) /* enumeration */;
    return;

  } else if (key=="BalanceRatioSelector") {
    result=static_cast<double>(device->BalanceRatioSelector.GetValue()) /* enumeration */;
    return;

  } else if (key=="BalanceRatioAbs") {
    result=static_cast<double>(device->BalanceRatioAbs.GetValue());
    return;

  } else if (key=="BalanceRatioRaw") {
    result=static_cast<double>(device->BalanceRatioRaw.GetValue());
    return;

  } else if (key=="GammaEnable") {
    result=static_cast<double>(device->GammaEnable.GetValue());
    return;

  } else if (key=="Gamma") {
    result=static_cast<double>(device->Gamma.GetValue());
    return;

  } else if (key=="DigitalShift") {
    result=static_cast<double>(device->DigitalShift.GetValue());
    return;

  } else if (key=="PixelFormat") {
    result=static_cast<double>(device->PixelFormat.GetValue()) /* enumeration */;
    return;

  } else if (key=="PixelCoding") {
    result=static_cast<double>(device->PixelCoding.GetValue()) /* enumeration */;
    return;

  } else if (key=="PixelSize") {
    result=static_cast<double>(device->PixelSize.GetValue()) /* enumeration */;
    return;

  } else if (key=="PixelColorFilter") {
    result=static_cast<double>(device->PixelColorFilter.GetValue()) /* enumeration */;
    return;

  } else if (key=="PixelDynamicRangeMin") {
    result=static_cast<double>(device->PixelDynamicRangeMin.GetValue());
    return;

  } else if (key=="PixelDynamicRangeMax") {
    result=static_cast<double>(device->PixelDynamicRangeMax.GetValue());
    return;

  } else if (key=="SpatialCorrection") {
    result=static_cast<double>(device->SpatialCorrection.GetValue());
    return;

  } else if (key=="SpatialCorrectionAmount") {
    result=static_cast<double>(device->SpatialCorrectionAmount.GetValue());
    return;

  } else if (key=="SpatialCorrectionStartingLine") {
    result=static_cast<double>(device->SpatialCorrectionStartingLine.GetValue()) /* enumeration */;
    return;

  } else if (key=="TestImageSelector") {
    result=static_cast<double>(device->TestImageSelector.GetValue()) /* enumeration */;
    return;

  } else if (key=="Width") {
    result=static_cast<double>(device->Width.GetValue());
    return;

  } else if (key=="Height") {
    result=static_cast<double>(device->Height.GetValue());
    return;

  } else if (key=="OffsetX") {
    result=static_cast<double>(device->OffsetX.GetValue());
    return;

  } else if (key=="OffsetY") {
    result=static_cast<double>(device->OffsetY.GetValue());
    return;

  } else if (key=="LegacyBinningVertical") {
    result=static_cast<double>(device->LegacyBinningVertical.GetValue()) /* enumeration */;
    return;

  } else if (key=="BinningVertical") {
    result=static_cast<double>(device->BinningVertical.GetValue());
    return;

  } else if (key=="BinningHorizontal") {
    result=static_cast<double>(device->BinningHorizontal.GetValue());
    return;

  } else if (key=="AcquisitionMode") {
    result=static_cast<double>(device->AcquisitionMode.GetValue()) /* enumeration */;
    return;

  } else if (key=="TriggerSelector") {
    result=static_cast<double>(device->TriggerSelector.GetValue()) /* enumeration */;
    return;

  } else if (key=="TriggerMode") {
    result=static_cast<double>(device->TriggerMode.GetValue()) /* enumeration */;
    return;

  } else if (key=="TriggerSource") {
    result=static_cast<double>(device->TriggerSource.GetValue()) /* enumeration */;
    return;

  } else if (key=="TriggerActivation") {
    result=static_cast<double>(device->TriggerActivation.GetValue()) /* enumeration */;
    return;

  } else if (key=="ExposureMode") {
    result=static_cast<double>(device->ExposureMode.GetValue()) /* enumeration */;
    return;

  } else if (key=="ExposureAuto") {
    result=static_cast<double>(device->ExposureAuto.GetValue()) /* enumeration */;
    return;

  } else if (key=="ExposureTimeAbs") {
    result=static_cast<double>(device->ExposureTimeAbs.GetValue());
    return;

  } else if (key=="ExposureTimeBaseAbs") {
    result=static_cast<double>(device->ExposureTimeBaseAbs.GetValue());
    return;

  } else if (key=="ExposureTimeBaseAbsEnable") {
    result=static_cast<double>(device->ExposureTimeBaseAbsEnable.GetValue());
    return;

  } else if (key=="ExposureTimeRaw") {
    result=static_cast<double>(device->ExposureTimeRaw.GetValue());
    return;

  } else if (key=="AcquisitionLineRateAbs") {
    result=static_cast<double>(device->AcquisitionLineRateAbs.GetValue());
    return;

  } else if (key=="ResultingLineRateAbs") {
    result=static_cast<double>(device->ResultingLineRateAbs.GetValue());
    return;

  } else if (key=="AcquisitionFrameRateEnable") {
    result=static_cast<double>(device->AcquisitionFrameRateEnable.GetValue());
    return;

  } else if (key=="AcquisitionFrameRateAbs") {
    result=static_cast<double>(device->AcquisitionFrameRateAbs.GetValue());
    return;

  } else if (key=="ResultingFrameRateAbs") {
    result=static_cast<double>(device->ResultingFrameRateAbs.GetValue());
    return;

  } else if (key=="AveragingNumberOfFrames") {
    result=static_cast<double>(device->AveragingNumberOfFrames.GetValue());
    return;

  } else if (key=="LineSelector") {
    result=static_cast<double>(device->LineSelector.GetValue()) /* enumeration */;
    return;

  } else if (key=="LineMode") {
    result=static_cast<double>(device->LineMode.GetValue()) /* enumeration */;
    return;

  } else if (key=="LineFormat") {
    result=static_cast<double>(device->LineFormat.GetValue()) /* enumeration */;
    return;

  } else if (key=="LineSource") {
    result=static_cast<double>(device->LineSource.GetValue()) /* enumeration */;
    return;

  } else if (key=="LineInverter") {
    result=static_cast<double>(device->LineInverter.GetValue());
    return;

  } else if (key=="LineTermination") {
    result=static_cast<double>(device->LineTermination.GetValue());
    return;

  } else if (key=="LineDebouncerTimeAbs") {
    result=static_cast<double>(device->LineDebouncerTimeAbs.GetValue());
    return;

  } else if (key=="LineDebouncerTimeRaw") {
    result=static_cast<double>(device->LineDebouncerTimeRaw.GetValue());
    return;

  } else if (key=="LineStatus") {
    result=static_cast<double>(device->LineStatus.GetValue());
    return;

  } else if (key=="LineStatusAll") {
    result=static_cast<double>(device->LineStatusAll.GetValue());
    return;

  } else if (key=="UserOutputSelector") {
    result=static_cast<double>(device->UserOutputSelector.GetValue()) /* enumeration */;
    return;

  } else if (key=="UserOutputValue") {
    result=static_cast<double>(device->UserOutputValue.GetValue());
    return;

  } else if (key=="UserOutputValueAll") {
    result=static_cast<double>(device->UserOutputValueAll.GetValue());
    return;

  } else if (key=="UserOutputValueAllMask") {
    result=static_cast<double>(device->UserOutputValueAllMask.GetValue());
    return;

  } else if (key=="ShaftEncoderModuleLineSelector") {
    result=static_cast<double>(device->ShaftEncoderModuleLineSelector.GetValue()) /* enumeration */;
    return;

  } else if (key=="ShaftEncoderModuleLineSource") {
    result=static_cast<double>(device->ShaftEncoderModuleLineSource.GetValue()) /* enumeration */;
    return;

  } else if (key=="ShaftEncoderModuleMode") {
    result=static_cast<double>(device->ShaftEncoderModuleMode.GetValue()) /* enumeration */;
    return;

  } else if (key=="ShaftEncoderModuleCounterMode") {
    result=static_cast<double>(device->ShaftEncoderModuleCounterMode.GetValue()) /* enumeration */;
    return;

  } else if (key=="ShaftEncoderModuleCounter") {
    result=static_cast<double>(device->ShaftEncoderModuleCounter.GetValue());
    return;

  } else if (key=="ShaftEncoderModuleCounterMax") {
    result=static_cast<double>(device->ShaftEncoderModuleCounterMax.GetValue());
    return;

  } else if (key=="ShaftEncoderModuleReverseCounterMax") {
    result=static_cast<double>(device->ShaftEncoderModuleReverseCounterMax.GetValue());
    return;

  } else if (key=="TimerDelayTimebaseAbs") {
    result=static_cast<double>(device->TimerDelayTimebaseAbs.GetValue());
    return;

  } else if (key=="TimerDurationTimebaseAbs") {
    result=static_cast<double>(device->TimerDurationTimebaseAbs.GetValue());
    return;

  } else if (key=="TimerSelector") {
    result=static_cast<double>(device->TimerSelector.GetValue()) /* enumeration */;
    return;

  } else if (key=="TimerDelayAbs") {
    result=static_cast<double>(device->TimerDelayAbs.GetValue());
    return;

  } else if (key=="TimerDelayRaw") {
    result=static_cast<double>(device->TimerDelayRaw.GetValue());
    return;

  } else if (key=="TimerDurationAbs") {
    result=static_cast<double>(device->TimerDurationAbs.GetValue());
    return;

  } else if (key=="TimerDurationRaw") {
    result=static_cast<double>(device->TimerDurationRaw.GetValue());
    return;

  } else if (key=="TimerTriggerSource") {
    result=static_cast<double>(device->TimerTriggerSource.GetValue()) /* enumeration */;
    return;

  } else if (key=="TimerTriggerActivation") {
    result=static_cast<double>(device->TimerTriggerActivation.GetValue()) /* enumeration */;
    return;

  } else if (key=="TimerSequenceEnable") {
    result=static_cast<double>(device->TimerSequenceEnable.GetValue());
    return;

  } else if (key=="TimerSequenceLastEntryIndex") {
    result=static_cast<double>(device->TimerSequenceLastEntryIndex.GetValue());
    return;

  } else if (key=="TimerSequenceCurrentEntryIndex") {
    result=static_cast<double>(device->TimerSequenceCurrentEntryIndex.GetValue());
    return;

  } else if (key=="TimerSequenceEntrySelector") {
    result=static_cast<double>(device->TimerSequenceEntrySelector.GetValue()) /* enumeration */;
    return;

  } else if (key=="TimerSequenceTimerSelector") {
    result=static_cast<double>(device->TimerSequenceTimerSelector.GetValue()) /* enumeration */;
    return;

  } else if (key=="TimerSequenceTimerEnable") {
    result=static_cast<double>(device->TimerSequenceTimerEnable.GetValue());
    return;

  } else if (key=="TimerSequenceTimerInverter") {
    result=static_cast<double>(device->TimerSequenceTimerInverter.GetValue());
    return;

  } else if (key=="TimerSequenceTimerDelayRaw") {
    result=static_cast<double>(device->TimerSequenceTimerDelayRaw.GetValue());
    return;

  } else if (key=="TimerSequenceTimerDurationRaw") {
    result=static_cast<double>(device->TimerSequenceTimerDurationRaw.GetValue());
    return;

  } else if (key=="LUTSelector") {
    result=static_cast<double>(device->LUTSelector.GetValue()) /* enumeration */;
    return;

  } else if (key=="LUTEnable") {
    result=static_cast<double>(device->LUTEnable.GetValue());
    return;

  } else if (key=="LUTIndex") {
    result=static_cast<double>(device->LUTIndex.GetValue());
    return;

  } else if (key=="LUTValue") {
    result=static_cast<double>(device->LUTValue.GetValue());
    return;

  } else if (key=="UserSetSelector") {
    result=static_cast<double>(device->UserSetSelector.GetValue()) /* enumeration */;
    return;

  } else if (key=="UserSetDefaultSelector") {
    result=static_cast<double>(device->UserSetDefaultSelector.GetValue()) /* enumeration */;
    return;

  } else if (key=="AutoTargetValue") {
    result=static_cast<double>(device->AutoTargetValue.GetValue());
    return;

  } else if (key=="AutoGainRawLowerLimit") {
    result=static_cast<double>(device->AutoGainRawLowerLimit.GetValue());
    return;

  } else if (key=="AutoGainRawUpperLimit") {
    result=static_cast<double>(device->AutoGainRawUpperLimit.GetValue());
    return;

  } else if (key=="AutoExposureTimeAbsLowerLimit") {
    result=static_cast<double>(device->AutoExposureTimeAbsLowerLimit.GetValue());
    return;

  } else if (key=="AutoExposureTimeAbsUpperLimit") {
    result=static_cast<double>(device->AutoExposureTimeAbsUpperLimit.GetValue());
    return;

  } else if (key=="AutoFunctionAOISelector") {
    result=static_cast<double>(device->AutoFunctionAOISelector.GetValue()) /* enumeration */;
    return;

  } else if (key=="AutoFunctionAOIWidth") {
    result=static_cast<double>(device->AutoFunctionAOIWidth.GetValue());
    return;

  } else if (key=="AutoFunctionAOIHeight") {
    result=static_cast<double>(device->AutoFunctionAOIHeight.GetValue());
    return;

  } else if (key=="AutoFunctionAOIOffsetX") {
    result=static_cast<double>(device->AutoFunctionAOIOffsetX.GetValue());
    return;

  } else if (key=="AutoFunctionAOIOffsetY") {
    result=static_cast<double>(device->AutoFunctionAOIOffsetY.GetValue());
    return;

  } else if (key=="ShadingSelector") {
    result=static_cast<double>(device->ShadingSelector.GetValue()) /* enumeration */;
    return;

  } else if (key=="ShadingEnable") {
    result=static_cast<double>(device->ShadingEnable.GetValue());
    return;

  } else if (key=="ShadingStatus") {
    result=static_cast<double>(device->ShadingStatus.GetValue()) /* enumeration */;
    return;

  } else if (key=="ShadingSetDefaultSelector") {
    result=static_cast<double>(device->ShadingSetDefaultSelector.GetValue()) /* enumeration */;
    return;

  } else if (key=="ShadingSetSelector") {
    result=static_cast<double>(device->ShadingSetSelector.GetValue()) /* enumeration */;
    return;

  } else if (key=="UserDefinedValueSelector") {
    result=static_cast<double>(device->UserDefinedValueSelector.GetValue()) /* enumeration */;
    return;

  } else if (key=="UserDefinedValue") {
    result=static_cast<double>(device->UserDefinedValue.GetValue());
    return;

  } else if (key=="DeviceVendorName") {
    result=std::string(device->DeviceVendorName.GetValue().c_str());
    return;

  } else if (key=="DeviceModelName") {
    result=std::string(device->DeviceModelName.GetValue().c_str());
    return;

  } else if (key=="DeviceManufacturerInfo") {
    result=std::string(device->DeviceManufacturerInfo.GetValue().c_str());
    return;

  } else if (key=="DeviceVersion") {
    result=std::string(device->DeviceVersion.GetValue().c_str());
    return;

  } else if (key=="DeviceFirmwareVersion") {
    result=std::string(device->DeviceFirmwareVersion.GetValue().c_str());
    return;

  } else if (key=="DeviceID") {
    result=std::string(device->DeviceID.GetValue().c_str());
    return;

  } else if (key=="DeviceUserID") {
    result=std::string(device->DeviceUserID.GetValue().c_str());
    return;

  } else if (key=="DeviceScanType") {
    result=static_cast<double>(device->DeviceScanType.GetValue()) /* enumeration */;
    return;

  } else if (key=="SensorWidth") {
    result=static_cast<double>(device->SensorWidth.GetValue());
    return;

  } else if (key=="SensorHeight") {
    result=static_cast<double>(device->SensorHeight.GetValue());
    return;

  } else if (key=="WidthMax") {
    result=static_cast<double>(device->WidthMax.GetValue());
    return;

  } else if (key=="HeightMax") {
    result=static_cast<double>(device->HeightMax.GetValue());
    return;

  } else if (key=="TemperatureSelector") {
    result=static_cast<double>(device->TemperatureSelector.GetValue()) /* enumeration */;
    return;

  } else if (key=="TemperatureAbs") {
    result=static_cast<double>(device->TemperatureAbs.GetValue());
    return;

  } else if (key=="ParameterSelector") {
    result=static_cast<double>(device->ParameterSelector.GetValue()) /* enumeration */;
    return;

  } else if (key=="RemoveLimits") {
    result=static_cast<double>(device->RemoveLimits.GetValue());
    return;

  } else if (key=="ExpertFeatureAccessSelector") {
    result=static_cast<double>(device->ExpertFeatureAccessSelector.GetValue()) /* enumeration */;
    return;

  } else if (key=="ExpertFeatureAccessKey") {
    result=static_cast<double>(device->ExpertFeatureAccessKey.GetValue());
    return;

  } else if (key=="ExpertFeatureEnable") {
    result=static_cast<double>(device->ExpertFeatureEnable.GetValue());
    return;

  } else if (key=="PixelStepCorrectionEnable") {
    result=static_cast<double>(device->PixelStepCorrectionEnable.GetValue());
    return;

  } else if (key=="PixelStepCorrectionSelector") {
    result=static_cast<double>(device->PixelStepCorrectionSelector.GetValue()) /* enumeration */;
    return;

  } else if (key=="PixelStepCorrectionValueAbs") {
    result=static_cast<double>(device->PixelStepCorrectionValueAbs.GetValue());
    return;

  } else if (key=="PixelStepCorrectionValueRaw") {
    result=static_cast<double>(device->PixelStepCorrectionValueRaw.GetValue());
    return;

  } else if (key=="PixelStepCorrectionBusy") {
    result=static_cast<double>(device->PixelStepCorrectionBusy.GetValue());
    return;

  } else if (key=="ChunkModeActive") {
    result=static_cast<double>(device->ChunkModeActive.GetValue());
    return;

  } else if (key=="ChunkSelector") {
    result=static_cast<double>(device->ChunkSelector.GetValue()) /* enumeration */;
    return;

  } else if (key=="ChunkEnable") {
    result=static_cast<double>(device->ChunkEnable.GetValue());
    return;

  } else if (key=="ChunkStride") {
    result=static_cast<double>(device->ChunkStride.GetValue());
    return;

  } else if (key=="ChunkOffsetX") {
    result=static_cast<double>(device->ChunkOffsetX.GetValue());
    return;

  } else if (key=="ChunkOffsetY") {
    result=static_cast<double>(device->ChunkOffsetY.GetValue());
    return;

  } else if (key=="ChunkWidth") {
    result=static_cast<double>(device->ChunkWidth.GetValue());
    return;

  } else if (key=="ChunkHeight") {
    result=static_cast<double>(device->ChunkHeight.GetValue());
    return;

  } else if (key=="ChunkDynamicRangeMin") {
    result=static_cast<double>(device->ChunkDynamicRangeMin.GetValue());
    return;

  } else if (key=="ChunkDynamicRangeMax") {
    result=static_cast<double>(device->ChunkDynamicRangeMax.GetValue());
    return;

  } else if (key=="ChunkPixelFormat") {
    result=static_cast<double>(device->ChunkPixelFormat.GetValue()) /* enumeration */;
    return;

  } else if (key=="ChunkTimestamp") {
    result=static_cast<double>(device->ChunkTimestamp.GetValue());
    return;

  } else if (key=="ChunkFramecounter") {
    result=static_cast<double>(device->ChunkFramecounter.GetValue());
    return;

  } else if (key=="ChunkLineStatusAll") {
    result=static_cast<double>(device->ChunkLineStatusAll.GetValue());
    return;

  } else if (key=="ChunkTriggerinputcounter") {
    result=static_cast<double>(device->ChunkTriggerinputcounter.GetValue());
    return;

  } else if (key=="ChunkLineTriggerIgnoredCounter") {
    result=static_cast<double>(device->ChunkLineTriggerIgnoredCounter.GetValue());
    return;

  } else if (key=="ChunkFrameTriggerIgnoredCounter") {
    result=static_cast<double>(device->ChunkFrameTriggerIgnoredCounter.GetValue());
    return;

  } else if (key=="ChunkFrameTriggerCounter") {
    result=static_cast<double>(device->ChunkFrameTriggerCounter.GetValue());
    return;

  } else if (key=="ChunkFramesPerTriggerCounter") {
    result=static_cast<double>(device->ChunkFramesPerTriggerCounter.GetValue());
    return;

  } else if (key=="ChunkLineTriggerEndToEndCounter") {
    result=static_cast<double>(device->ChunkLineTriggerEndToEndCounter.GetValue());
    return;

  } else if (key=="ChunkPayloadCRC16") {
    result=static_cast<double>(device->ChunkPayloadCRC16.GetValue());
    return;

  } else if (key=="EventSelector") {
    result=static_cast<double>(device->EventSelector.GetValue()) /* enumeration */;
    return;

  } else if (key=="EventNotification") {
    result=static_cast<double>(device->EventNotification.GetValue()) /* enumeration */;
    return;

  } else if (key=="ExposureEndEventStreamChannelIndex") {
    result=static_cast<double>(device->ExposureEndEventStreamChannelIndex.GetValue());
    return;

  } else if (key=="ExposureEndEventFrameID") {
    result=static_cast<double>(device->ExposureEndEventFrameID.GetValue());
    return;

  } else if (key=="ExposureEndEventTimestamp") {
    result=static_cast<double>(device->ExposureEndEventTimestamp.GetValue());
    return;

  } else if (key=="LineStartOvertriggerEventStreamChannelIndex") {
    result=static_cast<double>(device->LineStartOvertriggerEventStreamChannelIndex.GetValue());
    return;

  } else if (key=="LineStartOvertriggerEventTimestamp") {
    result=static_cast<double>(device->LineStartOvertriggerEventTimestamp.GetValue());
    return;

  } else if (key=="FrameStartOvertriggerEventStreamChannelIndex") {
    result=static_cast<double>(device->FrameStartOvertriggerEventStreamChannelIndex.GetValue());
    return;

  } else if (key=="FrameStartOvertriggerEventTimestamp") {
    result=static_cast<double>(device->FrameStartOvertriggerEventTimestamp.GetValue());
    return;

  } else if (key=="EventOverrunEventStreamChannelIndex") {
    result=static_cast<double>(device->EventOverrunEventStreamChannelIndex.GetValue());
    return;

  } else if (key=="EventOverrunEventFrameID") {
    result=static_cast<double>(device->EventOverrunEventFrameID.GetValue());
    return;

  } else if (key=="EventOverrunEventTimestamp") {
    result=static_cast<double>(device->EventOverrunEventTimestamp.GetValue());
    return;

  } else if (key=="FileSelector") {
    result=static_cast<double>(device->FileSelector.GetValue()) /* enumeration */;
    return;

  } else if (key=="FileOperationSelector") {
    result=static_cast<double>(device->FileOperationSelector.GetValue()) /* enumeration */;
    return;

  } else if (key=="FileOpenMode") {
    result=static_cast<double>(device->FileOpenMode.GetValue()) /* enumeration */;
    return;

  } else if (key=="FileAccessOffset") {
    result=static_cast<double>(device->FileAccessOffset.GetValue());
    return;

  } else if (key=="FileAccessLength") {
    result=static_cast<double>(device->FileAccessLength.GetValue());
    return;

  } else if (key=="FileOperationStatus") {
    result=static_cast<double>(device->FileOperationStatus.GetValue()) /* enumeration */;
    return;

  } else if (key=="FileOperationResult") {
    result=static_cast<double>(device->FileOperationResult.GetValue());
    return;

  } else if (key=="FileSize") {
    result=static_cast<double>(device->FileSize.GetValue());
    return;

  } else if (key=="PayloadSize") {
    result=static_cast<double>(device->PayloadSize.GetValue());
    return;

  } else if (key=="GevVersionMajor") {
    result=static_cast<double>(device->GevVersionMajor.GetValue());
    return;

  } else if (key=="GevVersionMinor") {
    result=static_cast<double>(device->GevVersionMinor.GetValue());
    return;

  } else if (key=="GevDeviceModeIsBigEndian") {
    result=static_cast<double>(device->GevDeviceModeIsBigEndian.GetValue());
    return;

  } else if (key=="GevDeviceModeCharacterSet") {
    result=static_cast<double>(device->GevDeviceModeCharacterSet.GetValue());
    return;

  } else if (key=="GevInterfaceSelector") {
    result=static_cast<double>(device->GevInterfaceSelector.GetValue()) /* enumeration */;
    return;

  } else if (key=="GevMACAddress") {
    result=static_cast<double>(device->GevMACAddress.GetValue());
    return;

  } else if (key=="GevSupportedIPConfigurationLLA") {
    result=static_cast<double>(device->GevSupportedIPConfigurationLLA.GetValue());
    return;

  } else if (key=="GevSupportedIPConfigurationDHCP") {
    result=static_cast<double>(device->GevSupportedIPConfigurationDHCP.GetValue());
    return;

  } else if (key=="GevSupportedIPConfigurationPersistentIP") {
    result=static_cast<double>(device->GevSupportedIPConfigurationPersistentIP.GetValue());
    return;

  } else if (key=="GevCurrentIPConfiguration") {
    result=static_cast<double>(device->GevCurrentIPConfiguration.GetValue());
    return;

  } else if (key=="GevCurrentIPAddress") {
    result=static_cast<double>(device->GevCurrentIPAddress.GetValue());
    return;

  } else if (key=="GevCurrentSubnetMask") {
    result=static_cast<double>(device->GevCurrentSubnetMask.GetValue());
    return;

  } else if (key=="GevCurrentDefaultGateway") {
    result=static_cast<double>(device->GevCurrentDefaultGateway.GetValue());
    return;

  } else if (key=="GevPersistentIPAddress") {
    result=static_cast<double>(device->GevPersistentIPAddress.GetValue());
    return;

  } else if (key=="GevPersistentSubnetMask") {
    result=static_cast<double>(device->GevPersistentSubnetMask.GetValue());
    return;

  } else if (key=="GevPersistentDefaultGateway") {
    result=static_cast<double>(device->GevPersistentDefaultGateway.GetValue());
    return;

  } else if (key=="GevLinkSpeed") {
    result=static_cast<double>(device->GevLinkSpeed.GetValue());
    return;

  } else if (key=="GevFirstURL") {
    result=std::string(device->GevFirstURL.GetValue().c_str());
    return;

  } else if (key=="GevSecondURL") {
    result=std::string(device->GevSecondURL.GetValue().c_str());
    return;

  } else if (key=="GevNumberOfInterfaces") {
    result=static_cast<double>(device->GevNumberOfInterfaces.GetValue());
    return;

  } else if (key=="GevMessageChannelCount") {
    result=static_cast<double>(device->GevMessageChannelCount.GetValue());
    return;

  } else if (key=="GevStreamChannelCount") {
    result=static_cast<double>(device->GevStreamChannelCount.GetValue());
    return;

  } else if (key=="GevSupportedOptionalCommandsEVENTDATA") {
    result=static_cast<double>(device->GevSupportedOptionalCommandsEVENTDATA.GetValue());
    return;

  } else if (key=="GevSupportedOptionalCommandsEVENT") {
    result=static_cast<double>(device->GevSupportedOptionalCommandsEVENT.GetValue());
    return;

  } else if (key=="GevSupportedOptionalCommandsPACKETRESEND") {
    result=static_cast<double>(device->GevSupportedOptionalCommandsPACKETRESEND.GetValue());
    return;

  } else if (key=="GevSupportedOptionalCommandsWRITEMEM") {
    result=static_cast<double>(device->GevSupportedOptionalCommandsWRITEMEM.GetValue());
    return;

  } else if (key=="GevSupportedOptionalCommandsConcatenation") {
    result=static_cast<double>(device->GevSupportedOptionalCommandsConcatenation.GetValue());
    return;

  } else if (key=="GevHeartbeatTimeout") {
    result=static_cast<double>(device->GevHeartbeatTimeout.GetValue());
    return;

  } else if (key=="GevTimestampTickFrequency") {
    result=static_cast<double>(device->GevTimestampTickFrequency.GetValue());
    return;

  } else if (key=="GevTimestampValue") {
    result=static_cast<double>(device->GevTimestampValue.GetValue());
    return;

  } else if (key=="GevCCP") {
    result=static_cast<double>(device->GevCCP.GetValue()) /* enumeration */;
    return;

  } else if (key=="GevStreamChannelSelector") {
    result=static_cast<double>(device->GevStreamChannelSelector.GetValue()) /* enumeration */;
    return;

  } else if (key=="GevSCPInterfaceIndex") {
    result=static_cast<double>(device->GevSCPInterfaceIndex.GetValue());
    return;

  } else if (key=="GevSCDA") {
    result=static_cast<double>(device->GevSCDA.GetValue());
    return;

  } else if (key=="GevSCPHostPort") {
    result=static_cast<double>(device->GevSCPHostPort.GetValue());
    return;

  } else if (key=="GevSCPSPacketSize") {
    result=static_cast<double>(device->GevSCPSPacketSize.GetValue());
    return;

  } else if (key=="GevSCPD") {
    result=static_cast<double>(device->GevSCPD.GetValue());
    return;

  } else if (key=="GevSCFTD") {
    result=static_cast<double>(device->GevSCFTD.GetValue());
    return;

  } else if (key=="GevSCBWR") {
    result=static_cast<double>(device->GevSCBWR.GetValue());
    return;

  } else if (key=="GevSCBWRA") {
    result=static_cast<double>(device->GevSCBWRA.GetValue());
    return;

  } else if (key=="GevSCBWA") {
    result=static_cast<double>(device->GevSCBWA.GetValue());
    return;

  } else if (key=="GevSCDMT") {
    result=static_cast<double>(device->GevSCDMT.GetValue());
    return;

  } else if (key=="GevSCDCT") {
    result=static_cast<double>(device->GevSCDCT.GetValue());
    return;

  } else if (key=="GevSCFJM") {
    result=static_cast<double>(device->GevSCFJM.GetValue());
    return;

  } else if (key=="TLParamsLocked") {
    result=static_cast<double>(device->TLParamsLocked.GetValue());
    return;
  }
}
