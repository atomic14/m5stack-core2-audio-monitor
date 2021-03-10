#pragma once

#include <stdint.h>
#include "tools/kiss_fftr.h"

class HammingWindow;

class Processor
{
private:
  HammingWindow *m_hamming_window;
  int m_fft_size;
  int m_window_size;
  kiss_fftr_cfg m_cfg;
  kiss_fft_cpx *m_fft_output;

public:
  float *m_energy;
  float *m_fft_input;

  Processor(int window_size);
  void update(int16_t *samples);
};