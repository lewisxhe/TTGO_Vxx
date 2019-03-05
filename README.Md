# TTGO T10 & T4 TEST CODE


## 1.Install the following dependency library files:
- [TFT_eSPI](https://github.com/Bodmer/TFT_eSPI)
- [Button2](https://github.com/lewisxhe/Button2)


## 2.TFT_eSPI settings
- Move or copy TX_Vxx.h to `TFT_eSPI/User_Setups/`
- Add `#include <User_Setups/TX_Vxx.h>` to  `TFT_eSPI/User_Setup_Select.h`
  

## T10
| Name        | V18    |
| ----------- | ------ |
| TFT Driver  | ST7735 |
| TFT_MISO    | N/A    |
| TFT_MOSI    | 23     |
| TFT_SCLK    | 5      |
| TFT_CS      | 16     |
| TFT_DC      | 17     |
| TFT_RST     | N/A    |
| TFT_BL      | 27     |
| SD_MISO     | 2      |
| SD_MOSI     | 15     |
| SD_SCLK     | 14     |
| SD_CS       | 13     |
| I2C_SDA     | 21     |
| I2C_SCL     | 22     |
| SPEAKER_OUT | 25     |
| ADC_IN      | 35     |
| BUTTON1     | 36     |
| BUTTON2     | 37     |
| BUTTON3     | 39     |

## T4 
| Name        | V12     | V13     |
| ----------- | ------- | ------- |
| TFT Driver  | ILI9341 | ILI9341 |
| TFT_MISO    | 12      | 12      |
| TFT_MOSI    | 23      | 23      |
| TFT_SCLK    | 18      | 18      |
| TFT_CS      | 27      | 27      |
| TFT_DC      | 26      | 32      |
| TFT_RST     | 5       | 5       |
| SD_MISO     | 2       | 2       |
| SD_MOSI     | 15      | 15      |
| SD_SCLK     | 14      | 14      |
| SD_CS       | 13      | 13      |
| Button 0    | 14      | 14      |
| Button 1    | 5       | 5       |
| Button 2    | 27      | 27      |
| Button 3    | 25      | 25      |
| SPEAKER PWD | N/A     | 19      |
| SPEAKER OUT | N/A     | 25      |
| I2C_SDA     | 21      | 21      |
| I2C_SCL     | 22      | 22      |