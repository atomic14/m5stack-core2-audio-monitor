
#include <Arduino.h>
#include "I2SSampler.h"
#include "driver/i2s.h"

void I2SSampler::addSample(int16_t sample)
{
    // add the sample to the current audio buffer
    m_currentAudioBuffer[m_audioBufferPos] = sample;
    m_audioBufferPos++;
    // have we filled the buffer with data?
    if (m_audioBufferPos == m_bufferSizeInSamples)
    {
        // swap to the other buffer
        std::swap(m_currentAudioBuffer, m_capturedAudioBuffer);
        // reset the buffer position
        m_audioBufferPos = 0;
        // tell the writer task to save the data
        xTaskNotify(m_writerTaskHandle, 1, eIncrement);
    }
}

void i2sReaderTask(void *param)
{
    I2SSampler *sampler = (I2SSampler *)param;
    while (true)
    {
        // wait for some data to arrive on the queue
        i2s_event_t evt;
        if (xQueueReceive(sampler->m_i2sQueue, &evt, portMAX_DELAY) == pdPASS)
        {
            if (evt.type == I2S_EVENT_RX_DONE)
            {
                size_t bytesRead = 0;
                do
                {
                    // read data from the I2S peripheral
                    int16_t i2sData[1024];
                    // read from i2s
                    i2s_read(sampler->m_i2sPort, i2sData, 2048, &bytesRead, 10);
                    for (int i = 0; i < bytesRead / 2; i++)
                    {
                        sampler->addSample(i2sData[i]);
                    }
                } while (bytesRead > 0);
            }
        }
    }
}

void I2SSampler::start(i2s_port_t i2sPort, const i2s_pin_config_t &i2sPins, const i2s_config_t &i2sConfig, int32_t bufferSizeInSamples, TaskHandle_t writerTaskHandle)
{
    m_i2sPort = i2sPort;
    m_writerTaskHandle = writerTaskHandle;
    m_bufferSizeInSamples = bufferSizeInSamples;
    m_bufferSizeInBytes = bufferSizeInSamples * sizeof(int16_t);
    m_audioBuffer1 = (int16_t *)malloc(m_bufferSizeInBytes);
    m_audioBuffer2 = (int16_t *)malloc(m_bufferSizeInBytes);

    m_currentAudioBuffer = m_audioBuffer1;
    m_capturedAudioBuffer = m_audioBuffer2;

    m_writerTaskHandle = writerTaskHandle;
    //install and start i2s driver
    i2s_driver_install(m_i2sPort, &i2sConfig, 4, &m_i2sQueue);
    i2s_set_pin(i2sPort, &i2sPins);
    // start a task to read samples from the ADC
    TaskHandle_t readerTaskHandle;
    xTaskCreatePinnedToCore(i2sReaderTask, "i2s Reader Task", 8192, this, 1, &readerTaskHandle, 0);
}
