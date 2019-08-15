#include "main.h"
extern uint32_t cnt;
extern uint8_t data_read;
extern uint8_t actived;

#pragma vector=2+26
__interrupt void Read_SPI(void){
  /*!< count number of bytes read */
  cnt ++;
  data_read = read_byte_spi(SPI1);
  /*!< if button 1 or button 2 on MASTER are pressed */
  if(data_read == 'b'){
    actived = 1;
  }
  /*!< if STOP_BUTTON on MASTER is pressed */
  else if(data_read == 's'){
    actived = 0;
  }
  if(cnt == 1019){ /*!< 255*4(byte) - 1(byte) */
    #ifdef SLAVE_1 /*!< define SLAVE_1 in main.h file */
      write_byte_spi(SPI1,'1');
    #else          /*!< define SLAVE_2 */
      write_byte_spi(SPI1,'2');
    #endif
  }
  else if (cnt > 1019){
    cnt = 0;
    write_byte_spi(SPI1,'0');
  }
  else{
    write_byte_spi(SPI1,'0');
  }
}