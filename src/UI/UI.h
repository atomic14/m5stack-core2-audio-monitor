#pragma once

class Palette;
class Waveform;
class GraphicEqualiser;
class Spectrogram;
class M5Display;

class UI
{
private:
  Palette *m_palette;
  Waveform *m_waveform;
  GraphicEqualiser *m_graphic_equaliser;
  Spectrogram *m_spectrogram;
  M5Display &m_display;
  TaskHandle_t m_draw_task_handle;

public:
  UI(M5Display &display, int window_size);
  void toggle_display();
  void update(float *samples, float *fft);
  void draw();
  friend void drawing_task(void *param);
};
