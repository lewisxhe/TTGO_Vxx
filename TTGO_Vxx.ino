
// #define T4_V12
// #define T4_V13
// #define T10_V14
// #define T10_V18


#if defined (T10_V18)
#include "T10_V18.h"
#elif defined(T10_V14)
#include "T10_V14.h"
#elif defined(T4_V12)
#include "T4_V12.h"
#elif defined(T4_V13)
#include "T4_V13.h"
#else
#error "please select board version"
#endif


#include <TFT_eSPI.h>
#include <SPI.h>
#include "WiFi.h"
#include <Wire.h>
#include <Ticker.h>
#include <Button2.h>
#include <SD.h>

TFT_eSPI tft = TFT_eSPI(); // Invoke custom library

#ifdef ENABLE_MPU9250
#include "MPU9250.h"
#include "I2CBus.h"
Accelerometer_t acce;
Gyroscope_t gyro;
Magnetometer_t mag;
MPU9250 mpu(i2c);
#endif

SPIClass sdSPI(VSPI);
#define IP5306_ADDR         0X75
#define IP5306_REG_SYS_CTL0 0x00


uint8_t state = 0;
Button2 *pBtns = nullptr;
uint8_t g_btns[] =  BUTTONS_MAP;
char buff[512];
Ticker btnscanT;

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

void button_handle(uint8_t gpio)
{
    switch (gpio) {
#ifdef BUTTON_1
    case BUTTON_1: {
        state = 1;
    }
    break;
#endif

#ifdef BUTTON_2
    case BUTTON_2: {
        state = 2;
    }
    break;
#endif

#ifdef BUTTON_3
    case BUTTON_3: {
        state = 3;
    }
    break;
#endif

#ifdef BUTTON_4
    case BUTTON_4: {
        state = 4;
    }
    break;
#endif
    default:
        break;
    }
}

void button_callback(Button2 &b)
{
    for (int i = 0; i < sizeof(g_btns) / sizeof(g_btns[0]); ++i) {
        if (pBtns[i] == b) {
            Serial.printf("btn: %u press\n", pBtns[i].getAttachPin());
            button_handle(pBtns[i].getAttachPin());
        }
    }
}

void button_init()
{
    uint8_t args = sizeof(g_btns) / sizeof(g_btns[0]);
    pBtns = new Button2 [args];
    for (int i = 0; i < args; ++i) {
        pBtns[i] = Button2(g_btns[i]);
        pBtns[i].setPressedHandler(button_callback);
    }
#if defined(T10_V18) || defined(T4_V13)
    pBtns[0].setLongClickHandler([](Button2 & b) {

        int x = tft.width() / 2 ;
        int y = tft.height() / 2 - 30;
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
        esp_sleep_enable_ext0_wakeup((gpio_num_t )BUTTON_1, LOW);
        esp_deep_sleep_start();
    });
#endif
}

void button_loop()
{
    for (int i = 0; i < sizeof(g_btns) / sizeof(g_btns[0]); ++i) {
        pBtns[i].loop();
    }
}

void spisd_test()
{
    if (SD_CS >  0) {
        tft.fillScreen(TFT_BLACK);
        tft.setTextDatum(MC_DATUM);
        sdSPI.begin(SD_SCLK, SD_MISO, SD_MOSI, SD_CS);
        if (!SD.begin(SD_CS, sdSPI)) {
            tft.setTextFont(2);
            tft.setTextColor(TFT_RED, TFT_BLACK);
            tft.drawString("SDCard MOUNT FAIL", tft.width() / 2, tft.height() / 2);
        } else {
            uint32_t cardSize = SD.cardSize() / (1024 * 1024);
            String str = "SDCard Size: " + String(cardSize) + "MB";
            tft.setTextFont(2);
            tft.setTextColor(TFT_GREEN, TFT_BLACK);
            tft.drawString(str, tft.width() / 2, tft.height() / 2);
        }
        delay(2000);
    }

}


void playSound(void)
{
    if (SPEAKER_OUT > 0) {

        if (SPEAKER_PWD > 0) {
            digitalWrite(SPEAKER_PWD, HIGH);
            delay(200);
        }

        ledcWriteTone(CHANNEL_0, 1000);
        delay(200);
        ledcWriteTone(CHANNEL_0, 0);

        if (SPEAKER_PWD > 0) {
            delay(200);
            digitalWrite(SPEAKER_PWD, LOW);
        }
    }
}

void buzzer_test()
{
    if (SPEAKER_OUT > 0) {
        if (SPEAKER_PWD > 0) {
            pinMode(SPEAKER_PWD, OUTPUT);
        }
        ledcSetup(CHANNEL_0, 1000, 8);
        ledcAttachPin(SPEAKER_OUT, CHANNEL_0);
    }
}



void wifi_scan()
{
    tft.setTextColor(TFT_GREEN, TFT_BLACK);
    tft.fillScreen(TFT_BLACK);
    tft.setTextDatum(MC_DATUM);
    tft.setTextSize(1);

    tft.drawString("Scan Network", tft.width() / 2, tft.height() / 2);

    WiFi.mode(WIFI_STA);
    WiFi.disconnect();
    delay(100);

    int16_t n = WiFi.scanNetworks();
    tft.fillScreen(TFT_BLACK);
    if (n == 0) {
        tft.drawString("no networks found", tft.width() / 2, tft.height() / 2);
    } else {
        tft.setTextDatum(TL_DATUM);
        tft.setCursor(0, 0);
        Serial.printf("Fount %d net\n", n);
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

void setup()
{
    Serial.begin(115200);
    delay(1000);

    //Pin out Dump
    Serial.printf("Current select %s version\n", BOARD_VRESION);
    Serial.printf("TFT_MISO:%d\n", TFT_MISO);
    Serial.printf("TFT_MOSI:%d\n", TFT_MOSI);
    Serial.printf("TFT_SCLK:%d\n", TFT_SCLK);
    Serial.printf("TFT_CS:%d\n", TFT_CS);
    Serial.printf("TFT_DC:%d\n", TFT_DC);
    Serial.printf("TFT_RST:%d\n", TFT_RST);
    Serial.printf("TFT_BL:%d\n", TFT_BL);
    Serial.printf("SD_MISO:%d\n", SD_MISO);
    Serial.printf("SD_MOSI:%d\n", SD_MOSI);
    Serial.printf("SD_SCLK:%d\n", SD_SCLK);
    Serial.printf("SD_CS:%d\n", SD_CS);
    Serial.printf("I2C_SDA:%d\n", I2C_SDA);
    Serial.printf("I2C_SCL:%d\n", I2C_SCL);
    Serial.printf("SPEAKER_PWD:%d\n", SPEAKER_PWD);
    Serial.printf("SPEAKER_OUT:%d\n", SPEAKER_OUT);
    Serial.printf("ADC_IN:%d\n", ADC_IN);
    Serial.printf("BUTTON_1:%d\n", BUTTON_1);
    Serial.printf("BUTTON_2:%d\n", BUTTON_2);
    Serial.printf("BUTTON_3:%d\n", BUTTON_3);
#ifdef BUTTON_4
    Serial.printf("BUTTON_4:%d\n", BUTTON_4);
#endif

    tft.init();
    tft.setRotation(0);
    tft.fillScreen(TFT_BLACK);
    tft.setTextSize(1);
    tft.setTextColor(TFT_WHITE);
    tft.setCursor(0, 0);

    if (TFT_BL > 0) {
        pinMode(TFT_BL, OUTPUT);
        digitalWrite(TFT_BL, HIGH);
    }

    spisd_test();
    buzzer_test();
    button_init();

    if (I2C_SDA > 0) {
        Wire.begin(I2C_SDA, I2C_SCL);
#ifdef ENABLE_MPU9250
        i2c.scanI2Cdevice();
        mpu.begin();
#endif
    }
    btnscanT.attach_ms(30, button_loop);
}


void loop()
{
    switch (state) {
    case 1:
        state = 0;
        wifi_scan();
        break;
    case 2:
        state = 0;
        tft.setTextColor(TFT_GREEN, TFT_BLACK);
        tft.fillScreen(TFT_BLACK);
        tft.setTextDatum(MC_DATUM);
#ifdef T4_V12
        tft.drawString("Undefined function", tft.width() / 2, tft.height() / 2);
#else
        tft.drawString("Buzzer Test", tft.width() / 2, tft.height() / 2);
        playSound();
#endif
        break;
    case 3:
#ifdef ENABLE_MPU9250
        mpu.getAccelerometer(&acce);
        mpu.getGyroscope(&gyro);
        mpu.getMagnetometer(&mag);

        tft.setTextColor(TFT_GREEN, TFT_BLACK);
        tft.fillScreen(TFT_BLACK);
        tft.setTextDatum(TL_DATUM);

        snprintf(buff, sizeof(buff), "--  ACC GYR MAG");
        tft.drawString(buff, 0, 0);
        snprintf(buff, sizeof(buff), "x %d  %d  %d", acce.ax, gyro.gx, mag.mx);
        tft.drawString(buff, 0, 16);
        snprintf(buff, sizeof(buff), "y %d  %d  %d", acce.ay, gyro.gy, mag.my);
        tft.drawString(buff, 0, 32);
        snprintf(buff, sizeof(buff), "z %d  %d  %d", acce.az, gyro.gz, mag.mz);
        tft.drawString(buff, 0, 48);
        delay(200);
#else
        state = 0;
        listDir(SD, "/", 2);
#endif
        break;
    case 4:
        state = 0;
        tft.fillScreen(TFT_BLACK);
        tft.setTextDatum(MC_DATUM);
        tft.drawString("Undefined function", tft.width() / 2, tft.height() / 2);
        break;
    default:
        break;
    }
}