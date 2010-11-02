// set StreamGrabber attributes

static bool CameraPropertiesSet(Pylon::CBaslerGigECamera*device, 
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
