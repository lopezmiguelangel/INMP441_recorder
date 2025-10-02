#ifndef CLASES_H
#define CLASES_H

#include <Arduino.h>
#include <driver/i2s_std.h>
#include <SD_MMC.h>
#include "esp_heap_caps.h"

// ====== Defines ======
  #define RGB_BUILTIN 48  // Pin LED interno
  #define NUMPIXELS 1
  #define CHANNEL_LEFT true
// ====== INMP441 ======
  #define I2S_MIC_LR_PIN_GND true
  #define I2S_PIN_WS 13
  #define I2S_PIN_BCK 12
  #define I2S_PIN_SDIN 11
  #define I2S_SAMPLE_RATE 64000
  #define I2S_BUF_COUNT 6
  #define I2S_BUF_LEN (I2S_SAMPLE_RATE / 200)
  #define I2S_SAMPLE_SIZE sizeof(uint32_t)
  #define I2S_BUFFER_SAMPLES ((I2S_BUF_COUNT - 1) * I2S_BUF_LEN)
  #define I2S_BUFFER_BYTES (I2S_BUFFER_SAMPLES * I2S_SAMPLE_SIZE)
// ======= SDMMC =======
  #define MMC_D2 4  
  #define MMC_D3 5
  #define MMC_CMD 6
  #define MMC_CLK 7
  #define MMC_D0 15
  #define MMC_D1 16
  #define BLOCK_SD 1024*3          // 3KB is the fastest size to save data in chunks to the SD in MMC mode.
  #define SDMMC_SPEED SDMMC_FREQ_HIGHSPEED // Best for ESP32-S3.
// =====================

// ==== PSRAMBuffer ====
class PSRAMBuffer {
public:
    PSRAMBuffer();
    bool init(size_t size = 0);
    uint8_t* get();
    size_t getSize();
    void freeBuffer();

private:
    uint8_t* buffer;
    size_t size_bytes;
};
// =====================

// ===== INMP441 =====
class INMP441 {
public:
    INMP441();
    ~INMP441();

    bool begin(int bck = I2S_PIN_BCK,
               int ws = I2S_PIN_WS,
               int din = I2S_PIN_SDIN,
               int sample_rate = I2S_SAMPLE_RATE,
               int buf_count = I2S_BUF_COUNT,
               int buf_len = I2S_BUF_LEN,
               bool channelLeft = I2S_MIC_LR_PIN_GND);

    bool read(uint8_t* destination, size_t bytes_to_read, size_t* read);

private:
    i2s_chan_handle_t rx_handle;
    uint8_t* aux_buffer;
    size_t aux_buffer_size;
    static constexpr size_t word_size = 4;
    bool channelLeft;
};

// --------------------
// Funciones auxiliares
void initSDMMC();

#endif
