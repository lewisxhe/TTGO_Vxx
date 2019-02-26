TTGO T4 V1.2 TestCode
===========================

## 1.Install the following dependency library files:
- [TFT_eSPI](https://github.com/Bodmer/TFT_eSPI)
- [OneButton](https://github.com/mathertel/OneButton)


## 2.TFT_eSPI settings
- Move or copy T4_V1x.h to `TFT_eSPI/User_Setups/`
- Add `#include <User_Setups/T4_V1x.h>` to  TFT_eSPI/User_Setup_Select.h


## TTGO T4 Pinout
| Name        | V12 | V13 |
| ----------- | --- | --- |
| TFT_MISO    | 12  | 12  |
| TFT_MOSI    | 23  | 23  |
| TFT_SCLK    | 18  | 18  |
| TFT_CS      | 27  | 27  |
| TFT_DC      | 32  | 26  |
| TFT_RST     | 5   | 5   |
| SD_MISO     | 2   | 2   |
| SD_MOSI     | 15  | 15  |
| SD_SCLK     | 14  | 14  |
| SD_CS       | 13  | 13  |
| Button 0    | 14  | 14  |
| Button 1    | 5   | 5   |
| Button 2    | 27  | 27  |
| Button 3    | 25  | 25  |
| SPEAKER PWD | 19  | N/A |
| SPEAKER OUT | 25  | N/A |
| IP5306 SDA  | 21  | 21  |
| IP5306 SCL  | 22  | 22  |

- T4 V13 Deepsleep current  ~1.1mA