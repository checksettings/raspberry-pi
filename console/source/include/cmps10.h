#ifndef CMPS10_H
#define CMPS10_H

#define CMPS10_DEFAULT_ADDRESS               0xC0

// Measure read commands
#define CMPS10_VERSION_REGISTER              0x00 //
#define CMPS10_BEARING_REGISTER              0x01 // Compass Bearing as a byte, i.e. 0-255 for a full circle 
#define CMPS10_BEARING_HIGH_REGISTER         0x02 // Compass Bearing as a word, i.e. 0-3599 for a full circle, representing 0-359.9 degrees.
#define CMPS10_BEARING_LOW_REGISTER          0x03

#define CMPS10_PITCH_REGISTER                0x04 // Pitch angle - signed byte giving angle in degrees from the horizontal plane
#define CMPS10_ROLL_REGISTER                 0x05 // Roll angle - signed byte giving angle in degrees from the horizontal plane

#define CMPS10_MAGNETOMETER_X_HIGH_REGISTER  0x0A // Magnetometer X axis raw output, 16 bit signed integer with register 10 being the upper 8 bits
#define CMPS10_MAGNETOMETER_X_LOW_REGISTER   0x0B
#define CMPS10_MAGNETOMETER_Y_HIGH_REGISTER  0x0C // Magnetometer Y axis raw output, 16 bit signed integer with register 12 being the upper 8 bits
#define CMPS10_MAGNETOMETER_Y_LOW_REGISTER   0x0D
#define CMPS10_MAGNETOMETER_Z_HIGH_REGISTER  0x0E // Magnetometer Z axis raw output, 16 bit signed integer with register 14 being the upper 8 bits
#define CMPS10_MAGNETOMETER_Z_LOW_REGISTER   0x0F
#define CMPS10_ACCELEROMETER_X_HIGH_REGISTER 0x10 // Accelerometer  X axis raw output, 16 bit signed integer with register 16 being the upper 8 bits
#define CMPS10_ACCELEROMETER_X_LOW_REGISTER  0x11
#define CMPS10_ACCELEROMETER_Y_HIGH_REGISTER 0x12 // Accelerometer  Y axis raw output, 16 bit signed integer with register 18 being the upper 8 bits
#define CMPS10_ACCELEROMETER_Y_LOW_REGISTER  0x13
#define CMPS10_ACCELEROMETER_Z_HIGH_REGISTER 0x14 // Accelerometer  Z axis raw output, 16 bit signed integer with register 20 being the upper 8 bits
#define CMPS10_ACCELEROMETER_Z_LOW_REGISTER  0x15

// Send command register
#define CMPS10_COMMAND_REGISTER  0x16 // 

#define CMPS10_WAITING                       0xF0000

void cmps10Main();

#endif /* CMPS10_H */
