#ifndef I2C_H
#define I2C_H

#include <stdint.h>

// Basic
#define mmio32_2(x)               (*(volatile uint32_t*) (x))

#define BCM2835_PERI_BASE       0x20000000
#define BCM2835_BSC0_BASE       (BCM2835_PERI_BASE + 0x205000)
#define BCM2835_BSC1_BASE       (BCM2835_PERI_BASE + 0x804000)

// Defines for I2C
// GPIO register offsets from BCM2835_BSC*_BASE.
#define BCM2835_BSC_C           0x0000 ///< BSC Master Control
#define BCM2835_BSC_S           0x0004 ///< BSC Master Status
#define BCM2835_BSC_DLEN        0x0008 ///< BSC Master Data Length
#define BCM2835_BSC_A           0x000c ///< BSC Master Slave Address
#define BCM2835_BSC_FIFO        0x0010 ///< BSC Master Data FIFO
#define BCM2835_BSC_DIV         0x0014 ///< BSC Master Clock Divider
#define BCM2835_BSC_DEL         0x0018 ///< BSC Master Data Delay
#define BCM2835_BSC_CLKT        0x001c ///< BSC Master Clock Stretch Timeout

// Register masks for BSC_C
#define BCM2835_BSC_C_I2CEN     0x00008000 ///< I2C Enable, 0 = disabled, 1 = enabled
#define BCM2835_BSC_C_INTR      0x00000400 ///< Interrupt on RX
#define BCM2835_BSC_C_INTT      0x00000200 ///< Interrupt on TX
#define BCM2835_BSC_C_INTD      0x00000100 ///< Interrupt on DONE
#define BCM2835_BSC_C_ST        0x00000080 ///< Start transfer, 1 = Start a new transfer
#define BCM2835_BSC_C_CLEAR_1   0x00000020 ///< Clear FIFO Clear
#define BCM2835_BSC_C_CLEAR_2   0x00000010 ///< Clear FIFO Clear
#define BCM2835_BSC_C_READ      0x00000001 ///<	Read transfer

// Register masks for BSC_S
#define BCM2835_BSC_S_CLKT      0x00000200 ///< Clock stretch timeout
#define BCM2835_BSC_S_ERR       0x00000100 ///< ACK error
#define BCM2835_BSC_S_RXF       0x00000080 ///< RXF FIFO full, 0 = FIFO is not full, 1 = FIFO is full
#define BCM2835_BSC_S_TXE       0x00000040 ///< TXE FIFO full, 0 = FIFO is not full, 1 = FIFO is full
#define BCM2835_BSC_S_RXD       0x00000020 ///< RXD FIFO contains data
#define BCM2835_BSC_S_TXD       0x00000010 ///< TXD FIFO can accept data
#define BCM2835_BSC_S_RXR       0x00000008 ///< RXR FIFO needs reading (full)
#define BCM2835_BSC_S_TXW       0x00000004 ///< TXW FIFO needs writing (full)
#define BCM2835_BSC_S_DONE      0x00000002 ///< Transfer DONE
#define BCM2835_BSC_S_TA        0x00000001 ///< Transfer Active

#define BCM2835_BSC_FIFO_SIZE   16 ///< BSC FIFO size

// GPIO PINS
#define GPIO_PIN_0        0
#define GPIO_PIN_1        3
#define GPIO_PIN_2        6
#define GPIO_PIN_3        9
#define GPIO_PIN_16       18

// GPIO Usages
#define GPIO_INPUT        0b000
#define GPIO_OUTPUT       0b001
#define GPIO_ALTERNATIVE0 0b100

#define GPFSEL0_OFFS   0
#define GPFSEL1_OFFS   4

#define GPSET0_OFFS    28
#define GPCLR0_OFFS    40
#define GPIO_PIN_OFFS  16
#define GPIO_FSEL_ADDR (BCM2835_PERI_BASE + 0x200000)

// Additional Defines
#define WAIT_DELAY_2      0x3F0000


/// Specifies the reason codes for the bcm2835_i2c_write and bcm2835_i2c_read functions.
typedef enum
{
    BCM2835_I2C_REASON_OK   	       = 0x00,      ///< Success
    BCM2835_I2C_REASON_ERROR_NACK    = 0x01,      ///< Received a NACK
    BCM2835_I2C_REASON_ERROR_CLKT    = 0x02,      ///< Received Clock Stretch Timeout
    BCM2835_I2C_REASON_ERROR_DATA    = 0x04,      ///< Not all data is sent / received
} bcm2835I2CReasonCodes;

// LED

extern void i2cInit(void);

extern void i2cInit(void);
extern void i2cClose(void);
extern uint8_t i2cRead(uint8_t address, uint32_t bytes, char *data);
extern uint8_t i2cReadWithRegister(uint8_t address, const char *reg_address, uint32_t bytes, char *data);
extern uint8_t i2cWrite(uint8_t address, uint32_t bytes, const char *data);

#endif	/* I2C_H */
