#include "main.h"
#include <stdint.h>
#include <intrinsics.h>
#include "clk.h"
#include "tim4.h"
#include "gpio.h"
#include "exti.h"
#include "itc.h"
#include "spi.h"
char gui[5] = {'s','b','a'};
uint8_t ch = 0;

/* delay ms */
void delay(unsigned long int n);

void main (void) {
  
  
  /* button */
  /* BUTTON SLAVE 1 */
  cbi(GPIOA->DDR, 5); //input mode
  sbi(GPIOA->CR1, 5); //floating(0), pull-up(1)
  cbi(GPIOA->CR2, 5); //interrupt en(1), interrupt dis(0)
  /* BUTTON SLAVE 2 */
  cbi(GPIOA->DDR, 2); //input mode
  sbi(GPIOA->CR1, 2); //floating(0), pull-up(1)
  sbi(GPIOA->CR2, 2); //interrupt en(1), interrupt dis(0)
  /* BUTTON STOP */
  cbi(GPIOA->DDR, 3); //input mode
  sbi(GPIOA->CR1, 3); //floating(0), pull-up(1)
  sbi(GPIOA->CR2, 3); //interrupt en(1), interrupt dis(0)
  /* BUTTON CANCEL */
  cbi(GPIOA->DDR, 4); //input mode
  sbi(GPIOA->CR1, 4); //floating(0), pull-up(1)
  sbi(GPIOA->CR2, 4); //interrupt en(1), interrupt dis(0)
  
   sbi(EXTI->CR2, 3);
  cbi(EXTI->CR2, 2);
  
  sbi(EXTI->CR1, 5);
  cbi(EXTI->CR1, 4);
  
  sbi(EXTI->CR1, 7);
  cbi(EXTI->CR1, 6);
  
  sbi(EXTI->CR2, 1);
  cbi(EXTI->CR2, 0);
  
  /* LED */
  /* LED SLAVE 1 */
  sbi(GPIOB->DDR, 0);
  sbi(GPIOB->CR1, 0);
  sbi(GPIOB->CR2, 0);
  /* LED SLAVE 2 */
  sbi(GPIOB->DDR, 1);
  sbi(GPIOB->CR1, 1);
  sbi(GPIOB->CR2, 1);
  
  
  Clk_Init();
  GPIO_Init();
  
  sbi(GPIOB->DDR, 2);   // PC6 - SCK - OUTPUT
  sbi(GPIOB->CR1, 2);   // push-pull
  sbi(GPIOB->CR2, 2);   // fast mode
  sbi(GPIOB->ODR, 2);   // pull high
  
  sbi(GPIOB->DDR, 3);   // PC6 - SCK - OUTPUT
  sbi(GPIOB->CR1, 3);   // push-pull
  sbi(GPIOB->CR2, 3);   // fast mode
  sbi(GPIOB->ODR, 3);   // pull high
  
  Itc_Init();
  Exti_Init();
  Tim4_Init();
  //LCD_Init();
  SPI_Init(SPI_FirstBit_MSB, SPI_BaudRatePrescaler_8, SPI_Mode_Master, SPI_CPOL_Low, \
           SPI_CPHA_1Edge, SPI_Direction_2Lines_FullDuplex, SPI_NSS_Soft, 0x07);
  SPI->CR3 |= (1<<6);
  SPI_Cmd(ENABLE);
  __enable_interrupt(); //enable global interrupt
  while(1) {
    //cbi(GPIOB->ODR, 3);   // pull low
  }
}


void delay(unsigned long int n) {
  timeCount = 0;
  while (timeCount != n) {};
}

#pragma vector = 11 //9+2
__interrupt void EXTI_Handle_Bit_1 (void) {
  //if(ch>4) ch=0;
  sbi(GPIOB->ODR, 3); 
  cbi(GPIOB->ODR, 2);
  printf("1\n");
  SPI_Transfer(gui[1]);
  sbi(EXTI->SR1, 1); //clear flag by set this bit
}
#pragma vector=27 //25+2
__interrupt void TIM4_UPD_OVF_TRG_IRQHandler(void) {
  timeCount++;
  cbi(TIM4->SR1, 0); //clear update interrupt flag
}
#pragma vector=28 //26+2
__interrupt void SPI_Receive_IRQ(void) {
  uint8_t data_read = SPI->DR;
}


#pragma vector = 15 //9+2
__interrupt void EXTI_Handle_Bit_button_1 (void) {
  cbi(GPIOB->ODR,2);
  printf("1\n");
  SPI_Transfer(gui[1]);
  sbi(GPIOB->ODR,2);
  sbi(EXTI->SR1, 5); //clear flag by set this bit
}

#pragma vector = 12
__interrupt void EXTI_Handle_Bit_button_2 (void) {
  sbi(GPIOB->ODR, 2);   // pull low
  cbi(GPIOB->ODR, 3);   // pull low
  printf("2\n");
  SPI_Transfer(gui[1]);
  sbi(EXTI->SR1, 2); //clear flag by set this bit
}

#pragma vector = 13
__interrupt void EXTI_Handle_Bit_button_stop (void) {
  cbi(GPIOB->ODR, 3);   // pull low
  cbi(GPIOB->ODR, 2);   // pull low
  printf("s\n");
  SPI_Transfer(gui[0]);
  sbi(EXTI->SR1, 3); //clear flag by set this bit
}

#pragma vector = 14
__interrupt void EXTI_Handle_Bit_button_cancel (void) {
  
  sbi(EXTI->SR1, 4); //clear flag by set this bit
}

