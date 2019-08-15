#include "iar_stm8l152_gpio.h"

/******************************************************************************/
/*                          Define for User				      */
/******************************************************************************/
/* Delay function using while loop */
void delay(uint32_t time){
  while(time--);
}
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
void config_4btns(){
  // PA1 - BUTTON 1
  cbi(GPIOA->DDR, 1);                   /*!< PA1 is an input pin */
  sbi(GPIOA->CR1, 1);                   /*!< Pull-up */
  sbi(GPIOA->CR2, 1);                   /*!< interrupt enable */
  EXTI->CR1 |= (1<<3);                  /*!< Falling edge interrupt */
  // PA2 - BUTTON 2
  cbi(GPIOA->DDR, 2);                   /*!< PA2 is an input pin */
  sbi(GPIOA->CR1, 2);                   /*!< Pull-up */
  sbi(GPIOA->CR2, 2);                   /*!< interrupt enable */
  EXTI->CR1 |= (1<<5);                  /*!< Falling edge interrupt */
  // PA3 - BUTTON 3
  cbi(GPIOA->DDR, 3);                   /*!< PA3is an input pin */
  sbi(GPIOA->CR1, 3);                   /*!< Pull-up */
  sbi(GPIOA->CR2, 3);                   /*!< interrupt enable */
  EXTI->CR1 |= (1<<7);                  /*!< Falling edge interrupt */
  // PA4 - BUTTON 4
  cbi(GPIOA->DDR, 4);                   /*!< PA4 is an input pin */
  sbi(GPIOA->CR1, 4);                   /*!< Pull-up */
  sbi(GPIOA->CR2, 4);                   /*!< interrupt enable */
  EXTI->CR2 |= (1<<1);                  /*!< Falling edge interrupt */
}

void config_Led_Port(){
  GPIOC->DDR = 0xFF;                    /*!< PC is an output port */
  GPIOC->CR1 = 0xFF;                    /*!< PC: push pull mode */
  GPIOC->CR2 = 0xFF;                    /*!< PC: fast mode */
  GPIOC->ODR = 0xFF;                    /*!< PC: turn off port */
}
/* Turn off led function */
void turn_off_all_led(){
   GPIOC->ODR = 0xFF;
}
/* Shift LED Effect */
void shift_led_effect(){
  /*!< Shift Led effect */
  for(uint8_t i = 0; i < 8; i++){
    cbi(GPIOC->ODR,i);
    delay(5000);
  }
  /*!< Turn off all leds */
  turn_off_all_led();
}