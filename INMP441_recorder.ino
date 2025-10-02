#include <Arduino.h>
#include "clases.h"
#include <Adafruit_NeoPixel.h>
#include <SD_MMC.h>            // SD MMC mode.

Adafruit_NeoPixel pixels(NUMPIXELS, RGB_BUILTIN, NEO_GRB + NEO_KHZ800);
INMP441 mic;
PSRAMBuffer psram;            // PSRAM como buffer principal
const char* audioFileName = "/audio.bin";

void setup() {
  myInit();
  i2sRead();
  savePsramToSD();
}

void loop() {
  visuals();
}

void myInit() {
  pixels.setPixelColor(0, pixels.Color( 0,  0, 0));
  delay(1000);
  Serial.begin(115200);
  while (!Serial) delay(1000);
  delay(1000);
  Serial.println("Iniciando grabación...");

  if (!psram.init(0)) {  // PSRAM
    Serial.println("No se pudo reservar PSRAM");
    while (1);
  }
  if (!SD_MMC.setPins(MMC_CLK, MMC_CMD, MMC_D0, MMC_D1, MMC_D2, MMC_D3))
  {
    Serial.println("Falló MMC setPins");
    while (1);
  }

  if (!mic.begin(I2S_PIN_BCK, I2S_PIN_WS, I2S_PIN_SDIN,
                 I2S_SAMPLE_RATE, I2S_BUF_COUNT, I2S_BUF_LEN, I2S_MIC_LR_PIN_GND)) {
    Serial.println("Error inicializando I2S");
    while (1);
  }
}

void i2sRead(void) {
  static size_t offset = 0, i = 0;
  const size_t psram_size = psram.getSize();
  
  while (offset < psram_size) {
    if (i % 25 == 0) Serial.print(".");
    i++;
    size_t buffer_bytes = I2S_BUFFER_BYTES;
    size_t remaining = psram_size - offset;
    if (buffer_bytes > remaining) buffer_bytes = remaining;

    size_t bytesRead;
    if (mic.read(psram.get() + offset, buffer_bytes, &bytesRead) && bytesRead > 0) {
      offset += bytesRead;
    } else {
      Serial.println("Error leyendo I2S");
      break;
    }
  }

  size_t total_samples = offset / I2S_SAMPLE_SIZE;
  float duration_sec = (float)total_samples / I2S_SAMPLE_RATE;
  Serial.printf("\nPSRAM llena: %u bytes, %u muestras, %.2f s\n",
                (unsigned)offset, (unsigned)total_samples, duration_sec);
  Serial.printf("PSRAM total: %u bytes, datos guardados: %u bytes\n",
                (unsigned)psram_size, (unsigned)offset);
}

void savePsramToSD() {
    size_t psram_size = psram.getSize();  // tamaño real de PSRAM

    // --- Abrir archivo para escritura de datos ---
    if (!SD_MMC.begin("/sdcard", true, SDMMC_SPEED)) {
        Serial.println("SD init failed");
        return;
    }

    File file = SD_MMC.open(audioFileName, FILE_WRITE);
    if (!file) {
        Serial.println("No se pudo abrir archivo WAV para escribir PSRAM");
        SD_MMC.end();
        return;
    }

    // --- Escribir PSRAM en bloques ---
    size_t pos = 0;
    while (pos < psram_size) {
        size_t toWrite = min((size_t)BLOCK_SD, psram_size - pos);
        size_t written = file.write((uint8_t*)psram.get() + pos, toWrite);

        if (written != toWrite) {
            Serial.println("Error escribiendo bloque");
            file.close();
            SD_MMC.end();
            return;
        }
        pos += toWrite;
    }

    file.flush();
    file.close();
    SD_MMC.end();

    Serial.println("SD write complete");
}

void visuals() {
  Serial.println("Visuals...");
  delay(1000);
  pixels.setPixelColor(0, pixels.Color( 0,  0, 10));
  delay(1000);
  pixels.setPixelColor(0, pixels.Color( 0, 10,  0));
  delay(1000);
  pixels.setPixelColor(0, pixels.Color( 10, 0,  0));

}
