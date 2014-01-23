#ifndef MMA8452Q_H
#define MMA8452Q_H

#define MMA8452Q_DEFAULT_ADDRESS        0x1C  // MMA8452Q Default address
#define MMA8452Q_WHO_AM_I								0x0D  // Device ID (0x2A)

#define MMA8452Q_STATUS									0x00  // Real time status
#define MMA8452Q_SYSMOD									0x0B  // Current system mode
#define MMA8452Q_WAKE_MOD								0x01  // Set to Wake MOD
#define MMA8452Q_XYZ_DATA_CFG						0x0E  // HPF Data Out and Dynamic Range Settings
#define MMA8452Q_RANGE_2G_NHPF					0x00  // Range 2G, no high pass filtering

#define MMA8452Q_CTRL_REG_1             0x2A  // Data Rate, ACTIVE MODE
#define MMA8452Q_CTRL_REG_2					    0x2B  // Sleep Enable, OS Modes, RST, ST
#define MMA8452Q_CTRL_REG_3					    0x2C  // Wake from Sleep, IPOL, PP_OD
#define MMA8452Q_CTRL_REG_4					    0x2D  // Interrupt enable register
#define MMA8452Q_CTRL_REG_5					    0x2E  // Interrupt pin (INT1/INT2) map

// measurement registers
#define MMA8452Q_OUT_X_MSB						  0x01  // Measurement of X [7:0]
#define MMA8452Q_OUT_Y_MSB						  0x03  // Measurement of Y [7:0]
#define MMA8452Q_OUT_Z_MSB						  0x05  // Measurement of Z [7:0]
#define MMA8452Q_OUT_X_LSB						  0x02  // Measurement of X [7:4]
#define MMA8452Q_OUT_Y_LSB						  0x04  // Measurement of Y [7:4]
#define MMA8452Q_OUT_Z_LSB						  0x06  // Measurement of Z [7:4]

// offset registers
#define MMA8452Q_OFF_X    						  0x2F  // X offset
#define MMA8452Q_OFF_Y				    		  0x30  // Y offset
#define MMA8452Q_OFF_Z				    		  0x31  // Z offset

// prototypes
uint32_t MMA8452QgetInit(void);
uint32_t MMA8452QgetVersion(void);
uint32_t MMA8452QgetMotion(uint16_t* motions);

#endif /* MMA8452Q_H */
