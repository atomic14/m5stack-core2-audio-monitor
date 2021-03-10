#include <Arduino.h>
#include <M5Core2.h>
#include "Waveform.h"

Waveform::Waveform(M5Display &display, int x, int y, int width, int height, int num_samples) : Component(x, y, width, height)
{
  m_num_samples = num_samples;
  m_samples = static_cast<float *>(ps_malloc(sizeof(float) * num_samples));
}

void Waveform::update(const float *samples)
{
  memcpy(m_samples, samples, sizeof(float) * m_num_samples);
}

void Waveform::_draw(M5Display &display)
{
  float x = 0;
  float x_step = (float)width / (float)m_num_samples;
  display.fillRect(x, y, width, height, 0);
  for (int i = 4; i < m_num_samples; i += 4)
  {
    display.drawLine(x, 60 + m_samples[i - 4] * 3, x + x_step * 4, 60 + m_samples[i] * 3, 0xfff);
    x += x_step * 4;
  }
  // m_sprite->pushSprite(0, 0);
}