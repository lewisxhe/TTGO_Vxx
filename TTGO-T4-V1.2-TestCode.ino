#include <TFT_eSPI.h> // Hardware-specific library
#include <SPI.h>
#include <OneButton.h>
#include "WiFi.h"
#include <Wire.h>
#include <FS.h>
#include <SD.h>

#define BUTTON_1 39
#define BUTTON_2 37
#define BUTTON_3 38
#define BUTTON_4 0

OneButton button1(BUTTON_1, true);
OneButton button2(BUTTON_2, true);
OneButton button3(BUTTON_3, true);
OneButton button4(BUTTON_4, true);

#define TFT_BL 4            // LED back-light
TFT_eSPI tft = TFT_eSPI();  // Invoke custom library
SPIClass *sdObj = nullptr;
#define ENABLE_SPI_SD


#define SD_SPI_CS 13   
#define SD_SPI_MOSI 15 
#define SD_SPI_MISO 2 
#define SD_SPI_CLK 14 

#define I2C_SDA 21
#define I2C_SCL 22

char buff[512];

#define IP5306_ADDR 0X75
#define IP5306_REG_SYS_CTL0 0x00

bool setPowerBoostKeepOn(int en)
{
    Wire.beginTransmission(IP5306_ADDR);
    Wire.write(IP5306_REG_SYS_CTL0);
    if (en)
        Wire.write(0x37); // Set bit1: 1 enable 0 disable boost keep on
    else
        Wire.write(0x35); // 0x37 is default reg value
    return Wire.endTransmission() == 0;
}


void listDir(fs::FS &fs, const char *dirname, uint8_t levels)
{
    tft.setTextColor(TFT_GREEN, TFT_BLACK);
    tft.fillScreen(TFT_BLACK);
    tft.setTextDatum(TL_DATUM);
    tft.setCursor(0, 0);

    tft.println("Listing directory:" + String(dirname));

    File root = fs.open(dirname);
    if (!root) {
        tft.println("- failed to open directory");
        return;
    }
    if (!root.isDirectory()) {
        tft.println(" - not a directory");
        return;
    }

    File file = root.openNextFile();
    while (file) {
        if (file.isDirectory()) {
            tft.print("  DIR : ");
            tft.println(file.name());
            if (levels) {
                listDir(fs, file.name(), levels - 1);
            }
        } else {
            tft.print("  FILE: ");
            tft.print(file.name());
            tft.print("  SIZE: ");
            tft.println(file.size());
        }
        file = root.openNextFile();
    }
}

void button1Func()
{
    tft.setTextColor(TFT_GREEN, TFT_BLACK);
    tft.fillScreen(TFT_BLACK);
    tft.setTextDatum(MC_DATUM);
    tft.drawString("Button Undefined function", tft.width() / 2, tft.height() / 2);
}

void button2Func()
{
    tft.setTextColor(TFT_GREEN, TFT_BLACK);
    tft.fillScreen(TFT_BLACK);
    tft.setTextDatum(MC_DATUM);
    tft.setTextSize(1);
    tft.drawString("Scan Network",  tft.width() / 2, tft.height() / 2);
    WiFi.mode(WIFI_STA);
    WiFi.disconnect();
    delay(100);
    int16_t n = WiFi.scanNetworks();
    tft.fillScreen(TFT_BLACK);
    if (n == 0) {
        tft.drawString("no networks found",  tft.width() / 2, tft.height() / 2);
    } else {
        tft.setTextDatum(TL_DATUM);
        tft.setCursor(0, 0);
        for (int i = 0; i < n; ++i) {
            sprintf(buff,
                    "[%d]:%s(%d)",
                    i + 1,
                    WiFi.SSID(i).c_str(),
                    WiFi.RSSI(i));
            tft.println(buff);
        }
    }
}

void button3Func()
{
    listDir(SD, "/", 2);
}

void button4Func()
{
    int x = tft.width() / 2;
    int y = tft.height() / 2;
    int r = digitalRead(TFT_BL);

    tft.setTextColor(TFT_GREEN, TFT_BLACK);
    tft.setTextDatum(MC_DATUM);
    tft.fillScreen(TFT_BLACK);
    tft.drawString(r ? "Backlight OFF" : "Backlight ON", x, y);
    tft.drawString("IP5306 KeepOn ", x - 20, y + 30);

    bool isOk = setPowerBoostKeepOn(1);
    tft.setTextColor(isOk ? TFT_GREEN : TFT_RED, TFT_BLACK);
    tft.drawString( isOk ? "PASS" : "FAIL", x + 50, y + 30);
    if (!isOk) {
        char *str = Wire.getErrorText(Wire.lastError());
        String err = "Wire " + String(str);
        tft.drawString( err, x + 50, y + 60);
        y += 60;
    } else {
        y += 30;
    }
    tft.setTextColor(TFT_GREEN, TFT_BLACK);
    tft.drawString("Press again to wake up", x - 20, y + 30);

    delay(6000);
    digitalWrite(TFT_BL, !r);
    esp_sleep_enable_ext0_wakeup((gpio_num_t )BUTTON_4, LOW);
    esp_deep_sleep_start();
}

//========================================SP SD

void spisd_test()
{
#ifdef ENABLE_SPI_SD
    tft.setTextDatum(MC_DATUM);
    sdObj = new SPIClass(HSPI);
    sdObj->begin(SD_SPI_CLK, SD_SPI_MISO, SD_SPI_MOSI, SD_SPI_CS);

    if (!SD.begin(SD_SPI_CS, *sdObj)) {
        tft.setTextFont(2);
        tft.setTextColor(TFT_RED, TFT_BLACK);
        tft.drawString("SD FAIL",  tft.width() / 2, tft.height() / 2);
    } else {
        uint32_t size = SD.cardSize() / 1024 / 1024;
        String info = "SIZE:" + String(size ) + "MB";
        tft.setTextFont(2);
        tft.setTextColor(TFT_GREEN, TFT_BLACK);
        tft.drawString(info,  tft.width() / 2, tft.height() / 2);
    }
#endif
}

void scanButton()
{
    button1.tick();
    button2.tick();
    button3.tick();
    button4.tick();
}

void setup()
{
    Serial.begin(115200);

    //Set up the display
    pinMode(TFT_BL, OUTPUT);
    // turn on backlight
    digitalWrite(TFT_BL, HIGH);

    tft.init();
    tft.setRotation(0);
    tft.fillScreen(TFT_BLACK);
    tft.setTextSize(1);
    tft.setTextColor(TFT_WHITE);
    tft.setCursor(0, 0);

    Wire.begin(I2C_SDA, I2C_SCL);

    spisd_test();
    button1.attachClick(button1Func);
    button2.attachClick(button2Func);
    button3.attachClick(button3Func);
    button4.attachClick(button4Func);
}

void loop()
{
    scanButton();
}
