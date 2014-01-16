#ifndef MAG3110_H
#define MAG3110_H

#define MAG3110_DEFAULT_ADDRESS       0x0E  // MAG3110 Default address
#define MAG3110_WHO_AM_I							0x07  // Device ID number

// Control Registers
#define MAG3110_DR_STATUS             0x00  // Data ready status per axis 
#define MAG3110_CTRL_REG_1            0x10  // Operation modes
#define MAG3110_CTRL_REG_2						0x11  // Operation modes

// measurement registers
#define MAG3110_OUT_X_MSB						  0x01  // Measurement of X [15:8]
#define MAG3110_OUT_Y_MSB						  0x03  // Measurement of Y [15:8]
#define MAG3110_OUT_Z_MSB						  0x05  // Measurement of Z [15:8]
#define MAG3110_OUT_X_LSB						  0x02  // Measurement of X [7:0]
#define MAG3110_OUT_Y_LSB						  0x04  // Measurement of Y [7:0]
#define MAG3110_OUT_Z_LSB						  0x06  // Measurement of Z [7:0]

// offset registers
#define MAG3110_OFF_X_MSB						  0x09  // Bits [14:7] of user X offset
#define MAG3110_OFF_Y_MSB						  0x0B  // Bits [14:7] of user Y offset
#define MAG3110_OFF_Z_MSB						  0x0D  // Bits [14:7] of user Z offset
#define MAG3110_OFF_X_LSB						  0x0A  // Bits [6:0] of user X offset
#define MAG3110_OFF_Y_LSB						  0x0C  // Bits [6:0] of user Y offset
#define MAG3110_OFF_Z_LSB						  0x0E  // Bits [6:0] of user Z offset

// additionl registers
#define MAG3110_TEMPERATURE           0x0F  // Temperature, signed 8 bits in C

// prototypes
uint32_t MAG3110getInit(void);
uint32_t MAG3110getVersion(void);
uint32_t MAG3110getDirection(uint16_t* directions);
uint32_t MAG3110getTemperature(void);

#endif /* MAG3110_H */
