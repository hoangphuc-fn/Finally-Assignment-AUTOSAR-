#include "clk.h"

void Clk_Init(void) {
  /*  Config System clock  */
  System_Clock->CKDIVR = (uint8_t)CLK_SYSCLKDiv_1; // pre = 1
  System_Clock->SWCR |= (uint8_t)0x02; // enable switch clock source
  System_Clock->SWR = (uint8_t)CLK_SYSCLKSource_HSI; //switch to HSI clock
  while (System_Clock->SCSR != CLK_SYSCLKSource_HSI) {} //wait util completed change to HSI
  /* Tim4 clock */
  System_Clock->PCKENR1 |= (1<<2); //enable clock for tim4
  /* SPI clock */
  System_Clock->PCKENR1 |= (1<<4); //enable clock for SPI
}