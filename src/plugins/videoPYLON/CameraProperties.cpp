// get GigE-Camera attributes
#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#ifdef HAVE_PYLON
#include "CameraProperties.h"
#include <map>

namespace gem{namespace pylon{namespace cameraproperties{
  static gem::Properties writeprops, readprops;

  typedef Pylon::CBaslerGigECamera DEVICE;
  /* GenApi::IBoolean */
  typedef bool (*t_getbool)(DEVICE*device);
  std::map<std::string, t_getbool>map_getbool;
  static bool getGammaEnable(DEVICE*device) { return device->GammaEnable.GetValue(); }
  static bool getExposureTimeBaseAbsEnable(DEVICE*device) { return device->ExposureTimeBaseAbsEnable.GetValue(); }
  static bool getAcquisitionFrameRateEnable(DEVICE*device) { return device->AcquisitionFrameRateEnable.GetValue(); }
  static bool getLineInverter(DEVICE*device) { return device->LineInverter.GetValue(); }
  static bool getLineTermination(DEVICE*device) { return device->LineTermination.GetValue(); }
  static bool getLineStatus(DEVICE*device) { return device->LineStatus.GetValue(); }
  static bool getUserOutputValue(DEVICE*device) { return device->UserOutputValue.GetValue(); }
  static bool getTimerSequenceEnable(DEVICE*device) { return device->TimerSequenceEnable.GetValue(); }
  static bool getTimerSequenceTimerEnable(DEVICE*device) { return device->TimerSequenceTimerEnable.GetValue(); }
  static bool getTimerSequenceTimerInverter(DEVICE*device) { return device->TimerSequenceTimerInverter.GetValue(); }
  static bool getLUTEnable(DEVICE*device) { return device->LUTEnable.GetValue(); }
  static bool getShadingEnable(DEVICE*device) { return device->ShadingEnable.GetValue(); }
  static bool getRemoveLimits(DEVICE*device) { return device->RemoveLimits.GetValue(); }
  static bool getExpertFeatureEnable(DEVICE*device) { return device->ExpertFeatureEnable.GetValue(); }
  static bool getPixelStepCorrectionEnable(DEVICE*device) { return device->PixelStepCorrectionEnable.GetValue(); }
  static bool getChunkModeActive(DEVICE*device) { return device->ChunkModeActive.GetValue(); }
  static bool getChunkEnable(DEVICE*device) { return device->ChunkEnable.GetValue(); }
  static bool getGevDeviceModeIsBigEndian(DEVICE*device) { return device->GevDeviceModeIsBigEndian.GetValue(); }
  static bool getGevSupportedIPConfigurationLLA(DEVICE*device) { return device->GevSupportedIPConfigurationLLA.GetValue(); }
  static bool getGevSupportedIPConfigurationDHCP(DEVICE*device) { return device->GevSupportedIPConfigurationDHCP.GetValue(); }
  static bool getGevSupportedIPConfigurationPersistentIP(DEVICE*device) { return device->GevSupportedIPConfigurationPersistentIP.GetValue(); }
  static bool getGevSupportedOptionalCommandsEVENTDATA(DEVICE*device) { return device->GevSupportedOptionalCommandsEVENTDATA.GetValue(); }
  static bool getGevSupportedOptionalCommandsEVENT(DEVICE*device) { return device->GevSupportedOptionalCommandsEVENT.GetValue(); }
  static bool getGevSupportedOptionalCommandsPACKETRESEND(DEVICE*device) { return device->GevSupportedOptionalCommandsPACKETRESEND.GetValue(); }
  static bool getGevSupportedOptionalCommandsWRITEMEM(DEVICE*device) { return device->GevSupportedOptionalCommandsWRITEMEM.GetValue(); }
  static bool getGevSupportedOptionalCommandsConcatenation(DEVICE*device) { return device->GevSupportedOptionalCommandsConcatenation.GetValue(); }

  typedef void (*t_setbool)(DEVICE*device, const bool);
  std::map<std::string, t_setbool>map_setbool;
  static void setGammaEnable(DEVICE*device, const bool v) { device->GammaEnable.SetValue(v); }
  static void setExposureTimeBaseAbsEnable(DEVICE*device, const bool v) { device->ExposureTimeBaseAbsEnable.SetValue(v); }
  static void setAcquisitionFrameRateEnable(DEVICE*device, const bool v) { device->AcquisitionFrameRateEnable.SetValue(v); }
  static void setLineInverter(DEVICE*device, const bool v) { device->LineInverter.SetValue(v); }
  static void setLineTermination(DEVICE*device, const bool v) { device->LineTermination.SetValue(v); }
  static void setLineStatus(DEVICE*device, const bool v) { device->LineStatus.SetValue(v); }
  static void setUserOutputValue(DEVICE*device, const bool v) { device->UserOutputValue.SetValue(v); }
  static void setTimerSequenceEnable(DEVICE*device, const bool v) { device->TimerSequenceEnable.SetValue(v); }
  static void setTimerSequenceTimerEnable(DEVICE*device, const bool v) { device->TimerSequenceTimerEnable.SetValue(v); }
  static void setTimerSequenceTimerInverter(DEVICE*device, const bool v) { device->TimerSequenceTimerInverter.SetValue(v); }
  static void setLUTEnable(DEVICE*device, const bool v) { device->LUTEnable.SetValue(v); }
  static void setShadingEnable(DEVICE*device, const bool v) { device->ShadingEnable.SetValue(v); }
  static void setRemoveLimits(DEVICE*device, const bool v) { device->RemoveLimits.SetValue(v); }
  static void setExpertFeatureEnable(DEVICE*device, const bool v) { device->ExpertFeatureEnable.SetValue(v); }
  static void setPixelStepCorrectionEnable(DEVICE*device, const bool v) { device->PixelStepCorrectionEnable.SetValue(v); }
  static void setChunkModeActive(DEVICE*device, const bool v) { device->ChunkModeActive.SetValue(v); }
  static void setChunkEnable(DEVICE*device, const bool v) { device->ChunkEnable.SetValue(v); }
  static void setGevDeviceModeIsBigEndian(DEVICE*device, const bool v) { device->GevDeviceModeIsBigEndian.SetValue(v); }
  static void setGevSupportedIPConfigurationLLA(DEVICE*device, const bool v) { device->GevSupportedIPConfigurationLLA.SetValue(v); }
  static void setGevSupportedIPConfigurationDHCP(DEVICE*device, const bool v) { device->GevSupportedIPConfigurationDHCP.SetValue(v); }
  static void setGevSupportedIPConfigurationPersistentIP(DEVICE*device, const bool v) { device->GevSupportedIPConfigurationPersistentIP.SetValue(v); }
  static void setGevSupportedOptionalCommandsEVENTDATA(DEVICE*device, const bool v) { device->GevSupportedOptionalCommandsEVENTDATA.SetValue(v); }
  static void setGevSupportedOptionalCommandsEVENT(DEVICE*device, const bool v) { device->GevSupportedOptionalCommandsEVENT.SetValue(v); }
  static void setGevSupportedOptionalCommandsPACKETRESEND(DEVICE*device, const bool v) { device->GevSupportedOptionalCommandsPACKETRESEND.SetValue(v); }
  static void setGevSupportedOptionalCommandsWRITEMEM(DEVICE*device, const bool v) { device->GevSupportedOptionalCommandsWRITEMEM.SetValue(v); }
  static void setGevSupportedOptionalCommandsConcatenation(DEVICE*device, const bool v) { device->GevSupportedOptionalCommandsConcatenation.SetValue(v); }

  /* GenApi::IInteger */
  typedef int64_t (*t_getint)(DEVICE*device);
  std::map<std::string, t_getint>map_getint;
  static int64_t getGainRaw(DEVICE*device) { return device->GainRaw.GetValue(); }
  static int64_t getBlackLevelRaw(DEVICE*device) { return device->BlackLevelRaw.GetValue(); }
  static int64_t getBalanceRatioRaw(DEVICE*device) { return device->BalanceRatioRaw.GetValue(); }
  static int64_t getDigitalShift(DEVICE*device) { return device->DigitalShift.GetValue(); }
  static int64_t getPixelDynamicRangeMin(DEVICE*device) { return device->PixelDynamicRangeMin.GetValue(); }
  static int64_t getPixelDynamicRangeMax(DEVICE*device) { return device->PixelDynamicRangeMax.GetValue(); }
  static int64_t getSpatialCorrection(DEVICE*device) { return device->SpatialCorrection.GetValue(); }
  static int64_t getSpatialCorrectionAmount(DEVICE*device) { return device->SpatialCorrectionAmount.GetValue(); }
  static int64_t getWidth(DEVICE*device) { return device->Width.GetValue(); }
  static int64_t getHeight(DEVICE*device) { return device->Height.GetValue(); }
  static int64_t getOffsetX(DEVICE*device) { return device->OffsetX.GetValue(); }
  static int64_t getOffsetY(DEVICE*device) { return device->OffsetY.GetValue(); }
  static int64_t getBinningVertical(DEVICE*device) { return device->BinningVertical.GetValue(); }
  static int64_t getBinningHorizontal(DEVICE*device) { return device->BinningHorizontal.GetValue(); }
  static int64_t getExposureTimeRaw(DEVICE*device) { return device->ExposureTimeRaw.GetValue(); }
  static int64_t getAveragingNumberOfFrames(DEVICE*device) { return device->AveragingNumberOfFrames.GetValue(); }
  static int64_t getLineDebouncerTimeRaw(DEVICE*device) { return device->LineDebouncerTimeRaw.GetValue(); }
  static int64_t getLineStatusAll(DEVICE*device) { return device->LineStatusAll.GetValue(); }
  static int64_t getUserOutputValueAll(DEVICE*device) { return device->UserOutputValueAll.GetValue(); }
  static int64_t getUserOutputValueAllMask(DEVICE*device) { return device->UserOutputValueAllMask.GetValue(); }
  static int64_t getShaftEncoderModuleCounter(DEVICE*device) { return device->ShaftEncoderModuleCounter.GetValue(); }
  static int64_t getShaftEncoderModuleCounterMax(DEVICE*device) { return device->ShaftEncoderModuleCounterMax.GetValue(); }
  static int64_t getShaftEncoderModuleReverseCounterMax(DEVICE*device) { return device->ShaftEncoderModuleReverseCounterMax.GetValue(); }
  static int64_t getTimerDelayRaw(DEVICE*device) { return device->TimerDelayRaw.GetValue(); }
  static int64_t getTimerDurationRaw(DEVICE*device) { return device->TimerDurationRaw.GetValue(); }
  static int64_t getTimerSequenceLastEntryIndex(DEVICE*device) { return device->TimerSequenceLastEntryIndex.GetValue(); }
  static int64_t getTimerSequenceCurrentEntryIndex(DEVICE*device) { return device->TimerSequenceCurrentEntryIndex.GetValue(); }
  static int64_t getTimerSequenceTimerDelayRaw(DEVICE*device) { return device->TimerSequenceTimerDelayRaw.GetValue(); }
  static int64_t getTimerSequenceTimerDurationRaw(DEVICE*device) { return device->TimerSequenceTimerDurationRaw.GetValue(); }
  static int64_t getLUTIndex(DEVICE*device) { return device->LUTIndex.GetValue(); }
  static int64_t getLUTValue(DEVICE*device) { return device->LUTValue.GetValue(); }
  static int64_t getAutoTargetValue(DEVICE*device) { return device->AutoTargetValue.GetValue(); }
  static int64_t getAutoGainRawLowerLimit(DEVICE*device) { return device->AutoGainRawLowerLimit.GetValue(); }
  static int64_t getAutoGainRawUpperLimit(DEVICE*device) { return device->AutoGainRawUpperLimit.GetValue(); }
  static int64_t getAutoFunctionAOIWidth(DEVICE*device) { return device->AutoFunctionAOIWidth.GetValue(); }
  static int64_t getAutoFunctionAOIHeight(DEVICE*device) { return device->AutoFunctionAOIHeight.GetValue(); }
  static int64_t getAutoFunctionAOIOffsetX(DEVICE*device) { return device->AutoFunctionAOIOffsetX.GetValue(); }
  static int64_t getAutoFunctionAOIOffsetY(DEVICE*device) { return device->AutoFunctionAOIOffsetY.GetValue(); }
  static int64_t getUserDefinedValue(DEVICE*device) { return device->UserDefinedValue.GetValue(); }
  static int64_t getSensorWidth(DEVICE*device) { return device->SensorWidth.GetValue(); }
  static int64_t getSensorHeight(DEVICE*device) { return device->SensorHeight.GetValue(); }
  static int64_t getWidthMax(DEVICE*device) { return device->WidthMax.GetValue(); }
  static int64_t getHeightMax(DEVICE*device) { return device->HeightMax.GetValue(); }
  static int64_t getExpertFeatureAccessKey(DEVICE*device) { return device->ExpertFeatureAccessKey.GetValue(); }
  static int64_t getPixelStepCorrectionValueRaw(DEVICE*device) { return device->PixelStepCorrectionValueRaw.GetValue(); }
  static int64_t getPixelStepCorrectionBusy(DEVICE*device) { return device->PixelStepCorrectionBusy.GetValue(); }
  static int64_t getChunkStride(DEVICE*device) { return device->ChunkStride.GetValue(); }
  static int64_t getChunkOffsetX(DEVICE*device) { return device->ChunkOffsetX.GetValue(); }
  static int64_t getChunkOffsetY(DEVICE*device) { return device->ChunkOffsetY.GetValue(); }
  static int64_t getChunkWidth(DEVICE*device) { return device->ChunkWidth.GetValue(); }
  static int64_t getChunkHeight(DEVICE*device) { return device->ChunkHeight.GetValue(); }
  static int64_t getChunkDynamicRangeMin(DEVICE*device) { return device->ChunkDynamicRangeMin.GetValue(); }
  static int64_t getChunkDynamicRangeMax(DEVICE*device) { return device->ChunkDynamicRangeMax.GetValue(); }
  static int64_t getChunkTimestamp(DEVICE*device) { return device->ChunkTimestamp.GetValue(); }
  static int64_t getChunkFramecounter(DEVICE*device) { return device->ChunkFramecounter.GetValue(); }
  static int64_t getChunkLineStatusAll(DEVICE*device) { return device->ChunkLineStatusAll.GetValue(); }
  static int64_t getChunkTriggerinputcounter(DEVICE*device) { return device->ChunkTriggerinputcounter.GetValue(); }
  static int64_t getChunkLineTriggerIgnoredCounter(DEVICE*device) { return device->ChunkLineTriggerIgnoredCounter.GetValue(); }
  static int64_t getChunkFrameTriggerIgnoredCounter(DEVICE*device) { return device->ChunkFrameTriggerIgnoredCounter.GetValue(); }
  static int64_t getChunkFrameTriggerCounter(DEVICE*device) { return device->ChunkFrameTriggerCounter.GetValue(); }
  static int64_t getChunkFramesPerTriggerCounter(DEVICE*device) { return device->ChunkFramesPerTriggerCounter.GetValue(); }
  static int64_t getChunkLineTriggerEndToEndCounter(DEVICE*device) { return device->ChunkLineTriggerEndToEndCounter.GetValue(); }
  static int64_t getChunkPayloadCRC16(DEVICE*device) { return device->ChunkPayloadCRC16.GetValue(); }
  static int64_t getExposureEndEventStreamChannelIndex(DEVICE*device) { return device->ExposureEndEventStreamChannelIndex.GetValue(); }
  static int64_t getExposureEndEventFrameID(DEVICE*device) { return device->ExposureEndEventFrameID.GetValue(); }
  static int64_t getExposureEndEventTimestamp(DEVICE*device) { return device->ExposureEndEventTimestamp.GetValue(); }
  static int64_t getLineStartOvertriggerEventStreamChannelIndex(DEVICE*device) { return device->LineStartOvertriggerEventStreamChannelIndex.GetValue(); }
  static int64_t getLineStartOvertriggerEventTimestamp(DEVICE*device) { return device->LineStartOvertriggerEventTimestamp.GetValue(); }
  static int64_t getFrameStartOvertriggerEventStreamChannelIndex(DEVICE*device) { return device->FrameStartOvertriggerEventStreamChannelIndex.GetValue(); }
  static int64_t getFrameStartOvertriggerEventTimestamp(DEVICE*device) { return device->FrameStartOvertriggerEventTimestamp.GetValue(); }
  static int64_t getEventOverrunEventStreamChannelIndex(DEVICE*device) { return device->EventOverrunEventStreamChannelIndex.GetValue(); }
  static int64_t getEventOverrunEventFrameID(DEVICE*device) { return device->EventOverrunEventFrameID.GetValue(); }
  static int64_t getEventOverrunEventTimestamp(DEVICE*device) { return device->EventOverrunEventTimestamp.GetValue(); }
  static int64_t getFileAccessOffset(DEVICE*device) { return device->FileAccessOffset.GetValue(); }
  static int64_t getFileAccessLength(DEVICE*device) { return device->FileAccessLength.GetValue(); }
  static int64_t getFileOperationResult(DEVICE*device) { return device->FileOperationResult.GetValue(); }
  static int64_t getFileSize(DEVICE*device) { return device->FileSize.GetValue(); }
  static int64_t getPayloadSize(DEVICE*device) { return device->PayloadSize.GetValue(); }
  static int64_t getGevVersionMajor(DEVICE*device) { return device->GevVersionMajor.GetValue(); }
  static int64_t getGevVersionMinor(DEVICE*device) { return device->GevVersionMinor.GetValue(); }
  static int64_t getGevDeviceModeCharacterSet(DEVICE*device) { return device->GevDeviceModeCharacterSet.GetValue(); }
  static int64_t getGevMACAddress(DEVICE*device) { return device->GevMACAddress.GetValue(); }
  static int64_t getGevCurrentIPConfiguration(DEVICE*device) { return device->GevCurrentIPConfiguration.GetValue(); }
  static int64_t getGevCurrentIPAddress(DEVICE*device) { return device->GevCurrentIPAddress.GetValue(); }
  static int64_t getGevCurrentSubnetMask(DEVICE*device) { return device->GevCurrentSubnetMask.GetValue(); }
  static int64_t getGevCurrentDefaultGateway(DEVICE*device) { return device->GevCurrentDefaultGateway.GetValue(); }
  static int64_t getGevPersistentIPAddress(DEVICE*device) { return device->GevPersistentIPAddress.GetValue(); }
  static int64_t getGevPersistentSubnetMask(DEVICE*device) { return device->GevPersistentSubnetMask.GetValue(); }
  static int64_t getGevPersistentDefaultGateway(DEVICE*device) { return device->GevPersistentDefaultGateway.GetValue(); }
  static int64_t getGevLinkSpeed(DEVICE*device) { return device->GevLinkSpeed.GetValue(); }
  static int64_t getGevNumberOfInterfaces(DEVICE*device) { return device->GevNumberOfInterfaces.GetValue(); }
  static int64_t getGevMessageChannelCount(DEVICE*device) { return device->GevMessageChannelCount.GetValue(); }
  static int64_t getGevStreamChannelCount(DEVICE*device) { return device->GevStreamChannelCount.GetValue(); }
  static int64_t getGevHeartbeatTimeout(DEVICE*device) { return device->GevHeartbeatTimeout.GetValue(); }
  static int64_t getGevTimestampTickFrequency(DEVICE*device) { return device->GevTimestampTickFrequency.GetValue(); }
  static int64_t getGevTimestampValue(DEVICE*device) { return device->GevTimestampValue.GetValue(); }
  static int64_t getGevSCPInterfaceIndex(DEVICE*device) { return device->GevSCPInterfaceIndex.GetValue(); }
  static int64_t getGevSCDA(DEVICE*device) { return device->GevSCDA.GetValue(); }
  static int64_t getGevSCPHostPort(DEVICE*device) { return device->GevSCPHostPort.GetValue(); }
  static int64_t getGevSCPSPacketSize(DEVICE*device) { return device->GevSCPSPacketSize.GetValue(); }
  static int64_t getGevSCPD(DEVICE*device) { return device->GevSCPD.GetValue(); }
  static int64_t getGevSCFTD(DEVICE*device) { return device->GevSCFTD.GetValue(); }
  static int64_t getGevSCBWR(DEVICE*device) { return device->GevSCBWR.GetValue(); }
  static int64_t getGevSCBWRA(DEVICE*device) { return device->GevSCBWRA.GetValue(); }
  static int64_t getGevSCBWA(DEVICE*device) { return device->GevSCBWA.GetValue(); }
  static int64_t getGevSCDMT(DEVICE*device) { return device->GevSCDMT.GetValue(); }
  static int64_t getGevSCDCT(DEVICE*device) { return device->GevSCDCT.GetValue(); }
  static int64_t getGevSCFJM(DEVICE*device) { return device->GevSCFJM.GetValue(); }
  static int64_t getTLParamsLocked(DEVICE*device) { return device->TLParamsLocked.GetValue(); }


  typedef void (*t_setint)(DEVICE*device, const int64_t);
  std::map<std::string, t_setint>map_setint;
  static void setGainRaw(DEVICE*device, const int64_t v) { device->GainRaw.SetValue(v); }
  static void setBlackLevelRaw(DEVICE*device, const int64_t v) { device->BlackLevelRaw.SetValue(v); }
  static void setBalanceRatioRaw(DEVICE*device, const int64_t v) { device->BalanceRatioRaw.SetValue(v); }
  static void setDigitalShift(DEVICE*device, const int64_t v) { device->DigitalShift.SetValue(v); }
  static void setPixelDynamicRangeMin(DEVICE*device, const int64_t v) { device->PixelDynamicRangeMin.SetValue(v); }
  static void setPixelDynamicRangeMax(DEVICE*device, const int64_t v) { device->PixelDynamicRangeMax.SetValue(v); }
  static void setSpatialCorrection(DEVICE*device, const int64_t v) { device->SpatialCorrection.SetValue(v); }
  static void setSpatialCorrectionAmount(DEVICE*device, const int64_t v) { device->SpatialCorrectionAmount.SetValue(v); }
  static void setWidth(DEVICE*device, const int64_t v) { device->Width.SetValue(v); }
  static void setHeight(DEVICE*device, const int64_t v) { device->Height.SetValue(v); }
  static void setOffsetX(DEVICE*device, const int64_t v) { device->OffsetX.SetValue(v); }
  static void setOffsetY(DEVICE*device, const int64_t v) { device->OffsetY.SetValue(v); }
  static void setBinningVertical(DEVICE*device, const int64_t v) { device->BinningVertical.SetValue(v); }
  static void setBinningHorizontal(DEVICE*device, const int64_t v) { device->BinningHorizontal.SetValue(v); }
  static void setExposureTimeRaw(DEVICE*device, const int64_t v) { device->ExposureTimeRaw.SetValue(v); }
  static void setAveragingNumberOfFrames(DEVICE*device, const int64_t v) { device->AveragingNumberOfFrames.SetValue(v); }
  static void setLineDebouncerTimeRaw(DEVICE*device, const int64_t v) { device->LineDebouncerTimeRaw.SetValue(v); }
  static void setLineStatusAll(DEVICE*device, const int64_t v) { device->LineStatusAll.SetValue(v); }
  static void setUserOutputValueAll(DEVICE*device, const int64_t v) { device->UserOutputValueAll.SetValue(v); }
  static void setUserOutputValueAllMask(DEVICE*device, const int64_t v) { device->UserOutputValueAllMask.SetValue(v); }
  static void setShaftEncoderModuleCounter(DEVICE*device, const int64_t v) { device->ShaftEncoderModuleCounter.SetValue(v); }
  static void setShaftEncoderModuleCounterMax(DEVICE*device, const int64_t v) { device->ShaftEncoderModuleCounterMax.SetValue(v); }
  static void setShaftEncoderModuleReverseCounterMax(DEVICE*device, const int64_t v) { device->ShaftEncoderModuleReverseCounterMax.SetValue(v); }
  static void setTimerDelayRaw(DEVICE*device, const int64_t v) { device->TimerDelayRaw.SetValue(v); }
  static void setTimerDurationRaw(DEVICE*device, const int64_t v) { device->TimerDurationRaw.SetValue(v); }
  static void setTimerSequenceLastEntryIndex(DEVICE*device, const int64_t v) { device->TimerSequenceLastEntryIndex.SetValue(v); }
  static void setTimerSequenceCurrentEntryIndex(DEVICE*device, const int64_t v) { device->TimerSequenceCurrentEntryIndex.SetValue(v); }
  static void setTimerSequenceTimerDelayRaw(DEVICE*device, const int64_t v) { device->TimerSequenceTimerDelayRaw.SetValue(v); }
  static void setTimerSequenceTimerDurationRaw(DEVICE*device, const int64_t v) { device->TimerSequenceTimerDurationRaw.SetValue(v); }
  static void setLUTIndex(DEVICE*device, const int64_t v) { device->LUTIndex.SetValue(v); }
  static void setLUTValue(DEVICE*device, const int64_t v) { device->LUTValue.SetValue(v); }
  static void setAutoTargetValue(DEVICE*device, const int64_t v) { device->AutoTargetValue.SetValue(v); }
  static void setAutoGainRawLowerLimit(DEVICE*device, const int64_t v) { device->AutoGainRawLowerLimit.SetValue(v); }
  static void setAutoGainRawUpperLimit(DEVICE*device, const int64_t v) { device->AutoGainRawUpperLimit.SetValue(v); }
  static void setAutoFunctionAOIWidth(DEVICE*device, const int64_t v) { device->AutoFunctionAOIWidth.SetValue(v); }
  static void setAutoFunctionAOIHeight(DEVICE*device, const int64_t v) { device->AutoFunctionAOIHeight.SetValue(v); }
  static void setAutoFunctionAOIOffsetX(DEVICE*device, const int64_t v) { device->AutoFunctionAOIOffsetX.SetValue(v); }
  static void setAutoFunctionAOIOffsetY(DEVICE*device, const int64_t v) { device->AutoFunctionAOIOffsetY.SetValue(v); }
  static void setUserDefinedValue(DEVICE*device, const int64_t v) { device->UserDefinedValue.SetValue(v); }
  static void setSensorWidth(DEVICE*device, const int64_t v) { device->SensorWidth.SetValue(v); }
  static void setSensorHeight(DEVICE*device, const int64_t v) { device->SensorHeight.SetValue(v); }
  static void setWidthMax(DEVICE*device, const int64_t v) { device->WidthMax.SetValue(v); }
  static void setHeightMax(DEVICE*device, const int64_t v) { device->HeightMax.SetValue(v); }
  static void setExpertFeatureAccessKey(DEVICE*device, const int64_t v) { device->ExpertFeatureAccessKey.SetValue(v); }
  static void setPixelStepCorrectionValueRaw(DEVICE*device, const int64_t v) { device->PixelStepCorrectionValueRaw.SetValue(v); }
  static void setPixelStepCorrectionBusy(DEVICE*device, const int64_t v) { device->PixelStepCorrectionBusy.SetValue(v); }
  static void setChunkStride(DEVICE*device, const int64_t v) { device->ChunkStride.SetValue(v); }
  static void setChunkOffsetX(DEVICE*device, const int64_t v) { device->ChunkOffsetX.SetValue(v); }
  static void setChunkOffsetY(DEVICE*device, const int64_t v) { device->ChunkOffsetY.SetValue(v); }
  static void setChunkWidth(DEVICE*device, const int64_t v) { device->ChunkWidth.SetValue(v); }
  static void setChunkHeight(DEVICE*device, const int64_t v) { device->ChunkHeight.SetValue(v); }
  static void setChunkDynamicRangeMin(DEVICE*device, const int64_t v) { device->ChunkDynamicRangeMin.SetValue(v); }
  static void setChunkDynamicRangeMax(DEVICE*device, const int64_t v) { device->ChunkDynamicRangeMax.SetValue(v); }
  static void setChunkTimestamp(DEVICE*device, const int64_t v) { device->ChunkTimestamp.SetValue(v); }
  static void setChunkFramecounter(DEVICE*device, const int64_t v) { device->ChunkFramecounter.SetValue(v); }
  static void setChunkLineStatusAll(DEVICE*device, const int64_t v) { device->ChunkLineStatusAll.SetValue(v); }
  static void setChunkTriggerinputcounter(DEVICE*device, const int64_t v) { device->ChunkTriggerinputcounter.SetValue(v); }
  static void setChunkLineTriggerIgnoredCounter(DEVICE*device, const int64_t v) { device->ChunkLineTriggerIgnoredCounter.SetValue(v); }
  static void setChunkFrameTriggerIgnoredCounter(DEVICE*device, const int64_t v) { device->ChunkFrameTriggerIgnoredCounter.SetValue(v); }
  static void setChunkFrameTriggerCounter(DEVICE*device, const int64_t v) { device->ChunkFrameTriggerCounter.SetValue(v); }
  static void setChunkFramesPerTriggerCounter(DEVICE*device, const int64_t v) { device->ChunkFramesPerTriggerCounter.SetValue(v); }
  static void setChunkLineTriggerEndToEndCounter(DEVICE*device, const int64_t v) { device->ChunkLineTriggerEndToEndCounter.SetValue(v); }
  static void setChunkPayloadCRC16(DEVICE*device, const int64_t v) { device->ChunkPayloadCRC16.SetValue(v); }
  static void setExposureEndEventStreamChannelIndex(DEVICE*device, const int64_t v) { device->ExposureEndEventStreamChannelIndex.SetValue(v); }
  static void setExposureEndEventFrameID(DEVICE*device, const int64_t v) { device->ExposureEndEventFrameID.SetValue(v); }
  static void setExposureEndEventTimestamp(DEVICE*device, const int64_t v) { device->ExposureEndEventTimestamp.SetValue(v); }
  static void setLineStartOvertriggerEventStreamChannelIndex(DEVICE*device, const int64_t v) { device->LineStartOvertriggerEventStreamChannelIndex.SetValue(v); }
  static void setLineStartOvertriggerEventTimestamp(DEVICE*device, const int64_t v) { device->LineStartOvertriggerEventTimestamp.SetValue(v); }
  static void setFrameStartOvertriggerEventStreamChannelIndex(DEVICE*device, const int64_t v) { device->FrameStartOvertriggerEventStreamChannelIndex.SetValue(v); }
  static void setFrameStartOvertriggerEventTimestamp(DEVICE*device, const int64_t v) { device->FrameStartOvertriggerEventTimestamp.SetValue(v); }
  static void setEventOverrunEventStreamChannelIndex(DEVICE*device, const int64_t v) { device->EventOverrunEventStreamChannelIndex.SetValue(v); }
  static void setEventOverrunEventFrameID(DEVICE*device, const int64_t v) { device->EventOverrunEventFrameID.SetValue(v); }
  static void setEventOverrunEventTimestamp(DEVICE*device, const int64_t v) { device->EventOverrunEventTimestamp.SetValue(v); }
  static void setFileAccessOffset(DEVICE*device, const int64_t v) { device->FileAccessOffset.SetValue(v); }
  static void setFileAccessLength(DEVICE*device, const int64_t v) { device->FileAccessLength.SetValue(v); }
  static void setFileOperationResult(DEVICE*device, const int64_t v) { device->FileOperationResult.SetValue(v); }
  static void setFileSize(DEVICE*device, const int64_t v) { device->FileSize.SetValue(v); }
  static void setPayloadSize(DEVICE*device, const int64_t v) { device->PayloadSize.SetValue(v); }
  static void setGevVersionMajor(DEVICE*device, const int64_t v) { device->GevVersionMajor.SetValue(v); }
  static void setGevVersionMinor(DEVICE*device, const int64_t v) { device->GevVersionMinor.SetValue(v); }
  static void setGevDeviceModeCharacterSet(DEVICE*device, const int64_t v) { device->GevDeviceModeCharacterSet.SetValue(v); }
  static void setGevMACAddress(DEVICE*device, const int64_t v) { device->GevMACAddress.SetValue(v); }
  static void setGevCurrentIPConfiguration(DEVICE*device, const int64_t v) { device->GevCurrentIPConfiguration.SetValue(v); }
  static void setGevCurrentIPAddress(DEVICE*device, const int64_t v) { device->GevCurrentIPAddress.SetValue(v); }
  static void setGevCurrentSubnetMask(DEVICE*device, const int64_t v) { device->GevCurrentSubnetMask.SetValue(v); }
  static void setGevCurrentDefaultGateway(DEVICE*device, const int64_t v) { device->GevCurrentDefaultGateway.SetValue(v); }
  static void setGevPersistentIPAddress(DEVICE*device, const int64_t v) { device->GevPersistentIPAddress.SetValue(v); }
  static void setGevPersistentSubnetMask(DEVICE*device, const int64_t v) { device->GevPersistentSubnetMask.SetValue(v); }
  static void setGevPersistentDefaultGateway(DEVICE*device, const int64_t v) { device->GevPersistentDefaultGateway.SetValue(v); }
  static void setGevLinkSpeed(DEVICE*device, const int64_t v) { device->GevLinkSpeed.SetValue(v); }
  static void setGevNumberOfInterfaces(DEVICE*device, const int64_t v) { device->GevNumberOfInterfaces.SetValue(v); }
  static void setGevMessageChannelCount(DEVICE*device, const int64_t v) { device->GevMessageChannelCount.SetValue(v); }
  static void setGevStreamChannelCount(DEVICE*device, const int64_t v) { device->GevStreamChannelCount.SetValue(v); }
  static void setGevHeartbeatTimeout(DEVICE*device, const int64_t v) { device->GevHeartbeatTimeout.SetValue(v); }
  static void setGevTimestampTickFrequency(DEVICE*device, const int64_t v) { device->GevTimestampTickFrequency.SetValue(v); }
  static void setGevTimestampValue(DEVICE*device, const int64_t v) { device->GevTimestampValue.SetValue(v); }
  static void setGevSCPInterfaceIndex(DEVICE*device, const int64_t v) { device->GevSCPInterfaceIndex.SetValue(v); }
  static void setGevSCDA(DEVICE*device, const int64_t v) { device->GevSCDA.SetValue(v); }
  static void setGevSCPHostPort(DEVICE*device, const int64_t v) { device->GevSCPHostPort.SetValue(v); }
  static void setGevSCPSPacketSize(DEVICE*device, const int64_t v) { device->GevSCPSPacketSize.SetValue(v); }
  static void setGevSCPD(DEVICE*device, const int64_t v) { device->GevSCPD.SetValue(v); }
  static void setGevSCFTD(DEVICE*device, const int64_t v) { device->GevSCFTD.SetValue(v); }
  static void setGevSCBWR(DEVICE*device, const int64_t v) { device->GevSCBWR.SetValue(v); }
  static void setGevSCBWRA(DEVICE*device, const int64_t v) { device->GevSCBWRA.SetValue(v); }
  static void setGevSCBWA(DEVICE*device, const int64_t v) { device->GevSCBWA.SetValue(v); }
  static void setGevSCDMT(DEVICE*device, const int64_t v) { device->GevSCDMT.SetValue(v); }
  static void setGevSCDCT(DEVICE*device, const int64_t v) { device->GevSCDCT.SetValue(v); }
  static void setGevSCFJM(DEVICE*device, const int64_t v) { device->GevSCFJM.SetValue(v); }
  static void setTLParamsLocked(DEVICE*device, const int64_t v) { device->TLParamsLocked.SetValue(v); }


  /* GenApi::IFloat */
  typedef double (*t_getfloat)(DEVICE*device);
  std::map<std::string, t_getfloat>map_getfloat;
  static double getGainAbs(DEVICE*device) { return device->GainAbs.GetValue(); }
  static double getBlackLevelAbs(DEVICE*device) { return device->BlackLevelAbs.GetValue(); }
  static double getBalanceRatioAbs(DEVICE*device) { return device->BalanceRatioAbs.GetValue(); }
  static double getGamma(DEVICE*device) { return device->Gamma.GetValue(); }
  static double getExposureTimeAbs(DEVICE*device) { return device->ExposureTimeAbs.GetValue(); }
  static double getExposureTimeBaseAbs(DEVICE*device) { return device->ExposureTimeBaseAbs.GetValue(); }
  static double getAcquisitionLineRateAbs(DEVICE*device) { return device->AcquisitionLineRateAbs.GetValue(); }
  static double getResultingLineRateAbs(DEVICE*device) { return device->ResultingLineRateAbs.GetValue(); }
  static double getAcquisitionFrameRateAbs(DEVICE*device) { return device->AcquisitionFrameRateAbs.GetValue(); }
  static double getResultingFrameRateAbs(DEVICE*device) { return device->ResultingFrameRateAbs.GetValue(); }
  static double getLineDebouncerTimeAbs(DEVICE*device) { return device->LineDebouncerTimeAbs.GetValue(); }
  static double getTimerDelayTimebaseAbs(DEVICE*device) { return device->TimerDelayTimebaseAbs.GetValue(); }
  static double getTimerDurationTimebaseAbs(DEVICE*device) { return device->TimerDurationTimebaseAbs.GetValue(); }
  static double getTimerDelayAbs(DEVICE*device) { return device->TimerDelayAbs.GetValue(); }
  static double getTimerDurationAbs(DEVICE*device) { return device->TimerDurationAbs.GetValue(); }
  static double getAutoExposureTimeAbsLowerLimit(DEVICE*device) { return device->AutoExposureTimeAbsLowerLimit.GetValue(); }
  static double getAutoExposureTimeAbsUpperLimit(DEVICE*device) { return device->AutoExposureTimeAbsUpperLimit.GetValue(); }
  static double getTemperatureAbs(DEVICE*device) { return device->TemperatureAbs.GetValue(); }
  static double getPixelStepCorrectionValueAbs(DEVICE*device) { return device->PixelStepCorrectionValueAbs.GetValue(); }

  typedef void (*t_setfloat)(DEVICE*device, const double);
  std::map<std::string, t_setfloat>map_setfloat;
  static void setGainAbs(DEVICE*device, const double v) { std::cerr<<__FUNCTION__<<std::endl; device->GainAbs.SetValue(v); }
  static void setBlackLevelAbs(DEVICE*device, const double v) { device->BlackLevelAbs.SetValue(v); }
  static void setBalanceRatioAbs(DEVICE*device, const double v) { device->BalanceRatioAbs.SetValue(v); }
  static void setGamma(DEVICE*device, const double v) { device->Gamma.SetValue(v); }
  static void setExposureTimeAbs(DEVICE*device, const double v) { device->ExposureTimeAbs.SetValue(v); }
  static void setExposureTimeBaseAbs(DEVICE*device, const double v) { device->ExposureTimeBaseAbs.SetValue(v); }
  static void setAcquisitionLineRateAbs(DEVICE*device, const double v) { device->AcquisitionLineRateAbs.SetValue(v); }
  static void setResultingLineRateAbs(DEVICE*device, const double v) { device->ResultingLineRateAbs.SetValue(v); }
  static void setAcquisitionFrameRateAbs(DEVICE*device, const double v) { device->AcquisitionFrameRateAbs.SetValue(v); }
  static void setResultingFrameRateAbs(DEVICE*device, const double v) { device->ResultingFrameRateAbs.SetValue(v); }
  static void setLineDebouncerTimeAbs(DEVICE*device, const double v) { device->LineDebouncerTimeAbs.SetValue(v); }
  static void setTimerDelayTimebaseAbs(DEVICE*device, const double v) { device->TimerDelayTimebaseAbs.SetValue(v); }
  static void setTimerDurationTimebaseAbs(DEVICE*device, const double v) { device->TimerDurationTimebaseAbs.SetValue(v); }
  static void setTimerDelayAbs(DEVICE*device, const double v) { device->TimerDelayAbs.SetValue(v); }
  static void setTimerDurationAbs(DEVICE*device, const double v) { device->TimerDurationAbs.SetValue(v); }
  static void setAutoExposureTimeAbsLowerLimit(DEVICE*device, const double v) { device->AutoExposureTimeAbsLowerLimit.SetValue(v); }
  static void setAutoExposureTimeAbsUpperLimit(DEVICE*device, const double v) { device->AutoExposureTimeAbsUpperLimit.SetValue(v); }
  static void setTemperatureAbs(DEVICE*device, const double v) { device->TemperatureAbs.SetValue(v); }
  static void setPixelStepCorrectionValueAbs(DEVICE*device, const double v) { device->PixelStepCorrectionValueAbs.SetValue(v); }

  /* GenApi::IString */
  typedef GenICam::gcstring (*t_getstring)(DEVICE*device);
  std::map<std::string, t_getstring>map_getstring;
  static GenICam::gcstring getDeviceVendorName(DEVICE*device) { return device->DeviceVendorName.GetValue(); }
  static GenICam::gcstring getDeviceModelName(DEVICE*device) { return device->DeviceModelName.GetValue(); }
  static GenICam::gcstring getDeviceManufacturerInfo(DEVICE*device) { return device->DeviceManufacturerInfo.GetValue(); }
  static GenICam::gcstring getDeviceVersion(DEVICE*device) { return device->DeviceVersion.GetValue(); }
  static GenICam::gcstring getDeviceFirmwareVersion(DEVICE*device) { return device->DeviceFirmwareVersion.GetValue(); }
  static GenICam::gcstring getDeviceID(DEVICE*device) { return device->DeviceID.GetValue(); }
  static GenICam::gcstring getDeviceUserID(DEVICE*device) { return device->DeviceUserID.GetValue(); }
  static GenICam::gcstring getGevFirstURL(DEVICE*device) { return device->GevFirstURL.GetValue(); }
  static GenICam::gcstring getGevSecondURL(DEVICE*device) { return device->GevSecondURL.GetValue(); }

  typedef void (*t_setstring)(DEVICE*device, GenICam::gcstring);
  std::map<std::string, t_setstring>map_setstring;
  static void setDeviceVendorName(DEVICE*device, const GenICam::gcstring v) { device->DeviceVendorName.SetValue(v); }
  static void setDeviceModelName(DEVICE*device, const GenICam::gcstring v) { device->DeviceModelName.SetValue(v); }
  static void setDeviceManufacturerInfo(DEVICE*device, const GenICam::gcstring v) { device->DeviceManufacturerInfo.SetValue(v); }
  static void setDeviceVersion(DEVICE*device, const GenICam::gcstring v) { device->DeviceVersion.SetValue(v); }
  static void setDeviceFirmwareVersion(DEVICE*device, const GenICam::gcstring v) { device->DeviceFirmwareVersion.SetValue(v); }
  static void setDeviceID(DEVICE*device, const GenICam::gcstring v) { device->DeviceID.SetValue(v); }
  static void setDeviceUserID(DEVICE*device, const GenICam::gcstring v) { device->DeviceUserID.SetValue(v); }
  static void setGevFirstURL(DEVICE*device, const GenICam::gcstring v) { device->GevFirstURL.SetValue(v); }
  static void setGevSecondURL(DEVICE*device, const GenICam::gcstring v) { device->GevSecondURL.SetValue(v); }

  /* GenApi::ICommand */
  // only setting(=calling), no getting
  typedef void (*t_setcommand)(DEVICE*device);
  std::map<std::string, t_setcommand>map_setcommand;
  static void setAcquisitionStart(DEVICE*device) { device->AcquisitionStart.Execute(); }
  static void setAcquisitionStop(DEVICE*device) { device->AcquisitionStop.Execute(); }
  static void setAcquisitionAbort(DEVICE*device) { device->AcquisitionAbort.Execute(); }
  static void setTriggerSoftware(DEVICE*device) { device->TriggerSoftware.Execute(); }
  static void setShaftEncoderModuleCounterReset(DEVICE*device) { device->ShaftEncoderModuleCounterReset.Execute(); }
  static void setShaftEncoderModuleReverseCounterReset(DEVICE*device) { device->ShaftEncoderModuleReverseCounterReset.Execute(); }
  static void setUserSetLoad(DEVICE*device) { device->UserSetLoad.Execute(); }
  static void setUserSetSave(DEVICE*device) { device->UserSetSave.Execute(); }
  static void setShadingSetActivate(DEVICE*device) { device->ShadingSetActivate.Execute(); }
  static void setShadingSetCreate(DEVICE*device) { device->ShadingSetCreate.Execute(); }
  static void setDeviceReset(DEVICE*device) { device->DeviceReset.Execute(); }
  static void setSavePixelStepCorrection(DEVICE*device) { device->SavePixelStepCorrection.Execute(); }
  static void setCreatePixelStepCorrection(DEVICE*device) { device->CreatePixelStepCorrection.Execute(); }
  static void setFileOperationExecute(DEVICE*device) { device->FileOperationExecute.Execute(); }
  static void setGevTimestampControlLatch(DEVICE*device) { device->GevTimestampControlLatch.Execute(); }
  static void setGevTimestampControlReset(DEVICE*device) { device->GevTimestampControlReset.Execute(); }
  static void setGevTimestampControlLatchReset(DEVICE*device) { device->GevTimestampControlLatchReset.Execute(); }

  /* GenApi::IRegister */
  // skip this, it's too lowlevel
  
  /* GenApi::IEnumerationT */
  // do this manually!
  std::map<std::string, bool>map_getenum; /* dummy map */
  std::map<std::string, bool>map_setenum; /* dummy map */

};};};

using namespace gem::pylon::cameraproperties;


//typedef t_int *(*t_perfroutine)(t_int *args);

//typedef GenApi::IInteger&(pylprop_int_tf) (Pylon::CBaslerGigECamera::*) (bool);


typedef GenApi::IInteger& bla_t;

static std::map<std::string, GenApi::IInteger&>s_intfun;

void   gem::pylon::cameraproperties::init() {
  static bool initialized=false;  if(initialized){return;} else {initialized=true;}

  map_getbool.clear();
  map_getbool["GammaEnable"]=getGammaEnable;
  map_getbool["ExposureTimeBaseAbsEnable"]=getExposureTimeBaseAbsEnable;
  map_getbool["AcquisitionFrameRateEnable"]=getAcquisitionFrameRateEnable;
  map_getbool["LineInverter"]=getLineInverter;
  map_getbool["LineTermination"]=getLineTermination;
  map_getbool["LineStatus"]=getLineStatus;
  map_getbool["UserOutputValue"]=getUserOutputValue;
  map_getbool["TimerSequenceEnable"]=getTimerSequenceEnable;
  map_getbool["TimerSequenceTimerEnable"]=getTimerSequenceTimerEnable;
  map_getbool["TimerSequenceTimerInverter"]=getTimerSequenceTimerInverter;
  map_getbool["LUTEnable"]=getLUTEnable;
  map_getbool["ShadingEnable"]=getShadingEnable;
  map_getbool["RemoveLimits"]=getRemoveLimits;
  map_getbool["ExpertFeatureEnable"]=getExpertFeatureEnable;
  map_getbool["PixelStepCorrectionEnable"]=getPixelStepCorrectionEnable;
  map_getbool["ChunkModeActive"]=getChunkModeActive;
  map_getbool["ChunkEnable"]=getChunkEnable;
  map_getbool["GevDeviceModeIsBigEndian"]=getGevDeviceModeIsBigEndian;
  map_getbool["GevSupportedIPConfigurationLLA"]=getGevSupportedIPConfigurationLLA;
  map_getbool["GevSupportedIPConfigurationDHCP"]=getGevSupportedIPConfigurationDHCP;
  map_getbool["GevSupportedIPConfigurationPersistentIP"]=getGevSupportedIPConfigurationPersistentIP;
  map_getbool["GevSupportedOptionalCommandsEVENTDATA"]=getGevSupportedOptionalCommandsEVENTDATA;
  map_getbool["GevSupportedOptionalCommandsEVENT"]=getGevSupportedOptionalCommandsEVENT;
  map_getbool["GevSupportedOptionalCommandsPACKETRESEND"]=getGevSupportedOptionalCommandsPACKETRESEND;
  map_getbool["GevSupportedOptionalCommandsWRITEMEM"]=getGevSupportedOptionalCommandsWRITEMEM;
  map_getbool["GevSupportedOptionalCommandsConcatenation"]=getGevSupportedOptionalCommandsConcatenation;


  map_setbool.clear();
  map_setbool["GammaEnable"]=setGammaEnable;
  map_setbool["ExposureTimeBaseAbsEnable"]=setExposureTimeBaseAbsEnable;
  map_setbool["AcquisitionFrameRateEnable"]=setAcquisitionFrameRateEnable;
  map_setbool["LineInverter"]=setLineInverter;
  map_setbool["LineTermination"]=setLineTermination;
  map_setbool["LineStatus"]=setLineStatus;
  map_setbool["UserOutputValue"]=setUserOutputValue;
  map_setbool["TimerSequenceEnable"]=setTimerSequenceEnable;
  map_setbool["TimerSequenceTimerEnable"]=setTimerSequenceTimerEnable;
  map_setbool["TimerSequenceTimerInverter"]=setTimerSequenceTimerInverter;
  map_setbool["LUTEnable"]=setLUTEnable;
  map_setbool["ShadingEnable"]=setShadingEnable;
  map_setbool["RemoveLimits"]=setRemoveLimits;
  map_setbool["ExpertFeatureEnable"]=setExpertFeatureEnable;
  map_setbool["PixelStepCorrectionEnable"]=setPixelStepCorrectionEnable;
  map_setbool["ChunkModeActive"]=setChunkModeActive;
  map_setbool["ChunkEnable"]=setChunkEnable;
  map_setbool["GevDeviceModeIsBigEndian"]=setGevDeviceModeIsBigEndian;
  map_setbool["GevSupportedIPConfigurationLLA"]=setGevSupportedIPConfigurationLLA;
  map_setbool["GevSupportedIPConfigurationDHCP"]=setGevSupportedIPConfigurationDHCP;
  map_setbool["GevSupportedIPConfigurationPersistentIP"]=setGevSupportedIPConfigurationPersistentIP;
  map_setbool["GevSupportedOptionalCommandsEVENTDATA"]=setGevSupportedOptionalCommandsEVENTDATA;
  map_setbool["GevSupportedOptionalCommandsEVENT"]=setGevSupportedOptionalCommandsEVENT;
  map_setbool["GevSupportedOptionalCommandsPACKETRESEND"]=setGevSupportedOptionalCommandsPACKETRESEND;
  map_setbool["GevSupportedOptionalCommandsWRITEMEM"]=setGevSupportedOptionalCommandsWRITEMEM;
  map_setbool["GevSupportedOptionalCommandsConcatenation"]=setGevSupportedOptionalCommandsConcatenation;


  map_getint.clear();
  map_getint["GainRaw"]=getGainRaw;
  map_getint["BlackLevelRaw"]=getBlackLevelRaw;
  map_getint["BalanceRatioRaw"]=getBalanceRatioRaw;
  map_getint["DigitalShift"]=getDigitalShift;
  map_getint["PixelDynamicRangeMin"]=getPixelDynamicRangeMin;
  map_getint["PixelDynamicRangeMax"]=getPixelDynamicRangeMax;
  map_getint["SpatialCorrection"]=getSpatialCorrection;
  map_getint["SpatialCorrectionAmount"]=getSpatialCorrectionAmount;
  map_getint["Width"]=getWidth;
  map_getint["Height"]=getHeight;
  map_getint["OffsetX"]=getOffsetX;
  map_getint["OffsetY"]=getOffsetY;
  map_getint["BinningVertical"]=getBinningVertical;
  map_getint["BinningHorizontal"]=getBinningHorizontal;
  map_getint["ExposureTimeRaw"]=getExposureTimeRaw;
  map_getint["AveragingNumberOfFrames"]=getAveragingNumberOfFrames;
  map_getint["LineDebouncerTimeRaw"]=getLineDebouncerTimeRaw;
  map_getint["LineStatusAll"]=getLineStatusAll;
  map_getint["UserOutputValueAll"]=getUserOutputValueAll;
  map_getint["UserOutputValueAllMask"]=getUserOutputValueAllMask;
  map_getint["ShaftEncoderModuleCounter"]=getShaftEncoderModuleCounter;
  map_getint["ShaftEncoderModuleCounterMax"]=getShaftEncoderModuleCounterMax;
  map_getint["ShaftEncoderModuleReverseCounterMax"]=getShaftEncoderModuleReverseCounterMax;
  map_getint["TimerDelayRaw"]=getTimerDelayRaw;
  map_getint["TimerDurationRaw"]=getTimerDurationRaw;
  map_getint["TimerSequenceLastEntryIndex"]=getTimerSequenceLastEntryIndex;
  map_getint["TimerSequenceCurrentEntryIndex"]=getTimerSequenceCurrentEntryIndex;
  map_getint["TimerSequenceTimerDelayRaw"]=getTimerSequenceTimerDelayRaw;
  map_getint["TimerSequenceTimerDurationRaw"]=getTimerSequenceTimerDurationRaw;
  map_getint["LUTIndex"]=getLUTIndex;
  map_getint["LUTValue"]=getLUTValue;
  map_getint["AutoTargetValue"]=getAutoTargetValue;
  map_getint["AutoGainRawLowerLimit"]=getAutoGainRawLowerLimit;
  map_getint["AutoGainRawUpperLimit"]=getAutoGainRawUpperLimit;
  map_getint["AutoFunctionAOIWidth"]=getAutoFunctionAOIWidth;
  map_getint["AutoFunctionAOIHeight"]=getAutoFunctionAOIHeight;
  map_getint["AutoFunctionAOIOffsetX"]=getAutoFunctionAOIOffsetX;
  map_getint["AutoFunctionAOIOffsetY"]=getAutoFunctionAOIOffsetY;
  map_getint["UserDefinedValue"]=getUserDefinedValue;
  map_getint["SensorWidth"]=getSensorWidth;
  map_getint["SensorHeight"]=getSensorHeight;
  map_getint["WidthMax"]=getWidthMax;
  map_getint["HeightMax"]=getHeightMax;
  map_getint["ExpertFeatureAccessKey"]=getExpertFeatureAccessKey;
  map_getint["PixelStepCorrectionValueRaw"]=getPixelStepCorrectionValueRaw;
  map_getint["PixelStepCorrectionBusy"]=getPixelStepCorrectionBusy;
  map_getint["ChunkStride"]=getChunkStride;
  map_getint["ChunkOffsetX"]=getChunkOffsetX;
  map_getint["ChunkOffsetY"]=getChunkOffsetY;
  map_getint["ChunkWidth"]=getChunkWidth;
  map_getint["ChunkHeight"]=getChunkHeight;
  map_getint["ChunkDynamicRangeMin"]=getChunkDynamicRangeMin;
  map_getint["ChunkDynamicRangeMax"]=getChunkDynamicRangeMax;
  map_getint["ChunkTimestamp"]=getChunkTimestamp;
  map_getint["ChunkFramecounter"]=getChunkFramecounter;
  map_getint["ChunkLineStatusAll"]=getChunkLineStatusAll;
  map_getint["ChunkTriggerinputcounter"]=getChunkTriggerinputcounter;
  map_getint["ChunkLineTriggerIgnoredCounter"]=getChunkLineTriggerIgnoredCounter;
  map_getint["ChunkFrameTriggerIgnoredCounter"]=getChunkFrameTriggerIgnoredCounter;
  map_getint["ChunkFrameTriggerCounter"]=getChunkFrameTriggerCounter;
  map_getint["ChunkFramesPerTriggerCounter"]=getChunkFramesPerTriggerCounter;
  map_getint["ChunkLineTriggerEndToEndCounter"]=getChunkLineTriggerEndToEndCounter;
  map_getint["ChunkPayloadCRC16"]=getChunkPayloadCRC16;
  map_getint["ExposureEndEventStreamChannelIndex"]=getExposureEndEventStreamChannelIndex;
  map_getint["ExposureEndEventFrameID"]=getExposureEndEventFrameID;
  map_getint["ExposureEndEventTimestamp"]=getExposureEndEventTimestamp;
  map_getint["LineStartOvertriggerEventStreamChannelIndex"]=getLineStartOvertriggerEventStreamChannelIndex;
  map_getint["LineStartOvertriggerEventTimestamp"]=getLineStartOvertriggerEventTimestamp;
  map_getint["FrameStartOvertriggerEventStreamChannelIndex"]=getFrameStartOvertriggerEventStreamChannelIndex;
  map_getint["FrameStartOvertriggerEventTimestamp"]=getFrameStartOvertriggerEventTimestamp;
  map_getint["EventOverrunEventStreamChannelIndex"]=getEventOverrunEventStreamChannelIndex;
  map_getint["EventOverrunEventFrameID"]=getEventOverrunEventFrameID;
  map_getint["EventOverrunEventTimestamp"]=getEventOverrunEventTimestamp;
  map_getint["FileAccessOffset"]=getFileAccessOffset;
  map_getint["FileAccessLength"]=getFileAccessLength;
  map_getint["FileOperationResult"]=getFileOperationResult;
  map_getint["FileSize"]=getFileSize;
  map_getint["PayloadSize"]=getPayloadSize;
  map_getint["GevVersionMajor"]=getGevVersionMajor;
  map_getint["GevVersionMinor"]=getGevVersionMinor;
  map_getint["GevDeviceModeCharacterSet"]=getGevDeviceModeCharacterSet;
  map_getint["GevMACAddress"]=getGevMACAddress;
  map_getint["GevCurrentIPConfiguration"]=getGevCurrentIPConfiguration;
  map_getint["GevCurrentIPAddress"]=getGevCurrentIPAddress;
  map_getint["GevCurrentSubnetMask"]=getGevCurrentSubnetMask;
  map_getint["GevCurrentDefaultGateway"]=getGevCurrentDefaultGateway;
  map_getint["GevPersistentIPAddress"]=getGevPersistentIPAddress;
  map_getint["GevPersistentSubnetMask"]=getGevPersistentSubnetMask;
  map_getint["GevPersistentDefaultGateway"]=getGevPersistentDefaultGateway;
  map_getint["GevLinkSpeed"]=getGevLinkSpeed;
  map_getint["GevNumberOfInterfaces"]=getGevNumberOfInterfaces;
  map_getint["GevMessageChannelCount"]=getGevMessageChannelCount;
  map_getint["GevStreamChannelCount"]=getGevStreamChannelCount;
  map_getint["GevHeartbeatTimeout"]=getGevHeartbeatTimeout;
  map_getint["GevTimestampTickFrequency"]=getGevTimestampTickFrequency;
  map_getint["GevTimestampValue"]=getGevTimestampValue;
  map_getint["GevSCPInterfaceIndex"]=getGevSCPInterfaceIndex;
  map_getint["GevSCDA"]=getGevSCDA;
  map_getint["GevSCPHostPort"]=getGevSCPHostPort;
  map_getint["GevSCPSPacketSize"]=getGevSCPSPacketSize;
  map_getint["GevSCPD"]=getGevSCPD;
  map_getint["GevSCFTD"]=getGevSCFTD;
  map_getint["GevSCBWR"]=getGevSCBWR;
  map_getint["GevSCBWRA"]=getGevSCBWRA;
  map_getint["GevSCBWA"]=getGevSCBWA;
  map_getint["GevSCDMT"]=getGevSCDMT;
  map_getint["GevSCDCT"]=getGevSCDCT;
  map_getint["GevSCFJM"]=getGevSCFJM;
  map_getint["TLParamsLocked"]=getTLParamsLocked;

  map_setint.clear();
  map_setint["GainRaw"]=setGainRaw;
  map_setint["BlackLevelRaw"]=setBlackLevelRaw;
  map_setint["BalanceRatioRaw"]=setBalanceRatioRaw;
  map_setint["DigitalShift"]=setDigitalShift;
  map_setint["PixelDynamicRangeMin"]=setPixelDynamicRangeMin;
  map_setint["PixelDynamicRangeMax"]=setPixelDynamicRangeMax;
  map_setint["SpatialCorrection"]=setSpatialCorrection;
  map_setint["SpatialCorrectionAmount"]=setSpatialCorrectionAmount;
  map_setint["Width"]=setWidth;
  map_setint["Height"]=setHeight;
  map_setint["OffsetX"]=setOffsetX;
  map_setint["OffsetY"]=setOffsetY;
  map_setint["BinningVertical"]=setBinningVertical;
  map_setint["BinningHorizontal"]=setBinningHorizontal;
  map_setint["ExposureTimeRaw"]=setExposureTimeRaw;
  map_setint["AveragingNumberOfFrames"]=setAveragingNumberOfFrames;
  map_setint["LineDebouncerTimeRaw"]=setLineDebouncerTimeRaw;
  map_setint["LineStatusAll"]=setLineStatusAll;
  map_setint["UserOutputValueAll"]=setUserOutputValueAll;
  map_setint["UserOutputValueAllMask"]=setUserOutputValueAllMask;
  map_setint["ShaftEncoderModuleCounter"]=setShaftEncoderModuleCounter;
  map_setint["ShaftEncoderModuleCounterMax"]=setShaftEncoderModuleCounterMax;
  map_setint["ShaftEncoderModuleReverseCounterMax"]=setShaftEncoderModuleReverseCounterMax;
  map_setint["TimerDelayRaw"]=setTimerDelayRaw;
  map_setint["TimerDurationRaw"]=setTimerDurationRaw;
  map_setint["TimerSequenceLastEntryIndex"]=setTimerSequenceLastEntryIndex;
  map_setint["TimerSequenceCurrentEntryIndex"]=setTimerSequenceCurrentEntryIndex;
  map_setint["TimerSequenceTimerDelayRaw"]=setTimerSequenceTimerDelayRaw;
  map_setint["TimerSequenceTimerDurationRaw"]=setTimerSequenceTimerDurationRaw;
  map_setint["LUTIndex"]=setLUTIndex;
  map_setint["LUTValue"]=setLUTValue;
  map_setint["AutoTargetValue"]=setAutoTargetValue;
  map_setint["AutoGainRawLowerLimit"]=setAutoGainRawLowerLimit;
  map_setint["AutoGainRawUpperLimit"]=setAutoGainRawUpperLimit;
  map_setint["AutoFunctionAOIWidth"]=setAutoFunctionAOIWidth;
  map_setint["AutoFunctionAOIHeight"]=setAutoFunctionAOIHeight;
  map_setint["AutoFunctionAOIOffsetX"]=setAutoFunctionAOIOffsetX;
  map_setint["AutoFunctionAOIOffsetY"]=setAutoFunctionAOIOffsetY;
  map_setint["UserDefinedValue"]=setUserDefinedValue;
  map_setint["SensorWidth"]=setSensorWidth;
  map_setint["SensorHeight"]=setSensorHeight;
  map_setint["WidthMax"]=setWidthMax;
  map_setint["HeightMax"]=setHeightMax;
  map_setint["ExpertFeatureAccessKey"]=setExpertFeatureAccessKey;
  map_setint["PixelStepCorrectionValueRaw"]=setPixelStepCorrectionValueRaw;
  map_setint["PixelStepCorrectionBusy"]=setPixelStepCorrectionBusy;
  map_setint["ChunkStride"]=setChunkStride;
  map_setint["ChunkOffsetX"]=setChunkOffsetX;
  map_setint["ChunkOffsetY"]=setChunkOffsetY;
  map_setint["ChunkWidth"]=setChunkWidth;
  map_setint["ChunkHeight"]=setChunkHeight;
  map_setint["ChunkDynamicRangeMin"]=setChunkDynamicRangeMin;
  map_setint["ChunkDynamicRangeMax"]=setChunkDynamicRangeMax;
  map_setint["ChunkTimestamp"]=setChunkTimestamp;
  map_setint["ChunkFramecounter"]=setChunkFramecounter;
  map_setint["ChunkLineStatusAll"]=setChunkLineStatusAll;
  map_setint["ChunkTriggerinputcounter"]=setChunkTriggerinputcounter;
  map_setint["ChunkLineTriggerIgnoredCounter"]=setChunkLineTriggerIgnoredCounter;
  map_setint["ChunkFrameTriggerIgnoredCounter"]=setChunkFrameTriggerIgnoredCounter;
  map_setint["ChunkFrameTriggerCounter"]=setChunkFrameTriggerCounter;
  map_setint["ChunkFramesPerTriggerCounter"]=setChunkFramesPerTriggerCounter;
  map_setint["ChunkLineTriggerEndToEndCounter"]=setChunkLineTriggerEndToEndCounter;
  map_setint["ChunkPayloadCRC16"]=setChunkPayloadCRC16;
  map_setint["ExposureEndEventStreamChannelIndex"]=setExposureEndEventStreamChannelIndex;
  map_setint["ExposureEndEventFrameID"]=setExposureEndEventFrameID;
  map_setint["ExposureEndEventTimestamp"]=setExposureEndEventTimestamp;
  map_setint["LineStartOvertriggerEventStreamChannelIndex"]=setLineStartOvertriggerEventStreamChannelIndex;
  map_setint["LineStartOvertriggerEventTimestamp"]=setLineStartOvertriggerEventTimestamp;
  map_setint["FrameStartOvertriggerEventStreamChannelIndex"]=setFrameStartOvertriggerEventStreamChannelIndex;
  map_setint["FrameStartOvertriggerEventTimestamp"]=setFrameStartOvertriggerEventTimestamp;
  map_setint["EventOverrunEventStreamChannelIndex"]=setEventOverrunEventStreamChannelIndex;
  map_setint["EventOverrunEventFrameID"]=setEventOverrunEventFrameID;
  map_setint["EventOverrunEventTimestamp"]=setEventOverrunEventTimestamp;
  map_setint["FileAccessOffset"]=setFileAccessOffset;
  map_setint["FileAccessLength"]=setFileAccessLength;
  map_setint["FileOperationResult"]=setFileOperationResult;
  map_setint["FileSize"]=setFileSize;
  map_setint["PayloadSize"]=setPayloadSize;
  map_setint["GevVersionMajor"]=setGevVersionMajor;
  map_setint["GevVersionMinor"]=setGevVersionMinor;
  map_setint["GevDeviceModeCharacterSet"]=setGevDeviceModeCharacterSet;
  map_setint["GevMACAddress"]=setGevMACAddress;
  map_setint["GevCurrentIPConfiguration"]=setGevCurrentIPConfiguration;
  map_setint["GevCurrentIPAddress"]=setGevCurrentIPAddress;
  map_setint["GevCurrentSubnetMask"]=setGevCurrentSubnetMask;
  map_setint["GevCurrentDefaultGateway"]=setGevCurrentDefaultGateway;
  map_setint["GevPersistentIPAddress"]=setGevPersistentIPAddress;
  map_setint["GevPersistentSubnetMask"]=setGevPersistentSubnetMask;
  map_setint["GevPersistentDefaultGateway"]=setGevPersistentDefaultGateway;
  map_setint["GevLinkSpeed"]=setGevLinkSpeed;
  map_setint["GevNumberOfInterfaces"]=setGevNumberOfInterfaces;
  map_setint["GevMessageChannelCount"]=setGevMessageChannelCount;
  map_setint["GevStreamChannelCount"]=setGevStreamChannelCount;
  map_setint["GevHeartbeatTimeout"]=setGevHeartbeatTimeout;
  map_setint["GevTimestampTickFrequency"]=setGevTimestampTickFrequency;
  map_setint["GevTimestampValue"]=setGevTimestampValue;
  map_setint["GevSCPInterfaceIndex"]=setGevSCPInterfaceIndex;
  map_setint["GevSCDA"]=setGevSCDA;
  map_setint["GevSCPHostPort"]=setGevSCPHostPort;
  map_setint["GevSCPSPacketSize"]=setGevSCPSPacketSize;
  map_setint["GevSCPD"]=setGevSCPD;
  map_setint["GevSCFTD"]=setGevSCFTD;
  map_setint["GevSCBWR"]=setGevSCBWR;
  map_setint["GevSCBWRA"]=setGevSCBWRA;
  map_setint["GevSCBWA"]=setGevSCBWA;
  map_setint["GevSCDMT"]=setGevSCDMT;
  map_setint["GevSCDCT"]=setGevSCDCT;
  map_setint["GevSCFJM"]=setGevSCFJM;
  map_setint["TLParamsLocked"]=setTLParamsLocked;


  map_getfloat.clear();
  map_getfloat["GainAbs"]=getGainAbs;
  map_getfloat["BlackLevelAbs"]=getBlackLevelAbs;
  map_getfloat["BalanceRatioAbs"]=getBalanceRatioAbs;
  map_getfloat["Gamma"]=getGamma;
  map_getfloat["ExposureTimeAbs"]=getExposureTimeAbs;
  map_getfloat["ExposureTimeBaseAbs"]=getExposureTimeBaseAbs;
  map_getfloat["AcquisitionLineRateAbs"]=getAcquisitionLineRateAbs;
  map_getfloat["ResultingLineRateAbs"]=getResultingLineRateAbs;
  map_getfloat["AcquisitionFrameRateAbs"]=getAcquisitionFrameRateAbs;
  map_getfloat["ResultingFrameRateAbs"]=getResultingFrameRateAbs;
  map_getfloat["LineDebouncerTimeAbs"]=getLineDebouncerTimeAbs;
  map_getfloat["TimerDelayTimebaseAbs"]=getTimerDelayTimebaseAbs;
  map_getfloat["TimerDurationTimebaseAbs"]=getTimerDurationTimebaseAbs;
  map_getfloat["TimerDelayAbs"]=getTimerDelayAbs;
  map_getfloat["TimerDurationAbs"]=getTimerDurationAbs;
  map_getfloat["AutoExposureTimeAbsLowerLimit"]=getAutoExposureTimeAbsLowerLimit;
  map_getfloat["AutoExposureTimeAbsUpperLimit"]=getAutoExposureTimeAbsUpperLimit;
  map_getfloat["TemperatureAbs"]=getTemperatureAbs;
  map_getfloat["PixelStepCorrectionValueAbs"]=getPixelStepCorrectionValueAbs;

  map_setfloat.clear();
  map_setfloat["GainAbs"]=setGainAbs;
  map_setfloat["BlackLevelAbs"]=setBlackLevelAbs;
  map_setfloat["BalanceRatioAbs"]=setBalanceRatioAbs;
  map_setfloat["Gamma"]=setGamma;
  map_setfloat["ExposureTimeAbs"]=setExposureTimeAbs;
  map_setfloat["ExposureTimeBaseAbs"]=setExposureTimeBaseAbs;
  map_setfloat["AcquisitionLineRateAbs"]=setAcquisitionLineRateAbs;
  map_setfloat["ResultingLineRateAbs"]=setResultingLineRateAbs;
  map_setfloat["AcquisitionFrameRateAbs"]=setAcquisitionFrameRateAbs;
  map_setfloat["ResultingFrameRateAbs"]=setResultingFrameRateAbs;
  map_setfloat["LineDebouncerTimeAbs"]=setLineDebouncerTimeAbs;
  map_setfloat["TimerDelayTimebaseAbs"]=setTimerDelayTimebaseAbs;
  map_setfloat["TimerDurationTimebaseAbs"]=setTimerDurationTimebaseAbs;
  map_setfloat["TimerDelayAbs"]=setTimerDelayAbs;
  map_setfloat["TimerDurationAbs"]=setTimerDurationAbs;
  map_setfloat["AutoExposureTimeAbsLowerLimit"]=setAutoExposureTimeAbsLowerLimit;
  map_setfloat["AutoExposureTimeAbsUpperLimit"]=setAutoExposureTimeAbsUpperLimit;
  map_setfloat["TemperatureAbs"]=setTemperatureAbs;
  map_setfloat["PixelStepCorrectionValueAbs"]=setPixelStepCorrectionValueAbs;

  map_getstring.clear();
  map_getstring["DeviceVendorName"]=getDeviceVendorName;
  map_getstring["DeviceModelName"]=getDeviceModelName;
  map_getstring["DeviceManufacturerInfo"]=getDeviceManufacturerInfo;
  map_getstring["DeviceVersion"]=getDeviceVersion;
  map_getstring["DeviceFirmwareVersion"]=getDeviceFirmwareVersion;
  map_getstring["DeviceID"]=getDeviceID;
  map_getstring["DeviceUserID"]=getDeviceUserID;
  map_getstring["GevFirstURL"]=getGevFirstURL;
  map_getstring["GevSecondURL"]=getGevSecondURL;

  map_setstring.clear();
  map_setstring["DeviceVendorName"]=setDeviceVendorName;
  map_setstring["DeviceModelName"]=setDeviceModelName;
  map_setstring["DeviceManufacturerInfo"]=setDeviceManufacturerInfo;
  map_setstring["DeviceVersion"]=setDeviceVersion;
  map_setstring["DeviceFirmwareVersion"]=setDeviceFirmwareVersion;
  map_setstring["DeviceID"]=setDeviceID;
  map_setstring["DeviceUserID"]=setDeviceUserID;
  map_setstring["GevFirstURL"]=setGevFirstURL;
  map_setstring["GevSecondURL"]=setGevSecondURL;

  map_setcommand.clear();
  map_setcommand["AcquisitionStart"]=setAcquisitionStart;
  map_setcommand["AcquisitionStop"]=setAcquisitionStop;
  map_setcommand["AcquisitionAbort"]=setAcquisitionAbort;
  map_setcommand["TriggerSoftware"]=setTriggerSoftware;
  map_setcommand["ShaftEncoderModuleCounterReset"]=setShaftEncoderModuleCounterReset;
  map_setcommand["ShaftEncoderModuleReverseCounterReset"]=setShaftEncoderModuleReverseCounterReset;
  map_setcommand["UserSetLoad"]=setUserSetLoad;
  map_setcommand["UserSetSave"]=setUserSetSave;
  map_setcommand["ShadingSetActivate"]=setShadingSetActivate;
  map_setcommand["ShadingSetCreate"]=setShadingSetCreate;
  map_setcommand["DeviceReset"]=setDeviceReset;
  map_setcommand["SavePixelStepCorrection"]=setSavePixelStepCorrection;
  map_setcommand["CreatePixelStepCorrection"]=setCreatePixelStepCorrection;
  map_setcommand["FileOperationExecute"]=setFileOperationExecute;
  map_setcommand["GevTimestampControlLatch"]=setGevTimestampControlLatch;
  map_setcommand["GevTimestampControlReset"]=setGevTimestampControlReset;
  map_setcommand["GevTimestampControlLatchReset"]=setGevTimestampControlLatchReset;


  map_getenum.clear();
  map_getenum["AcquisitionMode"]=true;
  map_getenum["AutoFunctionAOISelector"]=true;                                                  
  map_getenum["BalanceRatioSelector"]=true;                                                     
  map_getenum["BalanceWhiteAuto"]=true;                                                         
  map_getenum["BlackLevelSelector"]=true;                                                       
  map_getenum["ChunkPixelFormat"]=true;
  map_getenum["ChunkSelector"]=true;                                                            
  map_getenum["DeviceScanType"]=true;                                                           
  map_getenum["EventNotification"]=true;                                                        
  map_getenum["EventSelector"]=true;                                                            
  map_getenum["ExpertFeatureAccessSelector"]=true;
  map_getenum["ExposureAuto"]=true;                                                             
  map_getenum["ExposureMode"]=true;                                                             
  map_getenum["FileOpenMode"]=true;                                                             
  map_getenum["FileOperationSelector"]=true;                                                    
  map_getenum["FileOperationStatus"]=true;
  map_getenum["FileSelector"]=true;                                                             
  map_getenum["GainAuto"]=true;                                                                 
  map_getenum["GainSelector"]=true;                                                             
  map_getenum["GevCCP"]=true;
  map_getenum["GevInterfaceSelector"]=true;                                                     
  map_getenum["GevStreamChannelSelector"]=true;                                                 
  map_getenum["LegacyBinningVertical"]=true;                                                    
  map_getenum["LineFormat"]=true;                                                               
  map_getenum["LineMode"]=true;                                                                 
  map_getenum["LineSelector"]=true;
  map_getenum["LineSource"]=true;                                                               
  map_getenum["LUTSelector"]=true;                                                              
  map_getenum["ParameterSelector"]=true;
  map_getenum["PixelCoding"]=true;                                                              
  map_getenum["PixelColorFilter"]=true;
  map_getenum["PixelFormat"]=true;                                                              
  map_getenum["PixelSize"]=true;                                                                
  map_getenum["PixelStepCorrectionSelector"]=true;
  map_getenum["ShadingSelector"]=true;
  map_getenum["ShadingSetDefaultSelector"]=true;
  map_getenum["ShadingSetSelector"]=true;
  map_getenum["ShadingStatus"]=true;
  map_getenum["ShaftEncoderModuleCounterMode"]=true;
  map_getenum["ShaftEncoderModuleLineSelector"]=true;
  map_getenum["ShaftEncoderModuleLineSource"]=true;
  map_getenum["ShaftEncoderModuleMode"]=true;
  map_getenum["SpatialCorrectionStartingLine"]=true;
  map_getenum["TemperatureSelector"]=true;
  map_getenum["TestImageSelector"]=true;
  map_getenum["TimerSelector"]=true;
  map_getenum["TimerSequenceEntrySelector"]=true;
  map_getenum["TimerSequenceTimerSelector"]=true;
  map_getenum["TimerTriggerActivation"]=true;
  map_getenum["TimerTriggerSource"]=true;
  map_getenum["TriggerActivation"]=true;
  map_getenum["TriggerMode"]=true;
  map_getenum["TriggerSelector"]=true;
  map_getenum["TriggerSource"]=true;
  map_getenum["UserDefinedValueSelector"]=true;
  map_getenum["UserOutputSelector"]=true;
  map_getenum["UserSetDefaultSelector"]=true;
  map_getenum["UserSetSelector"]=true;

  map_setenum.clear();
  map_setenum["AcquisitionMode"]=true;
  map_setenum["AutoFunctionAOISelector"]=true;
  map_setenum["BalanceRatioSelector"]=true;
  map_setenum["BalanceWhiteAuto"]=true;
  map_setenum["BlackLevelSelector"]=true;
  map_setenum["ChunkPixelFormat"]=true;
  map_setenum["ChunkSelector"]=true;
  map_setenum["DeviceScanType"]=true;
  map_setenum["EventNotification"]=true;
  map_setenum["EventSelector"]=true;
  map_setenum["ExpertFeatureAccessSelector"]=true;
  map_setenum["ExposureAuto"]=true;
  map_setenum["ExposureMode"]=true;
  map_setenum["FileOpenMode"]=true;
  map_setenum["FileOperationSelector"]=true;
  map_setenum["FileOperationStatus"]=true;
  map_setenum["FileSelector"]=true;
  map_setenum["GainAuto"]=true;
  map_setenum["GainSelector"]=true;
  map_setenum["GevCCP"]=true;
  map_setenum["GevInterfaceSelector"]=true;
  map_setenum["GevStreamChannelSelector"]=true;
  map_setenum["LegacyBinningVertical"]=true;
  map_setenum["LineFormat"]=true;
  map_setenum["LineMode"]=true;
  map_setenum["LineSelector"]=true;
  map_setenum["LineSource"]=true;
  map_setenum["LUTSelector"]=true;
  map_setenum["ParameterSelector"]=true;
  map_setenum["PixelCoding"]=true;
  map_setenum["PixelColorFilter"]=true;
  map_setenum["PixelFormat"]=true;
  map_setenum["PixelSize"]=true;
  map_setenum["PixelStepCorrectionSelector"]=true;
  map_setenum["ShadingSelector"]=true;
  map_setenum["ShadingSetDefaultSelector"]=true;
  map_setenum["ShadingSetSelector"]=true;
  map_setenum["ShadingStatus"]=true;
  map_setenum["ShaftEncoderModuleCounterMode"]=true;
  map_setenum["ShaftEncoderModuleLineSelector"]=true;
  map_setenum["ShaftEncoderModuleLineSource"]=true;
  map_setenum["ShaftEncoderModuleMode"]=true;
  map_setenum["SpatialCorrectionStartingLine"]=true;
  map_setenum["TemperatureSelector"]=true;
  map_setenum["TestImageSelector"]=true;
  map_setenum["TimerSelector"]=true;
  map_setenum["TimerSequenceEntrySelector"]=true;
  map_setenum["TimerSequenceTimerSelector"]=true;
  map_setenum["TimerTriggerActivation"]=true;
  map_setenum["TimerTriggerSource"]=true;
  map_setenum["TriggerActivation"]=true;
  map_setenum["TriggerMode"]=true;
  map_setenum["TriggerSelector"]=true;
  map_setenum["TriggerSource"]=true;
  map_setenum["UserDefinedValueSelector"]=true;
  map_setenum["UserOutputSelector"]=true;
  map_setenum["UserSetDefaultSelector"]=true;
  map_setenum["UserSetSelector"]=true;


}
gem::Properties&gem::pylon::cameraproperties::getKeys(void) {
  gem::Properties&result=readprops; result.clear();
  gem::pylon::cameraproperties::init();

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
    gem::any typevalue=0;
    std::map<std::string, t_getfloat>::iterator it;
    for(it=map_getfloat.begin(); it!=map_getfloat.end(); ++it) {
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

#if 0
  do {
    gem::any typevalue;
    std::map<std::command, t_getcommand>::iterator it;
    for(it=map_getcommand.begin(); it!=map_getcommand.end(); ++it) {
      result.set(it->first, typevalue);
    }
  } while(0);
#endif

  do {
    gem::any typevalue=0;
    std::map<std::string, bool>::iterator it;
    for(it=map_getenum.begin(); it!=map_getenum.end(); ++it) {
      result.set(it->first, typevalue);
    }
  } while(0);


  return result;
}
gem::Properties&gem::pylon::cameraproperties::setKeys(void) {
  gem::Properties&result=writeprops; result.clear();
  gem::pylon::cameraproperties::init();

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
    gem::any typevalue=0;
    std::map<std::string, t_setfloat>::iterator it;
    for(it=map_setfloat.begin(); it!=map_setfloat.end(); ++it) {
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

  do {
    gem::any typevalue;
    std::map<std::string, t_setcommand>::iterator it;
    for(it=map_setcommand.begin(); it!=map_setcommand.end(); ++it) {
      result.set(it->first, typevalue);
    }
  } while(0);

  do {
    gem::any typevalue=0;
    std::map<std::string, bool>::iterator it;
    for(it=map_setenum.begin(); it!=map_setenum.end(); ++it) {
      result.set(it->first, typevalue);
    }
  } while(0);

  return result;
}

void gem::pylon::cameraproperties::get(Pylon::CBaslerGigECamera*device, 
                                std::string key,
                                gem::any&result)
{
  gem::pylon::cameraproperties::init();

  /* compatibility with other backends */
  if (key=="width") { /* for compat with other backends */
    result=static_cast<double>(device->Width.GetValue());
    return;

  } else if (key=="height") { /* for compat with other backends */
    result=static_cast<double>(device->Height.GetValue());
    return;
  }


  do {
    std::map<std::string, t_getbool>::iterator it=map_getbool.find(key);
    if(it!=map_getbool.end()) {
      result=static_cast<double>(it->second(device));
    }
  } while(0);
  do {
    std::map<std::string, t_getint>::iterator it=map_getint.find(key);
    if(it!=map_getint.end()) {
      result=static_cast<double>(it->second(device));
    }
  } while(0);
  do {
    std::map<std::string, t_getfloat>::iterator it=map_getfloat.find(key);
    if(it!=map_getfloat.end()) {
      result=static_cast<double>(it->second(device));
    }
  } while(0);
  do {
    std::map<std::string, t_getstring>::iterator it=map_getstring.find(key);
    if(it!=map_getstring.end()) {
      result=std::string((it->second(device)).c_str());
    }
  } while(0);


  do {
    std::map<std::string, bool>::iterator it=map_getenum.find(key);
    if(it!=map_getenum.end()) {
      /* enumerations */
  if(0) {;
  } else if (key=="GainAuto") {
    result=static_cast<double>(device->GainAuto.GetValue()) /* enumeration */;
    return;

  } else if (key=="GainSelector") {
    result=static_cast<double>(device->GainSelector.GetValue()) /* enumeration */;
    return;

  } else if (key=="BlackLevelSelector") {
    result=static_cast<double>(device->BlackLevelSelector.GetValue()) /* enumeration */;
    return;

  } else if (key=="BalanceWhiteAuto") {
    result=static_cast<double>(device->BalanceWhiteAuto.GetValue()) /* enumeration */;
    return;

  } else if (key=="BalanceRatioSelector") {
    result=static_cast<double>(device->BalanceRatioSelector.GetValue()) /* enumeration */;
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

  } else if (key=="SpatialCorrectionStartingLine") {
    result=static_cast<double>(device->SpatialCorrectionStartingLine.GetValue()) /* enumeration */;
    return;

  } else if (key=="TestImageSelector") {
    result=static_cast<double>(device->TestImageSelector.GetValue()) /* enumeration */;
    return;

  } else if (key=="LegacyBinningVertical") {
    result=static_cast<double>(device->LegacyBinningVertical.GetValue()) /* enumeration */;
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

  } else if (key=="UserOutputSelector") {
    result=static_cast<double>(device->UserOutputSelector.GetValue()) /* enumeration */;
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

  } else if (key=="TimerSelector") {
    result=static_cast<double>(device->TimerSelector.GetValue()) /* enumeration */;
    return;

  } else if (key=="TimerTriggerSource") {
    result=static_cast<double>(device->TimerTriggerSource.GetValue()) /* enumeration */;
    return;

  } else if (key=="TimerTriggerActivation") {
    result=static_cast<double>(device->TimerTriggerActivation.GetValue()) /* enumeration */;
    return;

  } else if (key=="TimerSequenceEntrySelector") {
    result=static_cast<double>(device->TimerSequenceEntrySelector.GetValue()) /* enumeration */;
    return;

  } else if (key=="TimerSequenceTimerSelector") {
    result=static_cast<double>(device->TimerSequenceTimerSelector.GetValue()) /* enumeration */;
    return;

  } else if (key=="LUTSelector") {
    result=static_cast<double>(device->LUTSelector.GetValue()) /* enumeration */;
    return;

  } else if (key=="UserSetSelector") {
    result=static_cast<double>(device->UserSetSelector.GetValue()) /* enumeration */;
    return;

  } else if (key=="UserSetDefaultSelector") {
    result=static_cast<double>(device->UserSetDefaultSelector.GetValue()) /* enumeration */;
    return;

  } else if (key=="AutoFunctionAOISelector") {
    result=static_cast<double>(device->AutoFunctionAOISelector.GetValue()) /* enumeration */;
    return;

  } else if (key=="ShadingSelector") {
    result=static_cast<double>(device->ShadingSelector.GetValue()) /* enumeration */;
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

  } else if (key=="DeviceScanType") {
    result=static_cast<double>(device->DeviceScanType.GetValue()) /* enumeration */;
    return;

  } else if (key=="TemperatureSelector") {
    result=static_cast<double>(device->TemperatureSelector.GetValue()) /* enumeration */;
    return;

  } else if (key=="ParameterSelector") {
    result=static_cast<double>(device->ParameterSelector.GetValue()) /* enumeration */;
    return;

  } else if (key=="ExpertFeatureAccessSelector") {
    result=static_cast<double>(device->ExpertFeatureAccessSelector.GetValue()) /* enumeration */;
    return;

  } else if (key=="PixelStepCorrectionSelector") {
    result=static_cast<double>(device->PixelStepCorrectionSelector.GetValue()) /* enumeration */;
    return;

  } else if (key=="ChunkSelector") {
    result=static_cast<double>(device->ChunkSelector.GetValue()) /* enumeration */;
    return;

  } else if (key=="ChunkPixelFormat") {
    result=static_cast<double>(device->ChunkPixelFormat.GetValue()) /* enumeration */;
    return;

  } else if (key=="EventSelector") {
    result=static_cast<double>(device->EventSelector.GetValue()) /* enumeration */;
    return;

  } else if (key=="EventNotification") {
    result=static_cast<double>(device->EventNotification.GetValue()) /* enumeration */;
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

  } else if (key=="FileOperationStatus") {
    result=static_cast<double>(device->FileOperationStatus.GetValue()) /* enumeration */;
    return;

  } else if (key=="GevInterfaceSelector") {
    result=static_cast<double>(device->GevInterfaceSelector.GetValue()) /* enumeration */;
    return;

  } else if (key=="GevCCP") {
    result=static_cast<double>(device->GevCCP.GetValue()) /* enumeration */;
    return;

  } else if (key=="GevStreamChannelSelector") {
    result=static_cast<double>(device->GevStreamChannelSelector.GetValue()) /* enumeration */;
    return;
  }
  }
  } while(0);
}


bool gem::pylon::cameraproperties::set(Pylon::CBaslerGigECamera*device, 
                                       std::string key,
                                       gem::Properties&props)
{
  gem::pylon::cameraproperties::init();
  double d;
  std::string s;

  /* compatibility with other APIs */
  if (key=="width") {
    if(props.get(key, d)) {
      device->Width.SetValue(d);
    }
    return true;
  } else if (key=="height") {
    if(props.get(key, d)) {
      device->Height.SetValue(d);
    }
    return true;
  }


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
  std::map<std::string, t_setfloat>::iterator it_f=map_setfloat.find(key);
  if(it_f != map_setfloat.end()) {
    if(props.get(key, d)) {
      it_f->second(device, static_cast<double>(d));
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
  std::map<std::string, t_setcommand>::iterator it_c=map_setcommand.find(key);
  if(it_c != map_setcommand.end()) {
    it_c->second(device);
    props.erase(key);
  }

  do {
    std::map<std::string, bool>::iterator it=map_setenum.find(key);
    if(it!=map_getenum.end()) {
      /* enumerations */
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
  }
    }
  } while(false);

  return false;
}
#endif /* HAVE_PYLON */
