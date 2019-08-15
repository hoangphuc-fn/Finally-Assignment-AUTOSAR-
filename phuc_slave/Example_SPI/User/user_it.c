#include "main.h"
extern uint32_t cnt;
extern uint8_t data_read;

#pragma vector=2+9
__interrupt void EXTI_Line_1(void){
    if(!gbi(GPIOC->IDR,1)){
    cnt ++;
    tbi(GPIOE->ODR,7);
  }
  sbi(EXTI->SR1,1);
}

#pragma vector=2+26
__interrupt void Read_SPI(void){
  tbi(GPIOC->ODR,7);
  /*!< print data send to MASTER device at 3rd position */
  lcd_print_char(data_read,3);
  data_read = read_byte_spi(SPI1);
  /*!< print data read from MASTER device at 1st position */
  if(data_read != 0) {
    lcd_print_char(data_read,1);
  }
  write_byte_spi(SPI1,data_read);
}