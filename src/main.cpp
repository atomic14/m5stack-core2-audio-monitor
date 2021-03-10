#include <Arduino.h>
#include <M5Core2.h>
#include <driver/i2s.h>
#include "Application.h"

#define WINDOW_SIZE 480

// i2s config for reading from both m5stack mic
i2s_config_t i2s_config = {
    .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_RX | I2S_MODE_PDM),
    .sample_rate = 16000,
    .bits_per_sample = I2S_BITS_PER_SAMPLE_16BIT,
    .channel_format = I2S_CHANNEL_FMT_ONLY_RIGHT,
    .communication_format = I2S_COMM_FORMAT_I2S,
    .intr_alloc_flags = ESP_INTR_FLAG_LEVEL1,
    .dma_buf_count = 2,
    .dma_buf_len = 1024,
};

// i2s pins
i2s_pin_config_t i2s_pins = {
    .bck_io_num = GPIO_NUM_12,
    .ws_io_num = GPIO_NUM_0,
    .data_out_num = I2S_PIN_NO_CHANGE,
    .data_in_num = GPIO_NUM_34};

Application *application;

void setup()
{
  Serial.begin(115200);
  M5.begin(true, false, false, false);
  M5.Lcd.setBrightness(255);
  M5.Lcd.fillScreen(TFT_BLACK);
  Serial.println("Started up");

  application = new Application(M5.Lcd, M5.Touch, WINDOW_SIZE);
  application->begin(i2s_config, i2s_pins);
}

void loop()
{
  application->loop();
}