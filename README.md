# INMP441 Arduino Recorder

Note: This project does **not produce a standard WAV file**, since audio is saved as 32-bit raw. (wav files are 16 bit PCM)
ESP32-S3 Dev Kit project that records audio from an **I2S INMP441 microphone** into **PSRAM** and saves it to the **SD card** as a raw `.bin` file. This allows different sample rates and full 32-bit raw recording for further analysis.

---

## Program Function

- Initializes PSRAM, I2S, and SDMMC.
- Reads audio from the INMP441 and stores it in PSRAM.
- When the buffer is full, saves the data to `audio.bin` on the SD card.
- Shows activity with a NeoPixel LED.
- The main loop only updates visuals; recording occurs in `setup()`.

---

## Pin Configuration

| Function        | Pin |
|-----------------|-----|
| WS (I2S)        | 13  |
| BCK (I2S)       | 12  |
| DIN (I2S)       | 11  |
| SD D0           | 15  |
| SD D1           | 16  |
| SD D2           | 4   |
| SD D3           | 5   |
| SD CMD          | 6   |
| SD CLK          | 7   |
| NeoPixel LED    | 48  |

---

## Audio Settings

- Microphone: INMP441 I2S  
- Sample rate: **64 kHz** (customizable)  
- Resolution: **32-bit PCM**  
- Channel used: **Left** (connect L/R pin to GND as needed)  
- Buffer: Entire PSRAM  
- Saved file: `audio.bin` (raw 32-bit)  

---

## Notes

- The `.bin` file can be processed in Python for playback or conversion to WAV.  
- NeoPixel LED indicates recording and visual activity.
