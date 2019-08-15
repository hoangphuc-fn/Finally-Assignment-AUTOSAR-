#ifndef _IAR_STM8L152_USART_H_
#define _IAR_STM8L152_USART_H_

#include "iar_stm8l152.h"

/******************************************************************************/
/*                          Define for User				      */
/******************************************************************************/

/*!< Config & Init USART */
void config_usart();
/*!< Send a byte through USART */
void usart_send_byte(char data_byte);
/*!< Send a string through USART */
void usart_send_string(char* data_str);

#endif	/* _IAR_STM8L152_USART_H_ */