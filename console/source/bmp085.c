#include "stdio.h"
#include "timer.h"
#include "i2c.h"
#include "bmp085.h"

#define MAX_LEN 32


char buffer[MAX_LEN];
char register_address;
static struct bmp085calibration calibration_data;
static long temperature = 0;
static long pressure = 0;


uint32_t BMP085getVersion(void)
{
  uint8_t ret_val;
  buffer[0] = 0x0;
  register_address = BMP085_REG_VERSION;
  ret_val = i2cReadWithRegister(BMP085_DEFAULT_ADDRESS, &register_address, 1, buffer);
//printf("  -error: %d\n",ret_val);
  return buffer[0];
}

void BMP085init()
{
  calibration_data.AC1 = 0;
  calibration_data.AC2 = 0;
  calibration_data.AC3 = 0;
  calibration_data.AC4 = 0;
  calibration_data.AC5 = 0;
  calibration_data.AC6 = 0;
  calibration_data.B1  = 0;
  calibration_data.B2  = 0;
  calibration_data.MB  = 0;
  calibration_data.MC  = 0;
  calibration_data.MD  = 0;
  buffer[0] = 0x0;   buffer[1] = 0x0;   buffer[2] = 0x0;   buffer[3] = 0x0;
  buffer[4] = 0x0;   buffer[5] = 0x0;   buffer[6] = 0x0;   buffer[7] = 0x0;
  buffer[8] = 0x0;   buffer[9] = 0x0;   buffer[10] = 0x0;  buffer[11] = 0x0;
  buffer[12] = 0x0;  buffer[13] = 0x0;  buffer[14] = 0x0;  buffer[15] = 0x0;
  buffer[16] = 0x0;  buffer[17] = 0x0;  buffer[18] = 0x0;  buffer[19] = 0x0;
  buffer[20] = 0x0;  buffer[21] = 0x0;
  register_address = BMP085_REG_AC1_MSB;
  i2cReadWithRegister(BMP085_DEFAULT_ADDRESS, &register_address, 22, buffer);

  calibration_data.AC1 = (buffer[0]  << 8) + buffer[1];
  calibration_data.AC2 = (buffer[2]  << 8) + buffer[3];
  calibration_data.AC3 = (buffer[4]  << 8) + buffer[4];
  calibration_data.AC4 = (buffer[6]  << 8) + buffer[7];
  calibration_data.AC5 = (buffer[8]  << 8) + buffer[9];
  calibration_data.AC6 = (buffer[10] << 8) + buffer[11];
  calibration_data.B1  = (buffer[12] << 8) + buffer[13];
  calibration_data.B2  = (buffer[14] << 8) + buffer[15];
  calibration_data.MB  = (buffer[16] << 8) + buffer[17];
  calibration_data.MC  = (buffer[18] << 8) + buffer[19];
  calibration_data.MD  = (buffer[20] << 8) + buffer[21];

  printf("  - AC1: %d\n",calibration_data.AC1);
  printf("  - AC2: %d\n",calibration_data.AC2);
  printf("  - AC3: %d\n",calibration_data.AC3);
  printf("  - AC4: %d\n",calibration_data.AC4);
  printf("  - AC5: %d\n",calibration_data.AC5);
  printf("  - AC6: %d\n",calibration_data.AC6);
  printf("  - B1:  %d\n",calibration_data.B1);
  printf("  - B2:  %d\n",calibration_data.B2);
  printf("  - MB:  %d\n",calibration_data.MB);
  printf("  - MC:  %d\n",calibration_data.MC);
  printf("  - MD:  %d\n",calibration_data.MD);

  temperature = 0;
  pressure = 0;
}

uint32_t BMP085getTemp()
{
  buffer[1] = BMP085_REG_COMMAND;
  buffer[1] = BMP085_COMMAND_REQUEST_UT;

  i2cWrite(BMP085_DEFAULT_ADDRESS, 2, buffer);

  delay(BMP085_WAITING);

  register_address = BMP085_REG_RESULT_MSB;
  i2cReadWithRegister(BMP085_DEFAULT_ADDRESS, &register_address, 2, buffer);
  return (buffer[0]<<8)+buffer[1];
}

uint32_t BMP085getPressure()
{
  buffer[1] = BMP085_REG_COMMAND;
  buffer[1] = BMP085_COMMAND_REQUEST_UP(BMP085_MODE_ULTRALOWPOWER);

  i2cWrite(BMP085_DEFAULT_ADDRESS, 2, buffer);

  delay(BMP085_WAITING);

  register_address = BMP085_REG_RESULT_MSB;
  i2cReadWithRegister(BMP085_DEFAULT_ADDRESS, &register_address, 3, buffer);

  return ((buffer[0] << 16) + (buffer[1] << 8) + buffer[2]) >> (8 - BMP085_MODE_ULTRALOWPOWER);
}


uint32_t power(uint32_t basis, uint32_t exponent)
{
  uint32_t temp = basis;
  uint32_t i;
  for(i = 0;i<exponent;i++)
    temp *=basis;

  return temp;
}

//float BMP085getAltitude()
uint32_t BMP085getAltitude()
{
  long temprerature_raw;
  long pressure_raw;
  long X1;
  long X2;
  long B5;
  long B6;
  long X3;
  long B3;
  unsigned long B4;
  unsigned long B7;
  long p;
//  long pressure;
  uint32_t sealevelPressure;
  uint32_t altitude = 0;

//-------------
  temprerature_raw = BMP085getTemp();
  pressure_raw = BMP085getPressure();

// temperature calculation
  X1 = (temprerature_raw - calibration_data.AC6) * calibration_data.AC5 / (1 << 15);
  X2 = calibration_data.MC * (1 << 11) / (X1 + calibration_data.MD);
  B5 = X1 + X2;
  temperature = (B5 + 8) / (1 << 4);

// pressure calculation
  B6 = B5 - 4000;
  X1 = (calibration_data.B2 * ((B6 * B6) >> 12)) >> 11;
  X2 = (calibration_data.AC2 * B6) >> 11;
  X3 = X1 + X2;
  B3 = (((calibration_data.AC1 * 4 + X3) << BMP085_MODE_ULTRALOWPOWER) + 2) / 4;

  X1 = (calibration_data.AC3 * B6) >> 13;
  X2 = (calibration_data.B1 * ((B6 * B6) >> 12)) >> 16;
  X3 = ((X1 + X2) + 2) >> 2;
  B4 = (calibration_data.AC4 * (unsigned long)(X3 + 32768)) >> 15;
  B7 = ((unsigned long)pressure_raw - B3) * (50000 >> BMP085_MODE_ULTRALOWPOWER);

  if (B7 < 0x80000000)
  {
    p = (B7 * 2) / B4;
  }
  else
  {
    p = (B7 / B4) * 2;
  }
  X1 = (p >> 8) * (p >> 8);
  X1 = (X1 * 3038) >> 16;
  X2 = (-7357 * p) >> 16;

  pressure = p + ((X1 + X2 + 3791) >> 4);

  printf("temperatur:  %d, pressure: %d\n",temperature,pressure);


  sealevelPressure = 101720;

  altitude = 44330 * (1 - power(pressure /sealevelPressure,200)/power(pressure /sealevelPressure,1051));

  return altitude;
}
/*
uint32_t BMP085getTemperature()
{
  return temperature;
}

uint32_t BMP085getPressure()
{
  return pressure;
}
*/


