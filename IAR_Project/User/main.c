#include <stdint.h>
#include <intrinsics.h>
#include "clk.h"
#include "tim4.h"
#include "gpio.h"
#include "exti.h"
#include "itc.h"
#include "spi.h"

Spi_ChannelConfigType spiChannel[SPI_MAX_CHANNEL+1];
Spi_JobConfigType spiJob[SPI_MAX_JOB+1];
Spi_SequenceConfigType spiSequence[SPI_MAX_SEQUENCE+1];
Spi_ConfigType spiDriver;

Spi_DataBufferType spiBuffer[4][4];

void Handle_Job_Finish_1 (void);
void Handle_Job_Finish_2 (void);
void SPI_Config(void);

/* delay ms */
void delay(unsigned long int n);

void main (void) {
  Clk_Init();
  GPIO_Init();
  Itc_Init();
  Exti_Init();
  Tim4_Init();
  /* SPI Driver - suppose they are configed in advance*/
  SPI_Config();
  Spi_Init(&spiDriver);
  __enable_interrupt(); //enable global interrupt
  spiBuffer[0][3]=(uint8)'1';
  spiBuffer[1][3]=(uint8)'a';
  spiBuffer[2][3]=(uint8)'4';
  spiBuffer[3][3]=(uint8)'b';
  while(1) {
  }
}


void delay(unsigned long int n) {
  timeCount = 0;
  while (timeCount != n) {};
}
uint8_t count = 1;
#pragma vector = 11 //9+2
__interrupt void EXTI_Handle_Bit_1 (void) {
  if(count > 4) count = 1;
  Spi_WriteIB(spiDriver.SpiChannel[count++].SpiChannelId, NULL);
  sbi(EXTI->SR1, 1); //clear flag by set this bit
}
#pragma vector=27 //25+2
__interrupt void TIM4_UPD_OVF_TRG_IRQHandler(void) {
  timeCount++;
  cbi(TIM4->SR1, 0); //clear update interrupt flag
}

void Handle_Job_Finish_1 (void) {
  /* Do something */
}
void Handle_Job_Finish_2 (void) {
  /* Do something */
}

void SPI_Config (void) {
        spiDriver.Max_Channel = SPI_MAX_CHANNEL;
        spiDriver.Max_Job = SPI_MAX_JOB;
        spiDriver.Max_Sequence = SPI_MAX_SEQUENCE;
        spiDriver.SpiChannel=spiChannel;
        spiDriver.SpiJob=spiJob;
        spiDriver.SpiSequence=spiSequence;
	/* channel id */
	/* job 0 */
	spiDriver.SpiChannel[1].SpiChannelId=1; //dinh danh
	spiDriver.SpiChannel[2].SpiChannelId=2; //254 buffer phia sau thi ke me no
	/* job 1 */
	spiDriver.SpiChannel[3].SpiChannelId=3;
	spiDriver.SpiChannel[4].SpiChannelId=4;

	/* data width */
	/* job 0 */
	spiDriver.SpiChannel[1].SpiDataWidth = 1;
	spiDriver.SpiChannel[2].SpiDataWidth = 1;
	/* job 1 */
	spiDriver.SpiChannel[3].SpiDataWidth = 1;
	spiDriver.SpiChannel[4].SpiDataWidth = 1;
	/* number of buffer each channel */
	/* job 0 */
	spiDriver.SpiChannel[1].SpiIbNBuffers = 4;
	spiDriver.SpiChannel[2].SpiIbNBuffers = 4;
	/* job 1 */
	spiDriver.SpiChannel[3].SpiIbNBuffers = 4;
	spiDriver.SpiChannel[4].SpiIbNBuffers = 4;

	/* fixed address buffer base */
	spiDriver.SpiChannel[1].SpiBufferBase = spiBuffer[0];
	spiDriver.SpiChannel[2].SpiBufferBase = spiBuffer[1];
	spiDriver.SpiChannel[3].SpiBufferBase = spiBuffer[2];
	spiDriver.SpiChannel[4].SpiBufferBase = spiBuffer[3];

	/* default transmit value (32bits) */
	spiDriver.SpiChannel[1].SpiDefaultData = 0x0001;
	spiDriver.SpiChannel[2].SpiDefaultData = 0x0000;
	/* job 1 */
	spiDriver.SpiChannel[3].SpiDefaultData = 0x0002;
	spiDriver.SpiChannel[4].SpiDefaultData = 0x0000;

	/* job's id */
	spiDriver.SpiJob[1].SpiJobId=1;
	spiDriver.SpiJob[2].SpiJobId=2;
	/* chip select pin */
	spiDriver.SpiJob[1].CS_pin = 0;
	spiDriver.SpiJob[2].CS_pin = 1;
	/* port chip select */
	spiDriver.SpiJob[1].GPIO_port = GPIOA;
	spiDriver.SpiJob[2].GPIO_port = GPIOA;
	/* priority of each job */
	spiDriver.SpiJob[1].SpiJobPriority = 0;
	spiDriver.SpiJob[2].SpiJobPriority = 1;
	/* handler for job's finishing */
	spiDriver.SpiJob[1].SpiJobEndNotification = &Handle_Job_Finish_1;
	spiDriver.SpiJob[2].SpiJobEndNotification = &Handle_Job_Finish_2;
	//muon goi ham thi (*(spiJob[0]->SpiJobEndNotification)) ()
	/* fixed link of channel */
	/* 1,3 2,4 is channel's id */
	spiDriver.SpiJob[1].ChannelLink[1]=1;
	spiDriver.SpiJob[2].ChannelLink[1]=3;
	for(uint16 i=2; i<=255; i++) {
          spiDriver.SpiJob[1].ChannelLink[i]=2;
          spiDriver.SpiJob[2].ChannelLink[i]=4;
	}
	/*SPI sequence
	* by default:
	* - we have only 1 sequence
	* - 2 job in this sequence
	*/
	/* sequence's id */
	spiDriver.SpiSequence[1].SpiSequenceId=1;
	/* number of spi's job */
	spiDriver.SpiSequence[1].JobNum = 2;
	/* job target (0 is defined as none job)*/
	spiDriver.SpiSequence[1].JobLink[1] = 0;
	spiDriver.SpiSequence[1].JobLink[2] = 0;
}
