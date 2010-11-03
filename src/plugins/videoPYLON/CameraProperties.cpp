// get GigE-Camera attributes
#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#ifdef HAVE_PYLON
#include "CameraProperties.h"

using namespace gem::pylon::cameraproperties;


void init() {}
std::vector<std::string>getKeys(void) {
  std::vector<std::string>result;
  return result;
}
std::vector<std::string>setKeys(void) {
  std::vector<std::string>result;
  return result;
}

void get(Pylon::CBaslerGigECamera*device, 
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

  } else if (key=="width") { /* for compat with other backends */
    result=static_cast<double>(device->Width.GetValue());
    return;

  } else if (key=="height") { /* for compat with other backends */
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


bool set(Pylon::CBaslerGigECamera*device, 
                                       std::string key,
                                       gem::Properties&props)
{
  if(0) {;
  } else if (key=="GainAuto") {
    double d; 
    std::string s; 
    Basler_GigECameraParams::GainAutoEnums  e;
    if(props.get(key, d)) { 
      if(d<0)return true;
      e=static_cast< Basler_GigECameraParams::GainAutoEnums >(d);
      device->GainAuto.SetValue(e); 
    } else if(props.get(key, s)) { 
      /* enumeration */ 
      //device->GainAuto.SetValue(e); 
    }
    return true;
  } else if (key=="GainSelector") {
    double d; 
    std::string s; 
    Basler_GigECameraParams::GainSelectorEnums  e;
    if(props.get(key, d)) { 
      if(d<0)return true;
      e=static_cast< Basler_GigECameraParams::GainSelectorEnums >(d);
      device->GainSelector.SetValue(e); 
    } else if(props.get(key, s)) { 
      /* enumeration */ 
      //device->GainSelector.SetValue(e); 
    }
    return true;
  } else if (key=="GainRaw") {
    double v; 
    if(props.get(key, v)) {
      device->GainRaw.SetValue(v);
    }
    return true;
  } else if (key=="GainAbs") {
    double v; 
    if(props.get(key, v)) {
      device->GainAbs.SetValue(v);
    }
    return true;
  } else if (key=="BlackLevelSelector") {
    double d; 
    std::string s; 
    Basler_GigECameraParams::BlackLevelSelectorEnums  e;
    if(props.get(key, d)) { 
      if(d<0)return true;
      e=static_cast< Basler_GigECameraParams::BlackLevelSelectorEnums >(d);
      device->BlackLevelSelector.SetValue(e); 
    } else if(props.get(key, s)) { 
      /* enumeration */ 
      //device->BlackLevelSelector.SetValue(e); 
    }
    return true;
  } else if (key=="BlackLevelRaw") {
    double v; 
    if(props.get(key, v)) {
      device->BlackLevelRaw.SetValue(v);
    }
    return true;
  } else if (key=="BlackLevelAbs") {
    double v; 
    if(props.get(key, v)) {
      device->BlackLevelAbs.SetValue(v);
    }
    return true;
  } else if (key=="BalanceWhiteAuto") {
    double d; 
    std::string s; 
    Basler_GigECameraParams::BalanceWhiteAutoEnums  e;
    if(props.get(key, d)) { 
      if(d<0)return true;
      e=static_cast< Basler_GigECameraParams::BalanceWhiteAutoEnums >(d);
      device->BalanceWhiteAuto.SetValue(e); 
    } else if(props.get(key, s)) { 
      /* enumeration */ 
      //device->BalanceWhiteAuto.SetValue(e); 
    }
    return true;
  } else if (key=="BalanceRatioSelector") {
    double d; 
    std::string s; 
    Basler_GigECameraParams::BalanceRatioSelectorEnums  e;
    if(props.get(key, d)) { 
      if(d<0)return true;
      e=static_cast< Basler_GigECameraParams::BalanceRatioSelectorEnums >(d);
      device->BalanceRatioSelector.SetValue(e); 
    } else if(props.get(key, s)) { 
      /* enumeration */ 
      //device->BalanceRatioSelector.SetValue(e); 
    }
    return true;
  } else if (key=="BalanceRatioAbs") {
    double v; 
    if(props.get(key, v)) {
      device->BalanceRatioAbs.SetValue(v);
    }
    return true;
  } else if (key=="BalanceRatioRaw") {
    double v; 
    if(props.get(key, v)) {
      device->BalanceRatioRaw.SetValue(v);
    }
    return true;
  } else if (key=="GammaEnable") {
    double v; 
    if(props.get(key, v)) {
      device->GammaEnable.SetValue(v);
    }
    return true;
  } else if (key=="Gamma") {
    double v; 
    if(props.get(key, v)) {
      device->Gamma.SetValue(v);
    }
    return true;
  } else if (key=="DigitalShift") {
    double v; 
    if(props.get(key, v)) {
      device->DigitalShift.SetValue(v);
    }
    return true;
  } else if (key=="PixelFormat") {
    double d; 
    std::string s; 
    Basler_GigECameraParams::PixelFormatEnums  e;
    if(props.get(key, d)) { 
      if(d<0)return true;
      e=static_cast< Basler_GigECameraParams::PixelFormatEnums >(d);
      device->PixelFormat.SetValue(e); 
    } else if(props.get(key, s)) { 
      /* enumeration */ 
      //device->PixelFormat.SetValue(e); 
    }
    return true;
  } else if (key=="PixelCoding") {
    double d; 
    std::string s; 
    Basler_GigECameraParams::PixelCodingEnums  e;
    if(props.get(key, d)) { 
      if(d<0)return true;
      e=static_cast< Basler_GigECameraParams::PixelCodingEnums >(d);
      device->PixelCoding.SetValue(e); 
    } else if(props.get(key, s)) { 
      /* enumeration */ 
      //device->PixelCoding.SetValue(e); 
    }
    return true;
  } else if (key=="PixelSize") {
    double d; 
    std::string s; 
    Basler_GigECameraParams::PixelSizeEnums  e;
    if(props.get(key, d)) { 
      if(d<0)return true;
      e=static_cast< Basler_GigECameraParams::PixelSizeEnums >(d);
      device->PixelSize.SetValue(e); 
    } else if(props.get(key, s)) { 
      /* enumeration */ 
      //device->PixelSize.SetValue(e); 
    }
    return true;
  } else if (key=="PixelColorFilter") {
    double d; 
    std::string s; 
    Basler_GigECameraParams::PixelColorFilterEnums  e;
    if(props.get(key, d)) { 
      if(d<0)return true;
      e=static_cast< Basler_GigECameraParams::PixelColorFilterEnums >(d);
      device->PixelColorFilter.SetValue(e); 
    } else if(props.get(key, s)) { 
      /* enumeration */ 
      //device->PixelColorFilter.SetValue(e); 
    }
    return true;
  } else if (key=="PixelDynamicRangeMin") {
    double v; 
    if(props.get(key, v)) {
      device->PixelDynamicRangeMin.SetValue(v);
    }
    return true;
  } else if (key=="PixelDynamicRangeMax") {
    double v; 
    if(props.get(key, v)) {
      device->PixelDynamicRangeMax.SetValue(v);
    }
    return true;
  } else if (key=="SpatialCorrection") {
    double v; 
    if(props.get(key, v)) {
      device->SpatialCorrection.SetValue(v);
    }
    return true;
  } else if (key=="SpatialCorrectionAmount") {
    double v; 
    if(props.get(key, v)) {
      device->SpatialCorrectionAmount.SetValue(v);
    }
    return true;
  } else if (key=="SpatialCorrectionStartingLine") {
    double d; 
    std::string s; 
    Basler_GigECameraParams::SpatialCorrectionStartingLineEnums  e;
    if(props.get(key, d)) { 
      if(d<0)return true;
      e=static_cast< Basler_GigECameraParams::SpatialCorrectionStartingLineEnums >(d);
      device->SpatialCorrectionStartingLine.SetValue(e); 
    } else if(props.get(key, s)) { 
      /* enumeration */ 
      //device->SpatialCorrectionStartingLine.SetValue(e); 
    }
    return true;
  } else if (key=="TestImageSelector") {
    double d; 
    std::string s; 
    Basler_GigECameraParams::TestImageSelectorEnums  e;
    if(props.get(key, d)) { 
      if(d<0)return true;
      e=static_cast< Basler_GigECameraParams::TestImageSelectorEnums >(d);
      device->TestImageSelector.SetValue(e); 
    } else if(props.get(key, s)) { 
      /* enumeration */ 
      //device->TestImageSelector.SetValue(e); 
    }
    return true;

  } else if (key=="width") {
    double v; 
    if(props.get(key, v)) {
      device->Width.SetValue(v);
    }
    return true;
  } else if (key=="height") {
    double v; 
    if(props.get(key, v)) {
      device->Height.SetValue(v);
    }
    return true;

  } else if (key=="OffsetX") {
    double v; 
    if(props.get(key, v)) {
      device->OffsetX.SetValue(v);
    }
    return true;
  } else if (key=="OffsetY") {
    double v; 
    if(props.get(key, v)) {
      device->OffsetY.SetValue(v);
    }
    return true;
  } else if (key=="LegacyBinningVertical") {
    double d; 
    std::string s; 
    Basler_GigECameraParams::LegacyBinningVerticalEnums  e;
    if(props.get(key, d)) { 
      if(d<0)return true;
      e=static_cast< Basler_GigECameraParams::LegacyBinningVerticalEnums >(d);
      device->LegacyBinningVertical.SetValue(e); 
    } else if(props.get(key, s)) { 
      /* enumeration */ 
      //device->LegacyBinningVertical.SetValue(e); 
    }
    return true;
  } else if (key=="BinningVertical") {
    double v; 
    if(props.get(key, v)) {
      device->BinningVertical.SetValue(v);
    }
    return true;
  } else if (key=="BinningHorizontal") {
    double v; 
    if(props.get(key, v)) {
      device->BinningHorizontal.SetValue(v);
    }
    return true;
  } else if (key=="AcquisitionMode") {
    double d; 
    std::string s; 
    Basler_GigECameraParams::AcquisitionModeEnums  e;
    if(props.get(key, d)) { 
      if(d<0)return true;
      e=static_cast< Basler_GigECameraParams::AcquisitionModeEnums >(d);
      device->AcquisitionMode.SetValue(e); 
    } else if(props.get(key, s)) { 
      /* enumeration */ 
      //device->AcquisitionMode.SetValue(e); 
    }
    return true;
  } else if (key=="TriggerSelector") {
    double d; 
    std::string s; 
    Basler_GigECameraParams::TriggerSelectorEnums  e;
    if(props.get(key, d)) { 
      if(d<0)return true;
      e=static_cast< Basler_GigECameraParams::TriggerSelectorEnums >(d);
      device->TriggerSelector.SetValue(e); 
    } else if(props.get(key, s)) { 
      /* enumeration */ 
      //device->TriggerSelector.SetValue(e); 
    }
    return true;
  } else if (key=="TriggerMode") {
    double d; 
    std::string s; 
    Basler_GigECameraParams::TriggerModeEnums  e;
    if(props.get(key, d)) { 
      if(d<0)return true;
      e=static_cast< Basler_GigECameraParams::TriggerModeEnums >(d);
      device->TriggerMode.SetValue(e); 
    } else if(props.get(key, s)) { 
      /* enumeration */ 
      //device->TriggerMode.SetValue(e); 
    }
    return true;
  } else if (key=="TriggerSource") {
    double d; 
    std::string s; 
    Basler_GigECameraParams::TriggerSourceEnums  e;
    if(props.get(key, d)) { 
      if(d<0)return true;
      e=static_cast< Basler_GigECameraParams::TriggerSourceEnums >(d);
      device->TriggerSource.SetValue(e); 
    } else if(props.get(key, s)) { 
      /* enumeration */ 
      //device->TriggerSource.SetValue(e); 
    }
    return true;
  } else if (key=="TriggerActivation") {
    double d; 
    std::string s; 
    Basler_GigECameraParams::TriggerActivationEnums  e;
    if(props.get(key, d)) { 
      if(d<0)return true;
      e=static_cast< Basler_GigECameraParams::TriggerActivationEnums >(d);
      device->TriggerActivation.SetValue(e); 
    } else if(props.get(key, s)) { 
      /* enumeration */ 
      //device->TriggerActivation.SetValue(e); 
    }
    return true;
  } else if (key=="ExposureMode") {
    double d; 
    std::string s; 
    Basler_GigECameraParams::ExposureModeEnums  e;
    if(props.get(key, d)) { 
      if(d<0)return true;
      e=static_cast< Basler_GigECameraParams::ExposureModeEnums >(d);
      device->ExposureMode.SetValue(e); 
    } else if(props.get(key, s)) { 
      /* enumeration */ 
      //device->ExposureMode.SetValue(e); 
    }
    return true;
  } else if (key=="ExposureAuto") {
    double d; 
    std::string s; 
    Basler_GigECameraParams::ExposureAutoEnums  e;
    if(props.get(key, d)) { 
      if(d<0)return true;
      e=static_cast< Basler_GigECameraParams::ExposureAutoEnums >(d);
      device->ExposureAuto.SetValue(e); 
    } else if(props.get(key, s)) { 
      /* enumeration */ 
      //device->ExposureAuto.SetValue(e); 
    }
    return true;
  } else if (key=="ExposureTimeAbs") {
    double v; 
    if(props.get(key, v)) {
      device->ExposureTimeAbs.SetValue(v);
    }
    return true;
  } else if (key=="ExposureTimeBaseAbs") {
    double v; 
    if(props.get(key, v)) {
      device->ExposureTimeBaseAbs.SetValue(v);
    }
    return true;
  } else if (key=="ExposureTimeBaseAbsEnable") {
    double v; 
    if(props.get(key, v)) {
      device->ExposureTimeBaseAbsEnable.SetValue(v);
    }
    return true;
  } else if (key=="ExposureTimeRaw") {
    double v; 
    if(props.get(key, v)) {
      device->ExposureTimeRaw.SetValue(v);
    }
    return true;
  } else if (key=="AcquisitionLineRateAbs") {
    double v; 
    if(props.get(key, v)) {
      device->AcquisitionLineRateAbs.SetValue(v);
    }
    return true;
  } else if (key=="ResultingLineRateAbs") {
    double v; 
    if(props.get(key, v)) {
      device->ResultingLineRateAbs.SetValue(v);
    }
    return true;
  } else if (key=="AcquisitionFrameRateEnable") {
    double v; 
    if(props.get(key, v)) {
      device->AcquisitionFrameRateEnable.SetValue(v);
    }
    return true;
  } else if (key=="AcquisitionFrameRateAbs") {
    double v; 
    if(props.get(key, v)) {
      device->AcquisitionFrameRateAbs.SetValue(v);
    }
    return true;
  } else if (key=="ResultingFrameRateAbs") {
    double v; 
    if(props.get(key, v)) {
      device->ResultingFrameRateAbs.SetValue(v);
    }
    return true;
  } else if (key=="AveragingNumberOfFrames") {
    double v; 
    if(props.get(key, v)) {
      device->AveragingNumberOfFrames.SetValue(v);
    }
    return true;
  } else if (key=="LineSelector") {
    double d; 
    std::string s; 
    Basler_GigECameraParams::LineSelectorEnums  e;
    if(props.get(key, d)) { 
      if(d<0)return true;
      e=static_cast< Basler_GigECameraParams::LineSelectorEnums >(d);
      device->LineSelector.SetValue(e); 
    } else if(props.get(key, s)) { 
      /* enumeration */ 
      //device->LineSelector.SetValue(e); 
    }
    return true;
  } else if (key=="LineMode") {
    double d; 
    std::string s; 
    Basler_GigECameraParams::LineModeEnums  e;
    if(props.get(key, d)) { 
      if(d<0)return true;
      e=static_cast< Basler_GigECameraParams::LineModeEnums >(d);
      device->LineMode.SetValue(e); 
    } else if(props.get(key, s)) { 
      /* enumeration */ 
      //device->LineMode.SetValue(e); 
    }
    return true;
  } else if (key=="LineFormat") {
    double d; 
    std::string s; 
    Basler_GigECameraParams::LineFormatEnums  e;
    if(props.get(key, d)) { 
      if(d<0)return true;
      e=static_cast< Basler_GigECameraParams::LineFormatEnums >(d);
      device->LineFormat.SetValue(e); 
    } else if(props.get(key, s)) { 
      /* enumeration */ 
      //device->LineFormat.SetValue(e); 
    }
    return true;
  } else if (key=="LineSource") {
    double d; 
    std::string s; 
    Basler_GigECameraParams::LineSourceEnums  e;
    if(props.get(key, d)) { 
      if(d<0)return true;
      e=static_cast< Basler_GigECameraParams::LineSourceEnums >(d);
      device->LineSource.SetValue(e); 
    } else if(props.get(key, s)) { 
      /* enumeration */ 
      //device->LineSource.SetValue(e); 
    }
    return true;
  } else if (key=="LineInverter") {
    double v; 
    if(props.get(key, v)) {
      device->LineInverter.SetValue(v);
    }
    return true;
  } else if (key=="LineTermination") {
    double v; 
    if(props.get(key, v)) {
      device->LineTermination.SetValue(v);
    }
    return true;
  } else if (key=="LineDebouncerTimeAbs") {
    double v; 
    if(props.get(key, v)) {
      device->LineDebouncerTimeAbs.SetValue(v);
    }
    return true;
  } else if (key=="LineDebouncerTimeRaw") {
    double v; 
    if(props.get(key, v)) {
      device->LineDebouncerTimeRaw.SetValue(v);
    }
    return true;
  } else if (key=="LineStatus") {
    double v; 
    if(props.get(key, v)) {
      device->LineStatus.SetValue(v);
    }
    return true;
  } else if (key=="LineStatusAll") {
    double v; 
    if(props.get(key, v)) {
      device->LineStatusAll.SetValue(v);
    }
    return true;
  } else if (key=="UserOutputSelector") {
    double d; 
    std::string s; 
    Basler_GigECameraParams::UserOutputSelectorEnums  e;
    if(props.get(key, d)) { 
      if(d<0)return true;
      e=static_cast< Basler_GigECameraParams::UserOutputSelectorEnums >(d);
      device->UserOutputSelector.SetValue(e); 
    } else if(props.get(key, s)) { 
      /* enumeration */ 
      //device->UserOutputSelector.SetValue(e); 
    }
    return true;
  } else if (key=="UserOutputValue") {
    double v; 
    if(props.get(key, v)) {
      device->UserOutputValue.SetValue(v);
    }
    return true;
  } else if (key=="UserOutputValueAll") {
    double v; 
    if(props.get(key, v)) {
      device->UserOutputValueAll.SetValue(v);
    }
    return true;
  } else if (key=="UserOutputValueAllMask") {
    double v; 
    if(props.get(key, v)) {
      device->UserOutputValueAllMask.SetValue(v);
    }
    return true;
  } else if (key=="ShaftEncoderModuleLineSelector") {
    double d; 
    std::string s; 
    Basler_GigECameraParams::ShaftEncoderModuleLineSelectorEnums  e;
    if(props.get(key, d)) { 
      if(d<0)return true;
      e=static_cast< Basler_GigECameraParams::ShaftEncoderModuleLineSelectorEnums >(d);
      device->ShaftEncoderModuleLineSelector.SetValue(e); 
    } else if(props.get(key, s)) { 
      /* enumeration */ 
      //device->ShaftEncoderModuleLineSelector.SetValue(e); 
    }
    return true;
  } else if (key=="ShaftEncoderModuleLineSource") {
    double d; 
    std::string s; 
    Basler_GigECameraParams::ShaftEncoderModuleLineSourceEnums  e;
    if(props.get(key, d)) { 
      if(d<0)return true;
      e=static_cast< Basler_GigECameraParams::ShaftEncoderModuleLineSourceEnums >(d);
      device->ShaftEncoderModuleLineSource.SetValue(e); 
    } else if(props.get(key, s)) { 
      /* enumeration */ 
      //device->ShaftEncoderModuleLineSource.SetValue(e); 
    }
    return true;
  } else if (key=="ShaftEncoderModuleMode") {
    double d; 
    std::string s; 
    Basler_GigECameraParams::ShaftEncoderModuleModeEnums  e;
    if(props.get(key, d)) { 
      if(d<0)return true;
      e=static_cast< Basler_GigECameraParams::ShaftEncoderModuleModeEnums >(d);
      device->ShaftEncoderModuleMode.SetValue(e); 
    } else if(props.get(key, s)) { 
      /* enumeration */ 
      //device->ShaftEncoderModuleMode.SetValue(e); 
    }
    return true;
  } else if (key=="ShaftEncoderModuleCounterMode") {
    double d; 
    std::string s; 
    Basler_GigECameraParams::ShaftEncoderModuleCounterModeEnums  e;
    if(props.get(key, d)) { 
      if(d<0)return true;
      e=static_cast< Basler_GigECameraParams::ShaftEncoderModuleCounterModeEnums >(d);
      device->ShaftEncoderModuleCounterMode.SetValue(e); 
    } else if(props.get(key, s)) { 
      /* enumeration */ 
      //device->ShaftEncoderModuleCounterMode.SetValue(e); 
    }
    return true;
  } else if (key=="ShaftEncoderModuleCounter") {
    double v; 
    if(props.get(key, v)) {
      device->ShaftEncoderModuleCounter.SetValue(v);
    }
    return true;
  } else if (key=="ShaftEncoderModuleCounterMax") {
    double v; 
    if(props.get(key, v)) {
      device->ShaftEncoderModuleCounterMax.SetValue(v);
    }
    return true;
  } else if (key=="ShaftEncoderModuleReverseCounterMax") {
    double v; 
    if(props.get(key, v)) {
      device->ShaftEncoderModuleReverseCounterMax.SetValue(v);
    }
    return true;
  } else if (key=="TimerDelayTimebaseAbs") {
    double v; 
    if(props.get(key, v)) {
      device->TimerDelayTimebaseAbs.SetValue(v);
    }
    return true;
  } else if (key=="TimerDurationTimebaseAbs") {
    double v; 
    if(props.get(key, v)) {
      device->TimerDurationTimebaseAbs.SetValue(v);
    }
    return true;
  } else if (key=="TimerSelector") {
    double d; 
    std::string s; 
    Basler_GigECameraParams::TimerSelectorEnums  e;
    if(props.get(key, d)) { 
      if(d<0)return true;
      e=static_cast< Basler_GigECameraParams::TimerSelectorEnums >(d);
      device->TimerSelector.SetValue(e); 
    } else if(props.get(key, s)) { 
      /* enumeration */ 
      //device->TimerSelector.SetValue(e); 
    }
    return true;
  } else if (key=="TimerDelayAbs") {
    double v; 
    if(props.get(key, v)) {
      device->TimerDelayAbs.SetValue(v);
    }
    return true;
  } else if (key=="TimerDelayRaw") {
    double v; 
    if(props.get(key, v)) {
      device->TimerDelayRaw.SetValue(v);
    }
    return true;
  } else if (key=="TimerDurationAbs") {
    double v; 
    if(props.get(key, v)) {
      device->TimerDurationAbs.SetValue(v);
    }
    return true;
  } else if (key=="TimerDurationRaw") {
    double v; 
    if(props.get(key, v)) {
      device->TimerDurationRaw.SetValue(v);
    }
    return true;
  } else if (key=="TimerTriggerSource") {
    double d; 
    std::string s; 
    Basler_GigECameraParams::TimerTriggerSourceEnums  e;
    if(props.get(key, d)) { 
      if(d<0)return true;
      e=static_cast< Basler_GigECameraParams::TimerTriggerSourceEnums >(d);
      device->TimerTriggerSource.SetValue(e); 
    } else if(props.get(key, s)) { 
      /* enumeration */ 
      //device->TimerTriggerSource.SetValue(e); 
    }
    return true;
  } else if (key=="TimerTriggerActivation") {
    double d; 
    std::string s; 
    Basler_GigECameraParams::TimerTriggerActivationEnums  e;
    if(props.get(key, d)) { 
      if(d<0)return true;
      e=static_cast< Basler_GigECameraParams::TimerTriggerActivationEnums >(d);
      device->TimerTriggerActivation.SetValue(e); 
    } else if(props.get(key, s)) { 
      /* enumeration */ 
      //device->TimerTriggerActivation.SetValue(e); 
    }
    return true;
  } else if (key=="TimerSequenceEnable") {
    double v; 
    if(props.get(key, v)) {
      device->TimerSequenceEnable.SetValue(v);
    }
    return true;
  } else if (key=="TimerSequenceLastEntryIndex") {
    double v; 
    if(props.get(key, v)) {
      device->TimerSequenceLastEntryIndex.SetValue(v);
    }
    return true;
  } else if (key=="TimerSequenceCurrentEntryIndex") {
    double v; 
    if(props.get(key, v)) {
      device->TimerSequenceCurrentEntryIndex.SetValue(v);
    }
    return true;
  } else if (key=="TimerSequenceEntrySelector") {
    double d; 
    std::string s; 
    Basler_GigECameraParams::TimerSequenceEntrySelectorEnums  e;
    if(props.get(key, d)) { 
      if(d<0)return true;
      e=static_cast< Basler_GigECameraParams::TimerSequenceEntrySelectorEnums >(d);
      device->TimerSequenceEntrySelector.SetValue(e); 
    } else if(props.get(key, s)) { 
      /* enumeration */ 
      //device->TimerSequenceEntrySelector.SetValue(e); 
    }
    return true;
  } else if (key=="TimerSequenceTimerSelector") {
    double d; 
    std::string s; 
    Basler_GigECameraParams::TimerSequenceTimerSelectorEnums  e;
    if(props.get(key, d)) { 
      if(d<0)return true;
      e=static_cast< Basler_GigECameraParams::TimerSequenceTimerSelectorEnums >(d);
      device->TimerSequenceTimerSelector.SetValue(e); 
    } else if(props.get(key, s)) { 
      /* enumeration */ 
      //device->TimerSequenceTimerSelector.SetValue(e); 
    }
    return true;
  } else if (key=="TimerSequenceTimerEnable") {
    double v; 
    if(props.get(key, v)) {
      device->TimerSequenceTimerEnable.SetValue(v);
    }
    return true;
  } else if (key=="TimerSequenceTimerInverter") {
    double v; 
    if(props.get(key, v)) {
      device->TimerSequenceTimerInverter.SetValue(v);
    }
    return true;
  } else if (key=="TimerSequenceTimerDelayRaw") {
    double v; 
    if(props.get(key, v)) {
      device->TimerSequenceTimerDelayRaw.SetValue(v);
    }
    return true;
  } else if (key=="TimerSequenceTimerDurationRaw") {
    double v; 
    if(props.get(key, v)) {
      device->TimerSequenceTimerDurationRaw.SetValue(v);
    }
    return true;
  } else if (key=="LUTSelector") {
    double d; 
    std::string s; 
    Basler_GigECameraParams::LUTSelectorEnums  e;
    if(props.get(key, d)) { 
      if(d<0)return true;
      e=static_cast< Basler_GigECameraParams::LUTSelectorEnums >(d);
      device->LUTSelector.SetValue(e); 
    } else if(props.get(key, s)) { 
      /* enumeration */ 
      //device->LUTSelector.SetValue(e); 
    }
    return true;
  } else if (key=="LUTEnable") {
    double v; 
    if(props.get(key, v)) {
      device->LUTEnable.SetValue(v);
    }
    return true;
  } else if (key=="LUTIndex") {
    double v; 
    if(props.get(key, v)) {
      device->LUTIndex.SetValue(v);
    }
    return true;
  } else if (key=="LUTValue") {
    double v; 
    if(props.get(key, v)) {
      device->LUTValue.SetValue(v);
    }
    return true;
  } else if (key=="UserSetSelector") {
    double d; 
    std::string s; 
    Basler_GigECameraParams::UserSetSelectorEnums  e;
    if(props.get(key, d)) { 
      if(d<0)return true;
      e=static_cast< Basler_GigECameraParams::UserSetSelectorEnums >(d);
      device->UserSetSelector.SetValue(e); 
    } else if(props.get(key, s)) { 
      /* enumeration */ 
      //device->UserSetSelector.SetValue(e); 
    }
    return true;
  } else if (key=="UserSetDefaultSelector") {
    double d; 
    std::string s; 
    Basler_GigECameraParams::UserSetDefaultSelectorEnums  e;
    if(props.get(key, d)) { 
      if(d<0)return true;
      e=static_cast< Basler_GigECameraParams::UserSetDefaultSelectorEnums >(d);
      device->UserSetDefaultSelector.SetValue(e); 
    } else if(props.get(key, s)) { 
      /* enumeration */ 
      //device->UserSetDefaultSelector.SetValue(e); 
    }
    return true;
  } else if (key=="AutoTargetValue") {
    double v; 
    if(props.get(key, v)) {
      device->AutoTargetValue.SetValue(v);
    }
    return true;
  } else if (key=="AutoGainRawLowerLimit") {
    double v; 
    if(props.get(key, v)) {
      device->AutoGainRawLowerLimit.SetValue(v);
    }
    return true;
  } else if (key=="AutoGainRawUpperLimit") {
    double v; 
    if(props.get(key, v)) {
      device->AutoGainRawUpperLimit.SetValue(v);
    }
    return true;
  } else if (key=="AutoExposureTimeAbsLowerLimit") {
    double v; 
    if(props.get(key, v)) {
      device->AutoExposureTimeAbsLowerLimit.SetValue(v);
    }
    return true;
  } else if (key=="AutoExposureTimeAbsUpperLimit") {
    double v; 
    if(props.get(key, v)) {
      device->AutoExposureTimeAbsUpperLimit.SetValue(v);
    }
    return true;
  } else if (key=="AutoFunctionAOISelector") {
    double d; 
    std::string s; 
    Basler_GigECameraParams::AutoFunctionAOISelectorEnums  e;
    if(props.get(key, d)) { 
      if(d<0)return true;
      e=static_cast< Basler_GigECameraParams::AutoFunctionAOISelectorEnums >(d);
      device->AutoFunctionAOISelector.SetValue(e); 
    } else if(props.get(key, s)) { 
      /* enumeration */ 
      //device->AutoFunctionAOISelector.SetValue(e); 
    }
    return true;
  } else if (key=="AutoFunctionAOIWidth") {
    double v; 
    if(props.get(key, v)) {
      device->AutoFunctionAOIWidth.SetValue(v);
    }
    return true;
  } else if (key=="AutoFunctionAOIHeight") {
    double v; 
    if(props.get(key, v)) {
      device->AutoFunctionAOIHeight.SetValue(v);
    }
    return true;
  } else if (key=="AutoFunctionAOIOffsetX") {
    double v; 
    if(props.get(key, v)) {
      device->AutoFunctionAOIOffsetX.SetValue(v);
    }
    return true;
  } else if (key=="AutoFunctionAOIOffsetY") {
    double v; 
    if(props.get(key, v)) {
      device->AutoFunctionAOIOffsetY.SetValue(v);
    }
    return true;
  } else if (key=="ShadingSelector") {
    double d; 
    std::string s; 
    Basler_GigECameraParams::ShadingSelectorEnums  e;
    if(props.get(key, d)) { 
      if(d<0)return true;
      e=static_cast< Basler_GigECameraParams::ShadingSelectorEnums >(d);
      device->ShadingSelector.SetValue(e); 
    } else if(props.get(key, s)) { 
      /* enumeration */ 
      //device->ShadingSelector.SetValue(e); 
    }
    return true;
  } else if (key=="ShadingEnable") {
    double v; 
    if(props.get(key, v)) {
      device->ShadingEnable.SetValue(v);
    }
    return true;
  } else if (key=="ShadingStatus") {
    double d; 
    std::string s; 
    Basler_GigECameraParams::ShadingStatusEnums  e;
    if(props.get(key, d)) { 
      if(d<0)return true;
      e=static_cast< Basler_GigECameraParams::ShadingStatusEnums >(d);
      device->ShadingStatus.SetValue(e); 
    } else if(props.get(key, s)) { 
      /* enumeration */ 
      //device->ShadingStatus.SetValue(e); 
    }
    return true;
  } else if (key=="ShadingSetDefaultSelector") {
    double d; 
    std::string s; 
    Basler_GigECameraParams::ShadingSetDefaultSelectorEnums  e;
    if(props.get(key, d)) { 
      if(d<0)return true;
      e=static_cast< Basler_GigECameraParams::ShadingSetDefaultSelectorEnums >(d);
      device->ShadingSetDefaultSelector.SetValue(e); 
    } else if(props.get(key, s)) { 
      /* enumeration */ 
      //device->ShadingSetDefaultSelector.SetValue(e); 
    }
    return true;
  } else if (key=="ShadingSetSelector") {
    double d; 
    std::string s; 
    Basler_GigECameraParams::ShadingSetSelectorEnums  e;
    if(props.get(key, d)) { 
      if(d<0)return true;
      e=static_cast< Basler_GigECameraParams::ShadingSetSelectorEnums >(d);
      device->ShadingSetSelector.SetValue(e); 
    } else if(props.get(key, s)) { 
      /* enumeration */ 
      //device->ShadingSetSelector.SetValue(e); 
    }
    return true;
  } else if (key=="UserDefinedValueSelector") {
    double d; 
    std::string s; 
    Basler_GigECameraParams::UserDefinedValueSelectorEnums  e;
    if(props.get(key, d)) { 
      if(d<0)return true;
      e=static_cast< Basler_GigECameraParams::UserDefinedValueSelectorEnums >(d);
      device->UserDefinedValueSelector.SetValue(e); 
    } else if(props.get(key, s)) { 
      /* enumeration */ 
      //device->UserDefinedValueSelector.SetValue(e); 
    }
    return true;
  } else if (key=="UserDefinedValue") {
    double v; 
    if(props.get(key, v)) {
      device->UserDefinedValue.SetValue(v);
    }
    return true;
  } else if (key=="DeviceVendorName") {
    std::string v; 
    if(props.get(key, v)) { 
      device->DeviceVendorName.SetValue(v.c_str());
    }
    return true;
  } else if (key=="DeviceModelName") {
    std::string v; 
    if(props.get(key, v)) { 
      device->DeviceModelName.SetValue(v.c_str());
    }
    return true;
  } else if (key=="DeviceManufacturerInfo") {
    std::string v; 
    if(props.get(key, v)) { 
      device->DeviceManufacturerInfo.SetValue(v.c_str());
    }
    return true;
  } else if (key=="DeviceVersion") {
    std::string v; 
    if(props.get(key, v)) { 
      device->DeviceVersion.SetValue(v.c_str());
    }
    return true;
  } else if (key=="DeviceFirmwareVersion") {
    std::string v; 
    if(props.get(key, v)) { 
      device->DeviceFirmwareVersion.SetValue(v.c_str());
    }
    return true;
  } else if (key=="DeviceID") {
    std::string v; 
    if(props.get(key, v)) { 
      device->DeviceID.SetValue(v.c_str());
    }
    return true;
  } else if (key=="DeviceUserID") {
    std::string v; 
    if(props.get(key, v)) { 
      device->DeviceUserID.SetValue(v.c_str());
    }
    return true;
  } else if (key=="DeviceScanType") {
    double d; 
    std::string s; 
    Basler_GigECameraParams::DeviceScanTypeEnums  e;
    if(props.get(key, d)) { 
      if(d<0)return true;
      e=static_cast< Basler_GigECameraParams::DeviceScanTypeEnums >(d);
      device->DeviceScanType.SetValue(e); 
    } else if(props.get(key, s)) { 
      /* enumeration */ 
      //device->DeviceScanType.SetValue(e); 
    }
    return true;
  } else if (key=="SensorWidth") {
    double v; 
    if(props.get(key, v)) {
      device->SensorWidth.SetValue(v);
    }
    return true;
  } else if (key=="SensorHeight") {
    double v; 
    if(props.get(key, v)) {
      device->SensorHeight.SetValue(v);
    }
    return true;
  } else if (key=="WidthMax") {
    double v; 
    if(props.get(key, v)) {
      device->WidthMax.SetValue(v);
    }
    return true;
  } else if (key=="HeightMax") {
    double v; 
    if(props.get(key, v)) {
      device->HeightMax.SetValue(v);
    }
    return true;
  } else if (key=="TemperatureSelector") {
    double d; 
    std::string s; 
    Basler_GigECameraParams::TemperatureSelectorEnums  e;
    if(props.get(key, d)) { 
      if(d<0)return true;
      e=static_cast< Basler_GigECameraParams::TemperatureSelectorEnums >(d);
      device->TemperatureSelector.SetValue(e); 
    } else if(props.get(key, s)) { 
      /* enumeration */ 
      //device->TemperatureSelector.SetValue(e); 
    }
    return true;
  } else if (key=="TemperatureAbs") {
    double v; 
    if(props.get(key, v)) {
      device->TemperatureAbs.SetValue(v);
    }
    return true;
  } else if (key=="ParameterSelector") {
    double d; 
    std::string s; 
    Basler_GigECameraParams::ParameterSelectorEnums  e;
    if(props.get(key, d)) { 
      if(d<0)return true;
      e=static_cast< Basler_GigECameraParams::ParameterSelectorEnums >(d);
      device->ParameterSelector.SetValue(e); 
    } else if(props.get(key, s)) { 
      /* enumeration */ 
      //device->ParameterSelector.SetValue(e); 
    }
    return true;
  } else if (key=="RemoveLimits") {
    double v; 
    if(props.get(key, v)) {
      device->RemoveLimits.SetValue(v);
    }
    return true;
  } else if (key=="ExpertFeatureAccessSelector") {
    double d; 
    std::string s; 
    Basler_GigECameraParams::ExpertFeatureAccessSelectorEnums  e;
    if(props.get(key, d)) { 
      if(d<0)return true;
      e=static_cast< Basler_GigECameraParams::ExpertFeatureAccessSelectorEnums >(d);
      device->ExpertFeatureAccessSelector.SetValue(e); 
    } else if(props.get(key, s)) { 
      /* enumeration */ 
      //device->ExpertFeatureAccessSelector.SetValue(e); 
    }
    return true;
  } else if (key=="ExpertFeatureAccessKey") {
    double v; 
    if(props.get(key, v)) {
      device->ExpertFeatureAccessKey.SetValue(v);
    }
    return true;
  } else if (key=="ExpertFeatureEnable") {
    double v; 
    if(props.get(key, v)) {
      device->ExpertFeatureEnable.SetValue(v);
    }
    return true;
  } else if (key=="PixelStepCorrectionEnable") {
    double v; 
    if(props.get(key, v)) {
      device->PixelStepCorrectionEnable.SetValue(v);
    }
    return true;
  } else if (key=="PixelStepCorrectionSelector") {
    double d; 
    std::string s; 
    Basler_GigECameraParams::PixelStepCorrectionSelectorEnums  e;
    if(props.get(key, d)) { 
      if(d<0)return true;
      e=static_cast< Basler_GigECameraParams::PixelStepCorrectionSelectorEnums >(d);
      device->PixelStepCorrectionSelector.SetValue(e); 
    } else if(props.get(key, s)) { 
      /* enumeration */ 
      //device->PixelStepCorrectionSelector.SetValue(e); 
    }
    return true;
  } else if (key=="PixelStepCorrectionValueAbs") {
    double v; 
    if(props.get(key, v)) {
      device->PixelStepCorrectionValueAbs.SetValue(v);
    }
    return true;
  } else if (key=="PixelStepCorrectionValueRaw") {
    double v; 
    if(props.get(key, v)) {
      device->PixelStepCorrectionValueRaw.SetValue(v);
    }
    return true;
  } else if (key=="PixelStepCorrectionBusy") {
    double v; 
    if(props.get(key, v)) {
      device->PixelStepCorrectionBusy.SetValue(v);
    }
    return true;
  } else if (key=="ChunkModeActive") {
    double v; 
    if(props.get(key, v)) {
      device->ChunkModeActive.SetValue(v);
    }
    return true;
  } else if (key=="ChunkSelector") {
    double d; 
    std::string s; 
    Basler_GigECameraParams::ChunkSelectorEnums  e;
    if(props.get(key, d)) { 
      if(d<0)return true;
      e=static_cast< Basler_GigECameraParams::ChunkSelectorEnums >(d);
      device->ChunkSelector.SetValue(e); 
    } else if(props.get(key, s)) { 
      /* enumeration */ 
      //device->ChunkSelector.SetValue(e); 
    }
    return true;
  } else if (key=="ChunkEnable") {
    double v; 
    if(props.get(key, v)) {
      device->ChunkEnable.SetValue(v);
    }
    return true;
  } else if (key=="ChunkStride") {
    double v; 
    if(props.get(key, v)) {
      device->ChunkStride.SetValue(v);
    }
    return true;
  } else if (key=="ChunkOffsetX") {
    double v; 
    if(props.get(key, v)) {
      device->ChunkOffsetX.SetValue(v);
    }
    return true;
  } else if (key=="ChunkOffsetY") {
    double v; 
    if(props.get(key, v)) {
      device->ChunkOffsetY.SetValue(v);
    }
    return true;
  } else if (key=="ChunkWidth") {
    double v; 
    if(props.get(key, v)) {
      device->ChunkWidth.SetValue(v);
    }
    return true;
  } else if (key=="ChunkHeight") {
    double v; 
    if(props.get(key, v)) {
      device->ChunkHeight.SetValue(v);
    }
    return true;
  } else if (key=="ChunkDynamicRangeMin") {
    double v; 
    if(props.get(key, v)) {
      device->ChunkDynamicRangeMin.SetValue(v);
    }
    return true;
  } else if (key=="ChunkDynamicRangeMax") {
    double v; 
    if(props.get(key, v)) {
      device->ChunkDynamicRangeMax.SetValue(v);
    }
    return true;
  } else if (key=="ChunkPixelFormat") {
    double d; 
    std::string s; 
    Basler_GigECameraParams::ChunkPixelFormatEnums  e;
    if(props.get(key, d)) { 
      if(d<0)return true;
      e=static_cast< Basler_GigECameraParams::ChunkPixelFormatEnums >(d);
      device->ChunkPixelFormat.SetValue(e); 
    } else if(props.get(key, s)) { 
      /* enumeration */ 
      //device->ChunkPixelFormat.SetValue(e); 
    }
    return true;
  } else if (key=="ChunkTimestamp") {
    double v; 
    if(props.get(key, v)) {
      device->ChunkTimestamp.SetValue(v);
    }
    return true;
  } else if (key=="ChunkFramecounter") {
    double v; 
    if(props.get(key, v)) {
      device->ChunkFramecounter.SetValue(v);
    }
    return true;
  } else if (key=="ChunkLineStatusAll") {
    double v; 
    if(props.get(key, v)) {
      device->ChunkLineStatusAll.SetValue(v);
    }
    return true;
  } else if (key=="ChunkTriggerinputcounter") {
    double v; 
    if(props.get(key, v)) {
      device->ChunkTriggerinputcounter.SetValue(v);
    }
    return true;
  } else if (key=="ChunkLineTriggerIgnoredCounter") {
    double v; 
    if(props.get(key, v)) {
      device->ChunkLineTriggerIgnoredCounter.SetValue(v);
    }
    return true;
  } else if (key=="ChunkFrameTriggerIgnoredCounter") {
    double v; 
    if(props.get(key, v)) {
      device->ChunkFrameTriggerIgnoredCounter.SetValue(v);
    }
    return true;
  } else if (key=="ChunkFrameTriggerCounter") {
    double v; 
    if(props.get(key, v)) {
      device->ChunkFrameTriggerCounter.SetValue(v);
    }
    return true;
  } else if (key=="ChunkFramesPerTriggerCounter") {
    double v; 
    if(props.get(key, v)) {
      device->ChunkFramesPerTriggerCounter.SetValue(v);
    }
    return true;
  } else if (key=="ChunkLineTriggerEndToEndCounter") {
    double v; 
    if(props.get(key, v)) {
      device->ChunkLineTriggerEndToEndCounter.SetValue(v);
    }
    return true;
  } else if (key=="ChunkPayloadCRC16") {
    double v; 
    if(props.get(key, v)) {
      device->ChunkPayloadCRC16.SetValue(v);
    }
    return true;
  } else if (key=="EventSelector") {
    double d; 
    std::string s; 
    Basler_GigECameraParams::EventSelectorEnums  e;
    if(props.get(key, d)) { 
      if(d<0)return true;
      e=static_cast< Basler_GigECameraParams::EventSelectorEnums >(d);
      device->EventSelector.SetValue(e); 
    } else if(props.get(key, s)) { 
      /* enumeration */ 
      //device->EventSelector.SetValue(e); 
    }
    return true;
  } else if (key=="EventNotification") {
    double d; 
    std::string s; 
    Basler_GigECameraParams::EventNotificationEnums  e;
    if(props.get(key, d)) { 
      if(d<0)return true;
      e=static_cast< Basler_GigECameraParams::EventNotificationEnums >(d);
      device->EventNotification.SetValue(e); 
    } else if(props.get(key, s)) { 
      /* enumeration */ 
      //device->EventNotification.SetValue(e); 
    }
    return true;
  } else if (key=="ExposureEndEventStreamChannelIndex") {
    double v; 
    if(props.get(key, v)) {
      device->ExposureEndEventStreamChannelIndex.SetValue(v);
    }
    return true;
  } else if (key=="ExposureEndEventFrameID") {
    double v; 
    if(props.get(key, v)) {
      device->ExposureEndEventFrameID.SetValue(v);
    }
    return true;
  } else if (key=="ExposureEndEventTimestamp") {
    double v; 
    if(props.get(key, v)) {
      device->ExposureEndEventTimestamp.SetValue(v);
    }
    return true;
  } else if (key=="LineStartOvertriggerEventStreamChannelIndex") {
    double v; 
    if(props.get(key, v)) {
      device->LineStartOvertriggerEventStreamChannelIndex.SetValue(v);
    }
    return true;
  } else if (key=="LineStartOvertriggerEventTimestamp") {
    double v; 
    if(props.get(key, v)) {
      device->LineStartOvertriggerEventTimestamp.SetValue(v);
    }
    return true;
  } else if (key=="FrameStartOvertriggerEventStreamChannelIndex") {
    double v; 
    if(props.get(key, v)) {
      device->FrameStartOvertriggerEventStreamChannelIndex.SetValue(v);
    }
    return true;
  } else if (key=="FrameStartOvertriggerEventTimestamp") {
    double v; 
    if(props.get(key, v)) {
      device->FrameStartOvertriggerEventTimestamp.SetValue(v);
    }
    return true;
  } else if (key=="EventOverrunEventStreamChannelIndex") {
    double v; 
    if(props.get(key, v)) {
      device->EventOverrunEventStreamChannelIndex.SetValue(v);
    }
    return true;
  } else if (key=="EventOverrunEventFrameID") {
    double v; 
    if(props.get(key, v)) {
      device->EventOverrunEventFrameID.SetValue(v);
    }
    return true;
  } else if (key=="EventOverrunEventTimestamp") {
    double v; 
    if(props.get(key, v)) {
      device->EventOverrunEventTimestamp.SetValue(v);
    }
    return true;
  } else if (key=="FileSelector") {
    double d; 
    std::string s; 
    Basler_GigECameraParams::FileSelectorEnums  e;
    if(props.get(key, d)) { 
      if(d<0)return true;
      e=static_cast< Basler_GigECameraParams::FileSelectorEnums >(d);
      device->FileSelector.SetValue(e); 
    } else if(props.get(key, s)) { 
      /* enumeration */ 
      //device->FileSelector.SetValue(e); 
    }
    return true;
  } else if (key=="FileOperationSelector") {
    double d; 
    std::string s; 
    Basler_GigECameraParams::FileOperationSelectorEnums  e;
    if(props.get(key, d)) { 
      if(d<0)return true;
      e=static_cast< Basler_GigECameraParams::FileOperationSelectorEnums >(d);
      device->FileOperationSelector.SetValue(e); 
    } else if(props.get(key, s)) { 
      /* enumeration */ 
      //device->FileOperationSelector.SetValue(e); 
    }
    return true;
  } else if (key=="FileOpenMode") {
    double d; 
    std::string s; 
    Basler_GigECameraParams::FileOpenModeEnums  e;
    if(props.get(key, d)) { 
      if(d<0)return true;
      e=static_cast< Basler_GigECameraParams::FileOpenModeEnums >(d);
      device->FileOpenMode.SetValue(e); 
    } else if(props.get(key, s)) { 
      /* enumeration */ 
      //device->FileOpenMode.SetValue(e); 
    }
    return true;
  } else if (key=="FileAccessOffset") {
    double v; 
    if(props.get(key, v)) {
      device->FileAccessOffset.SetValue(v);
    }
    return true;
  } else if (key=="FileAccessLength") {
    double v; 
    if(props.get(key, v)) {
      device->FileAccessLength.SetValue(v);
    }
    return true;
  } else if (key=="FileOperationStatus") {
    double d; 
    std::string s; 
    Basler_GigECameraParams::FileOperationStatusEnums  e;
    if(props.get(key, d)) { 
      if(d<0)return true;
      e=static_cast< Basler_GigECameraParams::FileOperationStatusEnums >(d);
      device->FileOperationStatus.SetValue(e); 
    } else if(props.get(key, s)) { 
      /* enumeration */ 
      //device->FileOperationStatus.SetValue(e); 
    }
    return true;
  } else if (key=="FileOperationResult") {
    double v; 
    if(props.get(key, v)) {
      device->FileOperationResult.SetValue(v);
    }
    return true;
  } else if (key=="FileSize") {
    double v; 
    if(props.get(key, v)) {
      device->FileSize.SetValue(v);
    }
    return true;
  } else if (key=="PayloadSize") {
    double v; 
    if(props.get(key, v)) {
      device->PayloadSize.SetValue(v);
    }
    return true;
  } else if (key=="GevVersionMajor") {
    double v; 
    if(props.get(key, v)) {
      device->GevVersionMajor.SetValue(v);
    }
    return true;
  } else if (key=="GevVersionMinor") {
    double v; 
    if(props.get(key, v)) {
      device->GevVersionMinor.SetValue(v);
    }
    return true;
  } else if (key=="GevDeviceModeIsBigEndian") {
    double v; 
    if(props.get(key, v)) {
      device->GevDeviceModeIsBigEndian.SetValue(v);
    }
    return true;
  } else if (key=="GevDeviceModeCharacterSet") {
    double v; 
    if(props.get(key, v)) {
      device->GevDeviceModeCharacterSet.SetValue(v);
    }
    return true;
  } else if (key=="GevInterfaceSelector") {
    double d; 
    std::string s; 
    Basler_GigECameraParams::GevInterfaceSelectorEnums  e;
    if(props.get(key, d)) { 
      if(d<0)return true;
      e=static_cast< Basler_GigECameraParams::GevInterfaceSelectorEnums >(d);
      device->GevInterfaceSelector.SetValue(e); 
    } else if(props.get(key, s)) { 
      /* enumeration */ 
      //device->GevInterfaceSelector.SetValue(e); 
    }
    return true;
  } else if (key=="GevMACAddress") {
    double v; 
    if(props.get(key, v)) {
      device->GevMACAddress.SetValue(v);
    }
    return true;
  } else if (key=="GevSupportedIPConfigurationLLA") {
    double v; 
    if(props.get(key, v)) {
      device->GevSupportedIPConfigurationLLA.SetValue(v);
    }
    return true;
  } else if (key=="GevSupportedIPConfigurationDHCP") {
    double v; 
    if(props.get(key, v)) {
      device->GevSupportedIPConfigurationDHCP.SetValue(v);
    }
    return true;
  } else if (key=="GevSupportedIPConfigurationPersistentIP") {
    double v; 
    if(props.get(key, v)) {
      device->GevSupportedIPConfigurationPersistentIP.SetValue(v);
    }
    return true;
  } else if (key=="GevCurrentIPConfiguration") {
    double v; 
    if(props.get(key, v)) {
      device->GevCurrentIPConfiguration.SetValue(v);
    }
    return true;
  } else if (key=="GevCurrentIPAddress") {
    double v; 
    if(props.get(key, v)) {
      device->GevCurrentIPAddress.SetValue(v);
    }
    return true;
  } else if (key=="GevCurrentSubnetMask") {
    double v; 
    if(props.get(key, v)) {
      device->GevCurrentSubnetMask.SetValue(v);
    }
    return true;
  } else if (key=="GevCurrentDefaultGateway") {
    double v; 
    if(props.get(key, v)) {
      device->GevCurrentDefaultGateway.SetValue(v);
    }
    return true;
  } else if (key=="GevPersistentIPAddress") {
    double v; 
    if(props.get(key, v)) {
      device->GevPersistentIPAddress.SetValue(v);
    }
    return true;
  } else if (key=="GevPersistentSubnetMask") {
    double v; 
    if(props.get(key, v)) {
      device->GevPersistentSubnetMask.SetValue(v);
    }
    return true;
  } else if (key=="GevPersistentDefaultGateway") {
    double v; 
    if(props.get(key, v)) {
      device->GevPersistentDefaultGateway.SetValue(v);
    }
    return true;
  } else if (key=="GevLinkSpeed") {
    double v; 
    if(props.get(key, v)) {
      device->GevLinkSpeed.SetValue(v);
    }
    return true;
  } else if (key=="GevFirstURL") {
    std::string v; 
    if(props.get(key, v)) { 
      device->GevFirstURL.SetValue(v.c_str());
    }
    return true;
  } else if (key=="GevSecondURL") {
    std::string v; 
    if(props.get(key, v)) { 
      device->GevSecondURL.SetValue(v.c_str());
    }
    return true;
  } else if (key=="GevNumberOfInterfaces") {
    double v; 
    if(props.get(key, v)) {
      device->GevNumberOfInterfaces.SetValue(v);
    }
    return true;
  } else if (key=="GevMessageChannelCount") {
    double v; 
    if(props.get(key, v)) {
      device->GevMessageChannelCount.SetValue(v);
    }
    return true;
  } else if (key=="GevStreamChannelCount") {
    double v; 
    if(props.get(key, v)) {
      device->GevStreamChannelCount.SetValue(v);
    }
    return true;
  } else if (key=="GevSupportedOptionalCommandsEVENTDATA") {
    double v; 
    if(props.get(key, v)) {
      device->GevSupportedOptionalCommandsEVENTDATA.SetValue(v);
    }
    return true;
  } else if (key=="GevSupportedOptionalCommandsEVENT") {
    double v; 
    if(props.get(key, v)) {
      device->GevSupportedOptionalCommandsEVENT.SetValue(v);
    }
    return true;
  } else if (key=="GevSupportedOptionalCommandsPACKETRESEND") {
    double v; 
    if(props.get(key, v)) {
      device->GevSupportedOptionalCommandsPACKETRESEND.SetValue(v);
    }
    return true;
  } else if (key=="GevSupportedOptionalCommandsWRITEMEM") {
    double v; 
    if(props.get(key, v)) {
      device->GevSupportedOptionalCommandsWRITEMEM.SetValue(v);
    }
    return true;
  } else if (key=="GevSupportedOptionalCommandsConcatenation") {
    double v; 
    if(props.get(key, v)) {
      device->GevSupportedOptionalCommandsConcatenation.SetValue(v);
    }
    return true;
  } else if (key=="GevHeartbeatTimeout") {
    double v; 
    if(props.get(key, v)) {
      device->GevHeartbeatTimeout.SetValue(v);
    }
    return true;
  } else if (key=="GevTimestampTickFrequency") {
    double v; 
    if(props.get(key, v)) {
      device->GevTimestampTickFrequency.SetValue(v);
    }
    return true;
  } else if (key=="GevTimestampValue") {
    double v; 
    if(props.get(key, v)) {
      device->GevTimestampValue.SetValue(v);
    }
    return true;
  } else if (key=="GevCCP") {
    double d; 
    std::string s; 
    Basler_GigECameraParams::GevCCPEnums  e;
    if(props.get(key, d)) { 
      if(d<0)return true;
      e=static_cast< Basler_GigECameraParams::GevCCPEnums >(d);
      device->GevCCP.SetValue(e); 
    } else if(props.get(key, s)) { 
      /* enumeration */ 
      //device->GevCCP.SetValue(e); 
    }
    return true;
  } else if (key=="GevStreamChannelSelector") {
    double d; 
    std::string s; 
    Basler_GigECameraParams::GevStreamChannelSelectorEnums  e;
    if(props.get(key, d)) { 
      if(d<0)return true;
      e=static_cast< Basler_GigECameraParams::GevStreamChannelSelectorEnums >(d);
      device->GevStreamChannelSelector.SetValue(e); 
    } else if(props.get(key, s)) { 
      /* enumeration */ 
      //device->GevStreamChannelSelector.SetValue(e); 
    }
    return true;
  } else if (key=="GevSCPInterfaceIndex") {
    double v; 
    if(props.get(key, v)) {
      device->GevSCPInterfaceIndex.SetValue(v);
    }
    return true;
  } else if (key=="GevSCDA") {
    double v; 
    if(props.get(key, v)) {
      device->GevSCDA.SetValue(v);
    }
    return true;
  } else if (key=="GevSCPHostPort") {
    double v; 
    if(props.get(key, v)) {
      device->GevSCPHostPort.SetValue(v);
    }
    return true;
  } else if (key=="GevSCPSPacketSize") {
    double v; 
    if(props.get(key, v)) {
      device->GevSCPSPacketSize.SetValue(v);
    }
    return true;
  } else if (key=="GevSCPD") {
    double v; 
    if(props.get(key, v)) {
      device->GevSCPD.SetValue(v);
    }
    return true;
  } else if (key=="GevSCFTD") {
    double v; 
    if(props.get(key, v)) {
      device->GevSCFTD.SetValue(v);
    }
    return true;
  } else if (key=="GevSCBWR") {
    double v; 
    if(props.get(key, v)) {
      device->GevSCBWR.SetValue(v);
    }
    return true;
  } else if (key=="GevSCBWRA") {
    double v; 
    if(props.get(key, v)) {
      device->GevSCBWRA.SetValue(v);
    }
    return true;
  } else if (key=="GevSCBWA") {
    double v; 
    if(props.get(key, v)) {
      device->GevSCBWA.SetValue(v);
    }
    return true;
  } else if (key=="GevSCDMT") {
    double v; 
    if(props.get(key, v)) {
      device->GevSCDMT.SetValue(v);
    }
    return true;
  } else if (key=="GevSCDCT") {
    double v; 
    if(props.get(key, v)) {
      device->GevSCDCT.SetValue(v);
    }
    return true;
  } else if (key=="GevSCFJM") {
    double v; 
    if(props.get(key, v)) {
      device->GevSCFJM.SetValue(v);
    }
    return true;
  } else if (key=="TLParamsLocked") {
    double v; 
    if(props.get(key, v)) {
      device->TLParamsLocked.SetValue(v);
    }
    return true;
  }
  return false;
}
#endif /* HAVE_PYLON */
