#include "main.h"
uint32_t cnt = 'A';
uint8_t actived = 0;
uint8_t data_read = 0;

void main(void){
  config_Led_Port();
  //config_Gled();
  //config_Bled();
  //config_Ubtn();
  //config_LCDvsRTC();
  config_spi(SPI_Mode_Slave, SPI_Direction_2Lines_FullDuplex, SPI_NSS_Soft);
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
