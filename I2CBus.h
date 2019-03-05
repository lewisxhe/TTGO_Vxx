#ifndef TTGO_I2CBUF_H
#define TTGO_I2CBUF_H

#include <Wire.h>

class I2CBus
{
  public:
    I2CBus()
    {
        Wire.begin();
    };
    ~I2CBus(){};
    void scanI2Cdevice(void);
    bool readBytes(uint8_t addr, uint8_t reg, uint8_t nbytes, uint8_t *data);
    bool writeBytes(uint8_t addr, uint8_t reg, uint8_t data);
};

extern I2CBus i2c;
#endif