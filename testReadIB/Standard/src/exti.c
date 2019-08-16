#include "exti.h"

void Exti_Init() {
  /* mode exti 
  * portc bit 1 selected
  * falling edge
  */
  sbi(EXTI->CR1, 3);
  cbi(EXTI->CR1, 2);
  
  sbi(EXTI->CR1, 5);
  cbi(EXTI->CR1, 4);
  
  sbi(EXTI->CR1, 7);
  cbi(EXTI->CR1, 6);
  
  sbi(EXTI->CR2, 1);
  cbi(EXTI->CR2, 0);
}