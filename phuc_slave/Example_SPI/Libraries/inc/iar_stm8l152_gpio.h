#ifndef _IAR_STM8L152_GPIO_H_
#define _IAR_STM8L152_GPIO_H_

#include "iar_stm8l152.h"

/******************************************************************************/
/*                          Normal Macro of GPIO's Registers                  */
/******************************************************************************/
#define PE_ODR  (*(uint8_t *)0x5014)
#define PE_IDR  (*(uint8_t *)0x5015)
#define PE_DDR  (*(uint8_t *)0x5016)
#define PE_CR1  (*(uint8_t *)0x5017)
#define PE_CR2  (*(uint8_t *)0x5018)

/******************************************************************************/
/*                          bit-field registers structures            	      */
/******************************************************************************/
typedef struct ODR_REGS_struct{
  unsigned char ODR0 :1;		/*!< Bit0 of ODR Register */
  unsigned char ODR1 :1; 		/*!< Bit1 of ODR Register */
  unsigned char ODR2 :1;		/*!< Bit2 of ODR Register */
  unsigned char ODR3 :1;		/*!< Bit3 of ODR Register */
  unsigned char ODR4 :1; 		/*!< Bit4 of ODR Register */
  unsigned char ODR5 :1; 		/*!< Bit5 of ODR Register */
  unsigned char ODR6 :1; 		/*!< Bit6 of ODR Register */
  unsigned char ODR7 :1; 		/*!< Bit7 of ODR Register */
}ODR_REG;

typedef struct IDR_REGS_struct{
  unsigned char IDR0 :1;		/*!< Bit0 of IDR Register */
  unsigned char IDR1 :1; 		/*!< Bit1 of IDR Register */
  unsigned char IDR2 :1;		/*!< Bit2 of IDR Register */
  unsigned char IDR3 :1;		/*!< Bit3 of IDR Register */
  unsigned char IDR4 :1; 		/*!< Bit4 of IDR Register */
  unsigned char IDR5 :1; 		/*!< Bit5 of IDR Register */
  unsigned char IDR6 :1; 		/*!< Bit6 of IDR Register */
  unsigned char IDR7 :1; 		/*!< Bit7 of IDR Register */
}IDR_REG;

typedef struct DDR_REGS_struct{
  unsigned char DDR0 :1;		/*!< Bit0 of DDR Register */
  unsigned char DDR1 :1; 		/*!< Bit1 of DDR Register */
  unsigned char DDR2 :1;		/*!< Bit2 of DDR Register */
  unsigned char DDR3 :1;		/*!< Bit3 of DDR Register */
  unsigned char DDR4 :1; 		/*!< Bit4 of DDR Register */
  unsigned char DDR5 :1; 		/*!< Bit5 of DDR Register */
  unsigned char DDR6 :1; 		/*!< Bit6 of DDR Register */
  unsigned char DDR7 :1; 		/*!< Bit7 of DDR Register */
}DDR_REG;

typedef struct CR1_REGS_struct{
  unsigned char CR10 :1;		/*!< Bit0 of CR1 Register */
  unsigned char CR11 :1; 		/*!< Bit1 of CR1 Register */
  unsigned char CR12 :1;		/*!< Bit2 of CR1 Register */
  unsigned char CR13 :1;		/*!< Bit3 of CR1 Register */
  unsigned char CR14 :1; 		/*!< Bit4 of CR1 Register */
  unsigned char CR15 :1; 		/*!< Bit5 of CR1 Register */
  unsigned char CR16 :1; 		/*!< Bit6 of CR1 Register */
  unsigned char CR17 :1; 		/*!< Bit7 of CR1 Register */
}CR1_REG;

typedef struct CR2_REGS_struct{
  unsigned char CR20 :1;		/*!< Bit0 of CR2 Register */
  unsigned char CR21 :1; 		/*!< Bit1 of CR2 Register */
  unsigned char CR22 :1;		/*!< Bit2 of CR2 Register */
  unsigned char CR23 :1;		/*!< Bit3 of CR2 Register */
  unsigned char CR24 :1; 		/*!< Bit4 of CR2 Register */
  unsigned char CR25 :1; 		/*!< Bit5 of CR2 Register */
  unsigned char CR26 :1; 		/*!< Bit6 of CR2 Register */
  unsigned char CR27 :1; 		/*!< Bit7 of CR2 Register */
}CR2_REG;

typedef struct GPIO_struct_BitField{
  ODR_REG ODR;		/*!< Output Data Register */
  IDR_REG IDR; 		/*!< Input Data Register */
  DDR_REG DDR;		/*!< Data Direction Register */
  CR1_REG CR1;		/*!< Configuration Register 1 */
  CR2_REG CR2; 		/*!< Configuration Register 2 */
}GPIO_TypeDef_BitField;

/******************************************************************************/
/*                          GPIO-Bit-Field declarations                       */
/******************************************************************************/

#define GPIOA_BF                    	((GPIO_TypeDef_BitField *) 0x5000)
#define GPIOB_BF                    	((GPIO_TypeDef_BitField *) 0x5005)
#define GPIOC_BF                     	((GPIO_TypeDef_BitField *) 0x500A)
#define GPIOD_BF                     	((GPIO_TypeDef_BitField *) 0x500F)
#define GPIOE_BF                     	((GPIO_TypeDef_BitField *) 0x5014)
#define GPIOF_BF                     	((GPIO_TypeDef_BitField *) 0x5019)
#define GPIOG_BF                     	((GPIO_TypeDef_BitField *) 0x501E)
#define GPIOH_BF                     	((GPIO_TypeDef_BitField *) 0x5023)
#define GPIOI_BF                     	((GPIO_TypeDef_BitField *) 0x5028)

/******************************************************************************/
/*                          Define for User				      */
/******************************************************************************/
/* Config PC7 (BLUE LED) */
void config_Bled();
/* Config PE7 (GREEN LED) */
void config_Gled();
/* Config User Button - PC1 */
void config_Ubtn();

#endif /* _IAR_STM8L152_GPIO_H_ */