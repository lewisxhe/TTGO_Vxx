// #define TFT_MISO    -1
// #define TFT_MOSI    23
// #define TFT_SCLK    5
// #define TFT_CS      16  // Chip select control pin
// #define TFT_DC      17  // Data Command control pin
// #define TFT_RST     -1  // Reset pin (could connect to RST pin)
#define TFT_BL      27  // Dispaly backlight control pin

#define SD_MISO     2
#define SD_MOSI     15
#define SD_SCLK     14
#define SD_CS       13

#define I2C_SDA     21
#define I2C_SCL     22

#define SPEAKER_PWD -1
#define SPEAKER_OUT 25
#define ADC_IN      35

#define BUTTON_1    36
#define BUTTON_2    37
#define BUTTON_3    39

#define BUTTONS_MAP {BUTTON_1,BUTTON_2,BUTTON_3}
#define ENABLE_MPU9250
#define CHANNEL_0   0
#define BOARD_VRESION   "<T10 V2.0>"