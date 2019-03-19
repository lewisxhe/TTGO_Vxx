#ifndef TTGO_MPU9250_H
#define TTGO_MPU9250_H

// #include "TTGO_Config.h"
#include "I2CBus.h"
#define ENABLE_MPU9250

#ifdef ENABLE_MPU9250

#define MPU9250_ADDRESS 0x68

#define MAG_ADDRESS 0x0C

#define GYRO_FULL_SCALE_250_DPS 0x00
#define GYRO_FULL_SCALE_500_DPS 0x08
#define GYRO_FULL_SCALE_1000_DPS 0x10
#define GYRO_FULL_SCALE_2000_DPS 0x18

#define ACC_FULL_SCALE_2_G 0x00
#define ACC_FULL_SCALE_4_G 0x08
#define ACC_FULL_SCALE_8_G 0x10
#define ACC_FULL_SCALE_16_G 0x18

typedef struct
{
    int16_t ax;
    int16_t ay;
    int16_t az;
} Accelerometer_t;

typedef struct
{
    int16_t gx;
    int16_t gy;
    int16_t gz;
} Gyroscope_t;

typedef struct
{
    int16_t mx;
    int16_t my;
    int16_t mz;
} Magnetometer_t;

class MPU9250
{
  public:
    MPU9250(I2CBus &i2c);
    ~MPU9250();
    void begin(void);
    void getAccelerometer(Accelerometer_t *acce);
    void getGyroscope(Gyroscope_t *gyro);
    void getMagnetometer(Magnetometer_t *mag);

  private:
    I2CBus *_i2c;
    uint8_t _buffer[14];
};

#endif
#endif