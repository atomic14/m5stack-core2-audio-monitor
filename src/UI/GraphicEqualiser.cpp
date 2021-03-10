#include <Arduino.h>
#include <algorithm>
#include <M5Core2.h>
#include "Palette.h"
#include "GraphicEqualiser.h"

#undef min

GraphicEqualiser::GraphicEqualiser(Palette *palette, int x, int y, int width, int height, int num_bins) : Component(x, y, width, height)
{
  m_palette = palette;
  m_num_bins = num_bins;
  bar_chart = static_cast<float *>(malloc(sizeof(float) * num_bins));
  for (int i = 0; i < num_bins; i++)
  {
    bar_chart[i] = 0.0f;
  }
  bar_chart_peaks = static_cast<float *>(malloc(sizeof(float) * num_bins));
  for (int i = 0; i < num_bins; i++)
  {
    bar_chart_peaks[i] = 0.0f;
  }
}

void GraphicEqualiser::update(float *mag)
{
  for (int i = 0; i < m_num_bins; i++)
  {
    float m = mag[i];
    if (m > bar_chart[i])
    {
      bar_chart[i] = m;
    }
    else
    {
      bar_chart[i] = 0.7 * bar_chart[i] + 0.3 * m;
    }
    if (m > bar_chart_peaks[i])
    {
      bar_chart_peaks[i] = m;
    }
    else
    {
      bar_chart_peaks[i] = 0.95 * bar_chart_peaks[i] + 0.05 * m;
    }
  }
}

void GraphicEqualiser::_draw(M5Display &display)
{
  int x = 0;
  int x_step = int(320.0f / (m_num_bins / 16));
  for (int i = 2; i < m_num_bins / 4; i += 4)
  {
    float ave = 0;
    for (int j = 0; j < 4; j++)
    {
      ave += bar_chart[i + j];
    }
    ave /= 4;
    int bar_value = std::min(120.0f, 0.25f * ave);
    ave = 0;
    for (int j = 0; j < 4; j++)
    {
      ave += bar_chart_peaks[i + j];
    }
    ave /= 4;
    int peak_value = std::min(120.0f, 0.25f * ave);
    display.fillRect(x, 120, x_step, 120 - bar_value, 0);
    display.drawLine(x, 240 - peak_value, x + x_step - 1, 240 - peak_value, m_palette->get_color(135 + peak_value));
    display.fillRect(x, 240 - bar_value, x_step - 1, bar_value, m_palette->get_color(135 + bar_value));
    x += x_step;
  }
  display.fillRect(x, 120, 320 - x, 120, 0);
}