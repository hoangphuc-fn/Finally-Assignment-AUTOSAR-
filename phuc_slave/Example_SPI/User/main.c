#include "main.h"
uint32_t cnt = 'A';

void delay(uint32_t time);
uint8_t data_read = 0;

void main(void){
  config_Gled();
  config_Bled();
  config_Ubtn();
  config_LCDvsRTC();
  config_spi(SPI_Mode_Slave, SPI_Direction_2Lines_FullDuplex, SPI_NSS_Soft);
  __enable_interrupt();
  while(1){
  }
}
void delay(uint32_t time){
  while(time--);
}
