#include "main.h"

uint32_t cnt = 0;
uint8_t actived = 0;
uint8_t data_read = 0;

void main(void){
  config_Led_Port();
  config_Gled();
  config_spi(SPI_Mode_Slave, SPI_Direction_2Lines_FullDuplex, SPI_NSS_Hard);
  __enable_interrupt();
  while(1){
    if(actived){
      shift_led_effect();
    }
    else{
      turn_off_all_led();
    }
  }
}
