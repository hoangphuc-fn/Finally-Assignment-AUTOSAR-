#ifndef _IAR_STM8L152_CLK_H_
#define _IAR_STM8L152_CLK_H_

#include "iar_stm8l152.h"

/******************************************************************************/
/*                          Define group CLK_SYSCLKDiv            	      */
/******************************************************************************/
typedef enum{
  CLK_SYSCLKDiv_1   = (uint8_t)0x00, 	/*!< System Clock Divider: 1 */
  CLK_SYSCLKDiv_2   = (uint8_t)0x01, 	/*!< System Clock Divider: 2 */
  CLK_SYSCLKDiv_4   = (uint8_t)0x02, 	/*!< System Clock Divider: 4 */
  CLK_SYSCLKDiv_8   = (uint8_t)0x03, 	/*!< System Clock Divider: 8 */
  CLK_SYSCLKDiv_16  = (uint8_t)0x04, 	/*!< System Clock Divider: 16 */
  CLK_SYSCLKDiv_32  = (uint8_t)0x05, 	/*!< System Clock Divider: 32 */
  CLK_SYSCLKDiv_64  = (uint8_t)0x06, 	/*!< System Clock Divider: 64 */
  CLK_SYSCLKDiv_128 = (uint8_t)0x07  	/*!< System Clock Divider: 128 */
} CLK_SYSCLKDiv_TypeDef;

/******************************************************************************/
/*                          Define group CLK_SYSCLKSource            	      */
/******************************************************************************/
typedef enum{
  CLK_SYSCLKSource_HSI = (uint8_t)0x01, /*!< System Clock Source HSI */
  CLK_SYSCLKSource_LSI = (uint8_t)0x02, /*!< System Clock Source LSI */
  CLK_SYSCLKSource_HSE = (uint8_t)0x04, /*!< System Clock Source HSE */
  CLK_SYSCLKSource_LSE = (uint8_t)0x08  /*!< System Clock Source LSE */
} CLK_SYSCLKSource_TypeDef;

/******************************************************************************/
/*                          Define for User				      */
/******************************************************************************/
/*!< Config HSI as System Clock */
void config_HSI_clock(CLK_SYSCLKDiv_TypeDef CLK_SYSCLKDiv);
/*!< Config LSE as System Clock */
void config_LSE_clock(CLK_SYSCLKDiv_TypeDef CLK_SYSCLKDiv);

#endif	/* _IAR_STM8L152_CLK_H_ */