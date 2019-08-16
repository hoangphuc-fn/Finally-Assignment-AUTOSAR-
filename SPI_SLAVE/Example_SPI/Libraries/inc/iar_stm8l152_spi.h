#ifndef _IAR_STM8L152_SPI_H_
#define _IAR_STM8L152_SPI_H_

#include "iar_stm8l152.h"

typedef enum {
  SPI_Direction_2Lines_FullDuplex = (uint8_t)0x00, /*!< 2-line uni-directional data mode enable */
  SPI_Direction_2Lines_RxOnly     = (uint8_t)0x04, /*!< Receiver only in 2 line uni-directional data mode */
  SPI_Direction_1Line_Rx          = (uint8_t)0x80, /*!< Receiver only in 1 line bi-directional data mode */
  SPI_Direction_1Line_Tx          = (uint8_t)0xC0  /*!< Transmit only in 1 line bi-directional data mode */
} SPI_DirectionMode_TypeDef;

typedef enum{
  SPI_NSS_Soft  = (uint8_t)0x02, /*!< Software slave management disabled */
  SPI_NSS_Hard  = (uint8_t)0x00  /*!< Software slave management enabled */
} SPI_NSS_TypeDef;

typedef enum {
  SPI_Direction_Rx = (uint8_t)0x00, /*!< Select Rx receive direction in bi-directional mode */
  SPI_Direction_Tx = (uint8_t)0x01  /*!< Select Tx transmission direction in bi-directional mode */
} SPI_Direction_TypeDef;

typedef enum{
  SPI_Mode_Master = (uint8_t)0x04, /*!< SPI Master configuration */
  SPI_Mode_Slave  = (uint8_t)0x00  /*!< SPI Slave configuration */
} SPI_Mode_TypeDef;

typedef enum {
  SPI_BaudRatePrescaler_2   = (uint8_t)0x00, /*!< SPI frequency = frequency(CPU)/2 */
  SPI_BaudRatePrescaler_4   = (uint8_t)0x08, /*!< SPI frequency = frequency(CPU)/4 */
  SPI_BaudRatePrescaler_8   = (uint8_t)0x10, /*!< SPI frequency = frequency(CPU)/8 */
  SPI_BaudRatePrescaler_16  = (uint8_t)0x18, /*!< SPI frequency = frequency(CPU)/16 */
  SPI_BaudRatePrescaler_32  = (uint8_t)0x20, /*!< SPI frequency = frequency(CPU)/32 */
  SPI_BaudRatePrescaler_64  = (uint8_t)0x28, /*!< SPI frequency = frequency(CPU)/64 */
  SPI_BaudRatePrescaler_128 = (uint8_t)0x30, /*!< SPI frequency = frequency(CPU)/128 */
  SPI_BaudRatePrescaler_256 = (uint8_t)0x38  /*!< SPI frequency = frequency(CPU)/256 */
} SPI_BaudRatePrescaler_TypeDef;

typedef enum {
  SPI_CPOL_Low  = (uint8_t)0x00, /*!< Clock to 0 when idle */
  SPI_CPOL_High = (uint8_t)0x02  /*!< Clock to 1 when idle */
} SPI_CPOL_TypeDef;

typedef enum {
  SPI_CPHA_1Edge = (uint8_t)0x00, /*!< The first clock transition is the first data capture edge */
  SPI_CPHA_2Edge = (uint8_t)0x01  /*!< The second clock transition is the first data capture edge */
} SPI_CPHA_TypeDef;

typedef enum {
  SPI_FirstBit_MSB = (uint8_t)0x00, /*!< MSB bit will be transmitted first */
  SPI_FirstBit_LSB = (uint8_t)0x80  /*!< LSB bit will be transmitted first */
} SPI_FirstBit_TypeDef;


/******************************************************************************/
/*                          Define for User				      */
/******************************************************************************/
/*!< Config SPI & CLOCK & SPI pin */
void config_spi(SPI_Mode_TypeDef SPI_Mode, SPI_DirectionMode_TypeDef SPI_Direction_Mode,SPI_NSS_TypeDef NSS_Mode);
/*!< write a byte data through SPIx */
void write_byte_spi(SPI_TypeDef *SPIx,unsigned char data);
/*!< read a byte data from SPIx */
unsigned char read_byte_spi(SPI_TypeDef *SPIx);

#endif	/* _IAR_STM8L152_SPI_H_ */