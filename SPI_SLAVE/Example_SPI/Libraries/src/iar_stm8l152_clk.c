#include "iar_stm8l152_clk.h"

/*!< Config HSI as System Clock */
void config_HSI_clock(CLK_SYSCLKDiv_TypeDef CLK_SYSCLKDiv){ 
  /*!<  HSI selected as system clock source */
  CLK->SWR = CLK_SYSCLKSource_HSI;
  /*!<  High-speed internal RC ON */
  CLK->ICKCR |= (1<<0);	
  /*!<  Wait until HSI clock ready */ 
  while(!gbi(CLK->ICKCR,1));  	
  /*!<  Enable clock switch execution */
  CLK->SWCR |= (1<<1);			
  /*!<  Wait until HSI is chosen as SYSCLK Source */ 
  while(CLK->SCSR != CLK_SYSCLKSource_HSI);
  /*!< System clock source/CLK_SYSCLKDiv */
  CLK->CKDIVR |= CLK_SYSCLKDiv;   	
}

/*!< Config LSE as System Clock */
void config_LSE_clock(CLK_SYSCLKDiv_TypeDef CLK_SYSCLKDiv){
  /*!<  LSE selected as system clock source */
  CLK->SWR = CLK_SYSCLKSource_LSE;	
  /*!<  Wait until LSE clock ready */
  while(!gbi(CLK->ECKCR,3));  		
  /*!<  Enable clock switch execution */ 
  CLK->SWCR |= (1<<1);			
  /*!<  Wait until HSI is chosen as SYSCLK Source */ 
  while(CLK->SCSR != CLK_SYSCLKSource_LSE);
  /*!< System clock source/CLK_SYSCLKDiv */
  CLK->CKDIVR |= CLK_SYSCLKDiv;   	
}