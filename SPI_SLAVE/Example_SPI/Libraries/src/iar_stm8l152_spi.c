#include "iar_stm8l152_spi.h"


void config_spi(SPI_Mode_TypeDef SPI_Mode, SPI_DirectionMode_TypeDef SPI_Direction_Mode, SPI_NSS_TypeDef NSS_Mode){
  /* Set HSI System Clock divider to 1 */
  CLK->CKDIVR = 0x00; 
  //---------------------------------- CR1 -----------------------------------//
  
  /* enable SPI1 clock */
  sbi(CLK->PCKENR1,4);  // enable SPI1 clock
  /* SPI_BaudRatePrescaler_8*/
  SPI1->CR1 |= SPI_BaudRatePrescaler_8;
  /* CPOL = 0*/
  SPI1->CR1 |= SPI_CPOL_Low;  
  /* CPHA = 0 */
  SPI1->CR1 |= SPI_CPHA_1Edge;
  /* MSB first*/
  SPI1->CR1 |= SPI_FirstBit_MSB;
    //---------------------------------- CR2 -----------------------------------//
  /* Config Direction Mode */
  SPI1->CR2 |= SPI_Direction_Mode;
  /* Config NSS Mode */
  SPI1->CR2 |= NSS_Mode;
  /* Config SPI Mode*/
  SPI1->CR1 |= SPI_Mode;
  /* ENABLE RXNE */
    SPI1->ICR |= (1<<6);
  //-------------------------------- SLAVE MODE -------------------------------//
  if(SPI_Mode == SPI_Mode_Slave){
    /* Config ISS Mode */
    SPI1->CR2 |= 0x00;
    //-------------------------------- SPI PIN ---------------------------------//
    cbi(GPIOB->DDR, 6);   // PB6 - MOSI - INPUT
    sbi(GPIOB->CR1, 6);   // pull-up
    cbi(GPIOB->CR2, 6);   // disable interrupt 
    
    sbi(GPIOB->DDR, 7);   // PB7 - MISO - OUTPUT
    sbi(GPIOB->CR1, 7);   // Push - pull
    sbi(GPIOB->CR2, 7);   // Fast mode
    
    cbi(GPIOB->DDR, 5);   // PB5 - SCK - INPUT
    sbi(GPIOB->CR1, 5);   // pull-up
    cbi(GPIOB->CR2, 5);   // disable interrupt 
    
    cbi(GPIOB->DDR, 4);   // PB4 - NSS - INPUT
    sbi(GPIOB->CR1, 4);   // Pull-up
    sbi(GPIOB->CR2, 4);   // Enable interrupt
    EXTI->CR2 |= (1<<1);  // Falling edge interrupt
  }
  //-------------------------------- MASTER MODE -------------------------------//
  else if(SPI_Mode == SPI_Mode_Master){
    /* Config ISS Mode */
    SPI1->CR2 |= 0x01;
    //-------------------------------- SPI PIN ---------------------------------//
    sbi(GPIOB->DDR, 6);   // PB6 - MOSI - OUTPUT
    sbi(GPIOB->CR1, 6);   // push-pull
    sbi(GPIOB->CR2, 6);   // fast mode
    
    cbi(GPIOB->DDR, 7);   // PB7 - MISO - INPUT
    sbi(GPIOB->CR1, 7);   // Pull-up
    cbi(GPIOB->CR2, 7);   // Disable interrupt
    
    sbi(GPIOB->DDR, 5);   // PB5 - SCK - OUTPUT
    sbi(GPIOB->CR1, 5);   // push-pull
    sbi(GPIOB->CR2, 5);   // fast mode
    cbi(GPIOB->ODR, 5);   // push down SCK
    
    sbi(GPIOB->DDR, 4);   // PB4 - NSS - OUTPUT
    sbi(GPIOB->CR1, 4);   // push - pull
    sbi(GPIOB->CR2, 4);   // Fast mode
    sbi(GPIOB->ODR, 4);   // pull high
  }
  /* enable SPI */
  SPI1->CR1 |= (1<<6);
}

void write_byte_spi(SPI_TypeDef *SPIx,unsigned char data){
  SPIx->DR = data;
  //while(gbi(SPIx->SR,1));
}

unsigned char read_byte_spi(SPI_TypeDef *SPIx){
  return  SPIx->DR ;
}