#include "Processor.h"
#include "HammingWindow.h"
#include "tools/kiss_fftr.h"

Processor::Processor(int window_size)
{
  m_window_size = window_size;
  // work out the FFT size
  m_fft_size = 1;
  while (m_fft_size < window_size)
  {
    m_fft_size *= 2;
  }
  // create the hamming window to apply to the samples
  m_hamming_window = new HammingWindow(m_fft_size);
  // initialise kiss fftr
  m_cfg = kiss_fftr_alloc(m_fft_size, false, 0, 0);

  m_fft_input = static_cast<float *>(malloc(sizeof(float) * m_fft_size));
  for (int i = 0; i < m_fft_size; i++)
  {
    m_fft_input[i] = 0;
  }
  int energy_size = m_fft_size / 2;
  m_fft_output = static_cast<kiss_fft_cpx *>(malloc(sizeof(kiss_fft_cpx) * energy_size));

  m_energy = static_cast<float *>(malloc(sizeof(float) * window_size / 4));
}

void Processor::update(int16_t *samples)
{
  int offset = (m_fft_size - m_window_size) / 2;
  for (int i = 0; i < m_window_size; i++)
  {
    m_fft_input[offset + i] = samples[i] / 30.0f;
  }
  // apply the hamming window
  m_hamming_window->applyWindow(m_fft_input);
  // do the fft
  kiss_fftr(
      m_cfg,
      m_fft_input,
      reinterpret_cast<kiss_fft_cpx *>(m_fft_output));

  for (int i = 0; i < m_window_size / 4; i++)
  {
    const float real = m_fft_output[i].r;
    const float imag = m_fft_output[i].i;
    m_energy[i] = sqrt((real * real) + (imag * imag));
  }
}