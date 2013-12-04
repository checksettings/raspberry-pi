#ifndef SRF08_H
#define SRF08_H

#define SRF08_DEFAULT_ADDRESS         0xE0 // SRF08 Default address
#define SRF08_MEASURE_INCH            0x50 // Measure start command (inch)
#define SRF08_MEASURE_CM              0x51 // Measure start command (centimeter)
#define SRF08_MEASURE_US              0x52 // Measure start command (microsecondes)
#define SRF08_ANN_MEASURE_INCH        0x53 // ANN-Mode Measure start command (inch)
#define SRF08_ANN_MEASURE_CM          0x54 // ANN-Mode Measure start command (centimeter)
#define SRF08_ANN_MEASURE_US          0x55 // ANN-Mode Measure start command (microsecondes)

// Register for sending commands
#define SRF08_COMMAND_REGISTER        0x00 // Register for writing the measure command to start measure process

// Register for reading commands
#define SRF08_VERSION_REGISTER        0x00 // Register for reading the software version of sensor
#define SRF08_BRIGHTNESS_REGISTER     0x01 // Register for reading the brightness value
#define SRF08_DISTANCE_HIGH_REGISTER  0x02 // Register for reading the high-byte of last measured distance
#define SRF08_DISTANCE_LOW_REGISTER   0x03 // Register for reading the low-byte of last measured distance

// Register for ANN-Mode reading
#define SRF08_ANN01_HIGH_REGISTER     0x02 // Register for reading the high-byte of 1. ANN measure
#define SRF08_ANN01_LOW_REGISTER      0x03 // Register for reading the low-byte of 1. ANN measure
#define SRF08_ANN02_HIGH_REGISTER     0x04 // Register for reading the high-byte of 2. ANN measure
#define SRF08_ANN02_LOW_REGISTER      0x05 // Register for reading the low-byte of 2. ANN measure
#define SRF08_ANN03_HIGH_REGISTER     0x06 // Register for reading the high-byte of 3. ANN measure
#define SRF08_ANN03_LOW_REGISTER      0x07 // Register for reading the low-byte of 3. ANN measure
#define SRF08_ANN04_HIGH_REGISTER     0x08 // Register for reading the high-byte of 4. ANN measure
#define SRF08_ANN04_LOW_REGISTER      0x09 // Register for reading the low-byte of 4. ANN measure
#define SRF08_ANN05_HIGH_REGISTER     0x0A // Register for reading the high-byte of 5. ANN measure
#define SRF08_ANN05_LOW_REGISTER      0x0B // Register for reading the low-byte of 5. ANN measure
#define SRF08_ANN06_HIGH_REGISTER     0x0C // Register for reading the high-byte of 6. ANN measure
#define SRF08_ANN06_LOW_REGISTER      0x0D // Register for reading the low-byte of 6. ANN measure
#define SRF08_ANN07_HIGH_REGISTER     0x0E // Register for reading the high-byte of 7. ANN measure
#define SRF08_ANN07_LOW_REGISTER      0x0F // Register for reading the low-byte of 7. ANN measure
#define SRF08_ANN08_HIGH_REGISTER     0x10 // Register for reading the high-byte of 8. ANN measure
#define SRF08_ANN08_LOW_REGISTER      0x11 // Register for reading the low-byte of 8. ANN measure
#define SRF08_ANN09_HIGH_REGISTER     0x12 // Register for reading the high-byte of 9. ANN measure
#define SRF08_ANN09_LOW_REGISTER      0x13 // Register for reading the low-byte of 9. ANN measure
#define SRF08_ANN10_HIGH_REGISTER     0x14 // Register for reading the high-byte of 10. ANN measure
#define SRF08_ANN10_LOW_REGISTER      0x15 // Register for reading the low-byte of 10. ANN measure
#define SRF08_ANN11_HIGH_REGISTER     0x16 // Register for reading the high-byte of 11. ANN measure
#define SRF08_ANN11_LOW_REGISTER      0x17 // Register for reading the low-byte of 11. ANN measure
#define SRF08_ANN12_HIGH_REGISTER     0x18 // Register for reading the high-byte of 12. ANN measure
#define SRF08_ANN12_LOW_REGISTER      0x19 // Register for reading the low-byte of 12. ANN measure
#define SRF08_ANN13_HIGH_REGISTER     0x1A // Register for reading the high-byte of 13. ANN measure
#define SRF08_ANN13_LOW_REGISTER      0x1B // Register for reading the low-byte of 13. ANN measure
#define SRF08_ANN14_HIGH_REGISTER     0x1C // Register for reading the high-byte of 14. ANN measure
#define SRF08_ANN14_LOW_REGISTER      0x1D // Register for reading the low-byte of 14. ANN measure
#define SRF08_ANN15_HIGH_REGISTER     0x1E // Register for reading the high-byte of 15. ANN measure
#define SRF08_ANN15_LOW_REGISTER      0x1F // Register for reading the low-byte of 15. ANN measure
#define SRF08_ANN16_HIGH_REGISTER     0x20 // Register for reading the high-byte of 16. ANN measure
#define SRF08_ANN16_LOW_REGISTER      0x21 // Register for reading the low-byte of 16. ANN measure
#define SRF08_ANN17_HIGH_REGISTER     0x22 // Register for reading the high-byte of 17. ANN measure
#define SRF08_ANN17_LOW_REGISTER      0x23 // Register for reading the low-byte of 17. ANN measure

#define SRF08_ANN_NUM                 17   // Number of NNA

#define SRF08_WAITING                 0xF0000 // Time to wait between measurements 65 ms

void srf08Main();

#endif /* SRF08_H */
