#include "iar_stm8l152_usart.h"

void config_usart(){
  /*!< system clock = 16MHz */
  CLK->CKDIVR = 0x00;
  /*!< enable usart clock */
  sbi(CLK->PCKENR1, 5);
  /*!< Config RX pin - PC2 */
  cbi(GPIOC->DDR, 2);                   /*!< input pin */
  sbi(GPIOC->CR1, 2);                   /*!< pull-up */
  cbi(GPIOC->CR2, 2);                   /*!< disable interrupt */
  /*!< Config TX pin - PC3 */
  sbi(GPIOC->DDR,3);  			/*!< output pin */
  sbi(GPIOC->CR1,3);  			/*!< push-pull mode */
  sbi(GPIOC->CR2,3);  			/*!< fast speed */
  sbi(GPIOC->ODR,3);  			/*!< output high */
  
  /*!< 1 start bit, 8 data bits */
  cbi(USART1->CR1, 4);
  /*!< disable prity */
  cbi(USART1->CR1, 2);
  /*!< 1 stop bit */
  cbi(USART1->CR3, 5);
  cbi(USART1->CR3, 4);
  /*!< 9600 baudrate - 16MHz SysClock */
  USART1->BRR2 = 0x03;
  USART1->BRR1 = 0x68;
  /*!< enable receive */
  sbi(USART1->CR2,2);
  /*!< enable transmit */
  sbi(USART1->CR2,3);
  /*!< enable receive interrupt */
  sbi(USART1->CR2,5);
  /*!< enable usart */
  cbi(USART1->CR1, 5);
}

void usart_send_byte(char data_byte){
    USART1->DR = data_byte;
    while(!gbi(USART1->SR,7));
}

void usart_send_string(char* data_str){
  while(*data_str){
    usart_send_byte(*data_str);
    data_str++;
  }
}
