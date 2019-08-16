#ifndef _IAR_STM8L152_H_
#define _IAR_STM8L152_H_
#include <stdint.h>


/******************************************************************************/
/*                          Macro to Control Bit              	              */
/******************************************************************************/
#define sbi(sfr,bit)   (sfr |=(1<<bit))		/*!< Set bits value to 1 */
#define cbi(sfr,bit)   (sfr &=~(1<<bit))	/*!< Clear bits value to 0 */
#define tbi(sfr,bit)   (sfr ^=(1<<bit))		/*!< Toggle bits value */
#define gbi(sfr,bit)   (sfr &(1<<bit))		/*!< Get bits value */

/******************************************************************************/
/*                          GPIO registers structures            	      */
/******************************************************************************/
typedef struct GPIO_struct{
  volatile unsigned char ODR;		/*!< Output Data Register */
  volatile unsigned char IDR; 		/*!< Input Data Register */
  volatile unsigned char DDR;		/*!< Data Direction Register */
  volatile unsigned char CR1;		/*!< Configuration Register 1 */
  volatile unsigned char CR2; 		/*!< Configuration Register 2 */
}GPIO_TypeDef;

/******************************************************************************/
/*                          EXTI registers structures            	      */
/******************************************************************************/
typedef struct EXTI_struct{
  volatile unsigned char CR1;           /*!<  The four LSB EXTI  pin sensitivity */
  volatile unsigned char CR2;           /*!<  The four MSB EXTI  pin sensitivity */
  volatile unsigned char CR3;           /*!<  EXTI port B & port D sensitivity */
  volatile unsigned char SR1;           /*!<  Pins Status flag register 1 */
  volatile unsigned char SR2;           /*!<  Ports Status flage register 2 */
  volatile unsigned char CONF1;         /*!<  Port interrupt selector */
  volatile unsigned char RESERVED[4];  	/*!<  reserved area */
  volatile unsigned char CR4;           /*!<  EXTI port G & port H sensitivity */
  volatile unsigned char CONF2;         /*!<  Port interrupt selector */
}EXTI_TypeDef;
 
/******************************************************************************/
/*                          CLOCK registers structures            	      */
/******************************************************************************/
typedef struct CLK_struct{
  volatile unsigned char CKDIVR;        /*!< Clock Master Divider Register */
  volatile unsigned char CRTCR;         /*!< RTC Clock selection Register */
  volatile unsigned char ICKCR;         /*!< Internal Clocks Control Register */
  volatile unsigned char PCKENR1;       /*!< Peripheral Clock Gating Register 1 */
  volatile unsigned char PCKENR2;       /*!< Peripheral Clock Gating Register 2 */
  volatile unsigned char CCOR;          /*!< Configurable Clock Output Register */
  volatile unsigned char ECKCR;         /*!< External Clocks Control Register */
  volatile unsigned char SCSR;          /*!< System clock status Register */
  volatile unsigned char SWR;           /*!< System clock Switch Register */
  volatile unsigned char SWCR;          /*!< Switch Control Register */
  volatile unsigned char CSSR;          /*!< Clock Security Sytem Register */
  volatile unsigned char CBEEPR;        /*!< Clock BEEP Register */
  volatile unsigned char HSICALR;       /*!< HSI Calibration Register */
  volatile unsigned char HSITRIMR;      /*!< HSI clock Calibration Trimmer Register */
  volatile unsigned char HSIUNLCKR;     /*!< HSI Unlock  Register */
  volatile unsigned char REGCSR;        /*!< Main regulator control status register */
  volatile unsigned char PCKENR3;       /*!< Peripheral Clock Gating Register 3 */
}CLK_TypeDef;
          
/******************************************************************************/
/*                          LCD registers structures            	      */
/******************************************************************************/
typedef struct LCD_struct{
  volatile unsigned char CR1;      	/*!< LCD control register 1 */
  volatile unsigned char CR2;      	/*!< LCD control register 2 */
  volatile unsigned char CR3;      	/*!< LCD control register 3 */
  volatile unsigned char FRQ;      	/*!< LCD frequency register */
  volatile unsigned char PM[6];    	/*!< LCD portmask registers*/
  volatile unsigned char RESERVED1[2];  /*!< Reserved */
  volatile unsigned char RAM[22];  	/*!< LCD RAM registers*/
  volatile unsigned char RESERVED2[13]; /*!< Reserved */
  volatile unsigned char CR4;      	/*!< LCD control register 4 */
}LCD_TypeDef;

/******************************************************************************/
/*                          USART registers structures            	      */
/******************************************************************************/
typedef struct USART_struct{
  volatile unsigned char SR;   		/*!<  USART status register  */
  volatile unsigned char DR;   		/*!<  USART data register     */
  volatile unsigned char BRR1; 		/*!<  USART baud rate register   */
  volatile unsigned char BRR2; 		/*!<  USART DIV mantissa[11:8] SCIDIV fraction */
  volatile unsigned char CR1;  		/*!<  USART control register 1     */
  volatile unsigned char CR2;  		/*!<  USART control register 2     */
  volatile unsigned char CR3;  		/*!<  USART control register 3      */
  volatile unsigned char CR4;  		/*!< USART control register 4      */
  volatile unsigned char CR5;  		/*!<  USART control register 5       */
  volatile unsigned char GTR;  		/*!<  USART guard time register     */
  volatile unsigned char PSCR; 		/*!<  USART prescaler register     */
}USART_TypeDef;

/******************************************************************************/
/*                          SPI registers structures            	      */
/******************************************************************************/
typedef struct SPI_struct
{
 volatile unsigned char CR1;    /*!< SPI control register 1 */
 volatile unsigned char CR2;    /*!< SPI control register 2 */
 volatile unsigned char ICR;    /*!< SPI DMA and interrupt control register */
 volatile unsigned char SR;     /*!< SPI status register */
 volatile unsigned char DR;     /*!< SPI data I/O register */
 volatile unsigned char CRCPR;  /*!< SPI CRC polynomial register */
 volatile unsigned char RXCRCR; /*!< SPI Rx CRC register */
 volatile unsigned char TXCRCR; /*!< SPI Tx CRC register */
}
SPI_TypeDef;
          
/******************************************************************************/
/*                          Periphral declarations                            */
/******************************************************************************/

#define GPIOA                     	((GPIO_TypeDef *) 0x5000)
#define GPIOB                     	((GPIO_TypeDef *) 0x5005)
#define GPIOC                     	((GPIO_TypeDef *) 0x500A)
#define GPIOD                     	((GPIO_TypeDef *) 0x500F)
#define GPIOE                     	((GPIO_TypeDef *) 0x5014)
#define GPIOF                     	((GPIO_TypeDef *) 0x5019)
#define GPIOG                     	((GPIO_TypeDef *) 0x501E)
#define GPIOH                     	((GPIO_TypeDef *) 0x5023)
#define GPIOI                     	((GPIO_TypeDef *) 0x5028)

#define EXTI                            ((EXTI_TypeDef *) 0x50A0)

#define CLK                             ((CLK_TypeDef *) 0x50C0)

#define LCD                             ((LCD_TypeDef *) 0x5400)

#define USART1                      	((USART_TypeDef *) 0x5230)

#define SPI1                  	      	((SPI_TypeDef *) 0x5200)


#endif /* _IAR_STM8L152_H_ */