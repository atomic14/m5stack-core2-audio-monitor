#include <Arduino.h>
#include <M5Core2.h>
#include <driver/i2s.h>
#include "Application.h"

// approx 30ms of audio @ 16KHz
#define WINDOW_SIZE 512

// i2s config for reading from both m5stack mic
i2s_config_t i2s_config = {
    .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_RX | I2S_MODE_PDM),
    .sample_rate = 64000,
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
  M5.begin(
      true,  // LCDEnable
      false, // SDEnable
      false, // SerialEnable !important
      false  // I2CEnable
  );
  M5.Axp.SetLcdVoltage(3000);
  M5.Lcd.fillScreen(TFT_BLACK);
  // M5Core grabs I2S_NUM_0 so let's get it back
  i2s_driver_uninstall(I2S_NUM_0);

  // create our application
  application = new Application(M5.Lcd, M5.Touch, WINDOW_SIZE);
  application->begin(i2s_config, i2s_pins);
}

void loop()
{
  // service the application
  application->loop();
}