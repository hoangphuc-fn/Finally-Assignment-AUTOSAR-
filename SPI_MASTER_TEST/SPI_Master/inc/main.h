#ifndef __MAIN_H
#define __MAIN_H
#include <stdint.h>
#include <intrinsics.h>
#include "clk.h"
#include "tim4.h"
#include "lcd.h"
#include "gpio.h"
#include "exti.h"
#include "itc.h"
#include "spi.h"
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
  
  /* LED */
  /* LED SLAVE 1 */
  sbi(GPIOB->DDR, 0);
  sbi(GPIOB->CR1, 0);
  sbi(GPIOB->CR2, 0);
  /* LED SLAVE 2 */
  sbi(GPIOB->DDR, 1);
  sbi(GPIOB->CR1, 1);
  sbi(GPIOB->CR2, 1);

#endif