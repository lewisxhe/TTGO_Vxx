#include "MPU9250.h"
#include <Arduino.h>
// #include "Wire.h"
#ifdef ENABLE_MPU9250

// // This function read Nbytes bytes from I2C device at address Address.
// // Put read bytes starting at register Register in the Data array.
// static void I2Cread(uint16_t Address, uint8_t Register, uint8_t Nbytes, uint8_t *Data)
// {
//     // Set register address
//     Wire.beginTransmission(Address);
//     Wire.write(Register);
//     Wire.endTransmission();

//     // Read Nbytes
//     Wire.requestFrom(Address, Nbytes);
//     uint8_t index = 0;
//     while (Wire.available())
//         Data[index++] = Wire.read();
// }
// static void I2CwriteByte(uint16_t Address, uint8_t Register, uint8_t Data)
// {
//     // Set register address
//     Wire.beginTransmission(Address);
//     Wire.write(Register);
//     Wire.write(Data);
//     Wire.endTransmission();
// }

MPU9250::MPU9250(I2CBus &i2c)
{
    _i2c = &i2c;
}
MPU9250::~MPU9250()
{
}

void MPU9250::begin()
{
    // Configure gyroscope range
    _i2c->writeBytes(MPU9250_ADDRESS, 27, GYRO_FULL_SCALE_2000_DPS);
    // I2CwriteByte(MPU9250_ADDRESS, 27, GYRO_FULL_SCALE_2000_DPS);
    // Configure accelerometers range
    _i2c->writeBytes(MPU9250_ADDRESS, 28, ACC_FULL_SCALE_16_G);
    // I2CwriteByte(MPU9250_ADDRESS, 28, ACC_FULL_SCALE_16_G);
    // Set by pass mode for the magnetometers
    _i2c->writeBytes(MPU9250_ADDRESS, 0x37, 0x02);
    // I2CwriteByte(MPU9250_ADDRESS, 0x37, 0x02);
    // Request first magnetometer single measurement
    _i2c->writeBytes(MAG_ADDRESS, 0x0A, 0x16);
    // I2CwriteByte(MAG_ADDRESS, 0x0A, 0x16);
}

void MPU9250::getAccelerometer(Accelerometer_t *acce)
{
    if (!acce)
        return;
    _i2c->readBytes(MPU9250_ADDRESS, 0x3B, 14, _buffer);
    // I2Cread(MPU9250_ADDRESS, 0x3B, 14, _buffer);
    acce->ax = (_buffer[0] << 8 | _buffer[1]);
    acce->ay = (_buffer[2] << 8 | _buffer[3]);
    acce->az = _buffer[4] << 8 | _buffer[5];
}

void MPU9250::getGyroscope(Gyroscope_t *gyro)
{
    if (!gyro)
        return;
    _i2c->readBytes(MPU9250_ADDRESS, 0x3B, 14, _buffer);
    // I2Cread(MPU9250_ADDRESS, 0x3B, 14, _buffer);
    gyro->gx = (_buffer[8] << 8 | _buffer[9]);
    gyro->gy = (_buffer[10] << 8 | _buffer[11]);
    gyro->gz = _buffer[12] << 8 | _buffer[13];
}

void MPU9250::getMagnetometer(Magnetometer_t *mag)
{
    if (!mag)
        return;
    _i2c->writeBytes(MAG_ADDRESS, 0x37, 0x02);
    _i2c->readBytes(MAG_ADDRESS, 0x03, 7, _buffer);
    // I2CwriteByte(MAG_ADDRESS, 0x37, 0x02);
    // I2Cread(MAG_ADDRESS, 0x03, 7, _buffer);
    mag->mx = (_buffer[0] << 8 | _buffer[1]);
    mag->my = (_buffer[2] << 8 | _buffer[3]);
    mag->mz = _buffer[4] << 8 | _buffer[5];
}

#endif