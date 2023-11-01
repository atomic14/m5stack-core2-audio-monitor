#include <Arduino.h>
#include <M5Core2.h>

#include "UI.h"
#include "UI/GraphicEqualiser.h"
#include "UI/Palette.h"
#include "UI/Spectrogram.h"
#include "UI/Waveform.h"

// Task to process samples
void drawing_task(void *param)
{
  UI *ui = reinterpret_cast<UI *>(param);
  const TickType_t xMaxBlockTime = pdMS_TO_TICKS(1000);
  while (true) {
    // wait to be told to redraw
    uint32_t ulNotificationValue = ulTaskNotifyTake(pdTRUE, xMaxBlockTime);
    if (ulNotificationValue != 0) {
      ui->draw();
    }
  }
}

UI::UI(M5Display &display, int window_size) : m_display(display)
{
  m_palette = new Palette();
  m_waveform = new Waveform(display, 0, 0, 320, 120, window_size);
  m_graphic_equaliser = new GraphicEqualiser(m_palette, 0, 120, 320, 120, window_size);
  m_spectrogram = new Spectrogram(m_palette, 0, 120, 320, 120);
  // start off with the spectrogram hidden
  m_spectrogram->visible = false;
  // create a drawing task to update our UI
  xTaskCreatePinnedToCore(drawing_task, "Drawing Task", 4096, this, 1, &m_draw_task_handle, 1);
}

void UI::toggle_display()
{
  m_graphic_equaliser->visible = !m_graphic_equaliser->visible;
  m_spectrogram->visible = !m_spectrogram->visible;
}

void UI::update(float *samples, float *fft)
{
  m_waveform->update(samples);
  m_graphic_equaliser->update(fft);
  m_spectrogram->update(fft);
  xTaskNotify(m_draw_task_handle, 1, eIncrement);
}

unsigned long draw_time = 0;
int draw_count = 0;
void UI::draw()
{
  auto start = millis();
  m_spectrogram->draw(m_display);
  m_graphic_equaliser->draw(m_display);
  m_waveform->draw(m_display);
  auto end = millis();
  draw_time += end - start;
  draw_count++;
  if (draw_count == 20) {
    Serial.printf("Drawing time %ld\n", draw_time / 20);
    draw_count = 0;
    draw_time = 0;
  }
}
