#pragma once

#include "Component.h"

class Waveform : public Component
{
private:
  float *m_samples;
  int m_num_samples;

public:
  Waveform(M5Display &display, int x, int y, int width, int height, int num_samples);
  void update(const float *samples);
  void _draw(M5Display &display);
};
