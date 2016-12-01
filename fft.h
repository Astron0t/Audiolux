#pragma once
#include "input.h"
#include <Audio.h>

namespace pixelmap {

class FFTInput : public Input {
  public:
    FFTInput(AudioAnalyzeFFT1024* fft);
    void update();
    float getInput(int index = 0);
  private:
    AudioAnalyzeFFT1024* fft_;
    uint16_t max_[512] = {0};
    float amp_;
    float freq_;
};

} // end namespace pixelmap
