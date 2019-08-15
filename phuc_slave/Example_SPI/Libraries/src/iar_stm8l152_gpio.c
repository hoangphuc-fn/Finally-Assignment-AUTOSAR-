#include "iar_stm8l152_gpio.h"

/******************************************************************************/
/*                          Define for User				      */
/******************************************************************************/
/* Config PC7 (BLUE LED) */
void config_Bled(){
  sbi(GPIOC->DDR,7);  			/*!< output pin */
  sbi(GPIOC->CR1,7);  			/*!< push-pull mode */
  sbi(GPIOC->CR2,7);  			/*!< fast speed */
  sbi(GPIOC->ODR,7);  			/*!< turn on led */
}

/* Config PE7 (GREEN LED) */
void config_Gled(){
  sbi(GPIOE->DDR,7);  			/*!< output pin */
  sbi(GPIOE->CR1,7);  			/*!< push-pull mode */
  sbi(GPIOE->CR2,7);  			/*!< fast speed */
  cbi(GPIOE->ODR,7);  			/*!< turn off led */
}

/* Config PC1 (USER BUTTON) */
void config_Ubtn(){
  // user button
  cbi(GPIOC->DDR, 1);                   /*!< PC1 is an input pin */
  cbi(GPIOC->CR1, 1);                   /*!< floating */
  sbi(GPIOC->CR2, 1);                   /*!< interrupt enable */
  EXTI->CR1 |= (1<<3);                  /*!< Falling edge interrupt */
}