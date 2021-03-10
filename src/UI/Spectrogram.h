#pragma once

#include "Component.h"

class Bitmap;
class Palette;

class Spectrogram : public Component
{
private:
  Palette *m_palette;

  Bitmap *bitmap;

public:
  Spectrogram(Palette *palette, int x, int y, int width, int height);
  void update(float *magnitudes);
  void _draw(M5Display &display);
};
