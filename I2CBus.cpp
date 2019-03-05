#include "I2CBus.h"
#include "Wire.h"
#include <Arduino.h>
void I2CBus::scanI2Cdevice(void)
{
    uint8_t err, addr;
    int nDevices = 0;
    for (addr = 1; addr < 127; addr++)
    {
        Wire.beginTransmission(addr);
        err = Wire.endTransmission();
        if (err == 0)
        {
            Serial.print("I2C device found at address 0x");
            if (addr < 16)
                Serial.print("0");
            Serial.print(addr, HEX);
            Serial.println(" !");
            nDevices++;
        }
        else if (err == 4)
        {
            Serial.print("Unknow error at address 0x");
            if (addr < 16)
                Serial.print("0");
            Serial.println(addr, HEX);
        }
    }
    if (nDevices == 0)
        Serial.println("No I2C devices found\n");
    else
        Serial.println("done\n");
}

bool I2CBus::readBytes(uint8_t addr, uint8_t reg, uint8_t nbytes, uint8_t *data)
{
    // Set register address
    Wire.beginTransmission(addr);
    Wire.write(reg);
    Wire.endTransmission();

    // Read Nbytes
    Wire.requestFrom(addr, nbytes);
    uint8_t index = 0;
    while (Wire.available())
        data[index++] = Wire.read();
}

bool I2CBus::writeBytes(uint8_t addr, uint8_t reg, uint8_t data)
{
    // Set register address
    Wire.beginTransmission(addr);
    Wire.write(reg);
    Wire.write(data);
    Wire.endTransmission();
}

I2CBus i2c = I2CBus();
