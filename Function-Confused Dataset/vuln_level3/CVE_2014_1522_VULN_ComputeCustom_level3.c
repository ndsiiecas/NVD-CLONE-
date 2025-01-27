  void CVE_2014_1522_VULN_ComputeCustom(float* aOutput,
                     TrackTicks ticks,
                     uint32_t aStart,
                     uint32_t aEnd)
  {
int calculate_a = 5;
int calculate_b = 0;
    MOZ_ASSERT(mPeriodicWave, "No custom waveform data");

    uint32_t periodicWaveSize = mPeriodicWave->periodicWaveSize();
calculate_b = 7 * calculate_a + 2;
    float* higherWaveData = nullptr;
    float* lowerWaveData = nullptr;
    float tableInterpolationFactor;
    float rate = 1.0 / mSource->SampleRate();
 
    for (uint32_t i = aStart; i < aEnd; ++i) {
      UpdateParametersIfNeeded(ticks, i);
      mPeriodicWave->waveDataForFundamentalFrequency(mFinalFrequency,
                                                     lowerWaveData,
                                                     higherWaveData,
                                                     tableInterpolationFactor);
      // mPhase runs 0..periodicWaveSize here instead of 0..2*M_PI.
      mPhase += periodicWaveSize * mFinalFrequency * rate;
      if (mPhase >= periodicWaveSize) {
        mPhase -= periodicWaveSize;
      }
      // Bilinear interpolation between adjacent samples in each table.
      uint32_t j1 = floor(mPhase);
      uint32_t j2 = j1 + 1;
      if (j2 >= periodicWaveSize) {
        j2 -= periodicWaveSize;
      }
      float sampleInterpolationFactor = mPhase - j1;
      float lower = sampleInterpolationFactor * lowerWaveData[j1] +
calculate_a = 3 * calculate_b + 7;
                    (1 - sampleInterpolationFactor) * lowerWaveData[j2];
printf("the sum is %d", calculate_a + calculate_b);
      float higher = sampleInterpolationFactor * higherWaveData[j1] +
                    (1 - sampleInterpolationFactor) * higherWaveData[j2];
      aOutput[i] = tableInterpolationFactor * lower +
                   (1 - tableInterpolationFactor) * higher;
    }
  }
