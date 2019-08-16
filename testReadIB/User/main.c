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

Spi_DataBufferType spiBuffer[4][4];

void Print_Job_Status (Spi_JobResultType jobStatus);
void Handle_Job_Finish_1 (void);
void Handle_Job_Finish_2 (void);
void SPI_Config(void);

/* delay ms */
void delay(unsigned long int n);

Spi_BufferType user_buffer[4];

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
  while(1) {
  }
}


void delay(unsigned long int n) {
  timeCount = 0;
  while (timeCount != n) {};
}

#pragma vector = 11 //9+2
__interrupt void EXTI_Handle_Bit_button_1 (void) {
  if(Spi_GetStatus() == SPI_IDLE) {
    user_buffer[0]='b'; //['b',0,0,0]
    
    Spi_WriteIB(1, user_buffer);
    spiDriver.SpiSequence[1].JobLink[1] = 1;
    spiDriver.SpiSequence[1].JobNum = 1;
    Spi_AsyncTransmit(spiDriver.SpiSequence[1].SpiSequenceId);
    
    Spi_ReadIB(2, user_buffer);
  }
  sbi(EXTI->SR1, 1); //clear flag by set this bit
}

#pragma vector = 10 //8+2
__interrupt void EXTI_Handle_Bit_button_2 (void) {
  sbi(EXTI->SR1, 0); //clear flag by set this bit
}

#pragma vector=27 //25+2
__interrupt void TIM4_UPD_OVF_TRG_IRQHandler(void) {
  timeCount++;
  cbi(TIM4->SR1, 0); //clear update interrupt flag
}

void Print_Job_Status (Spi_JobResultType jobStatus) {
  switch(jobStatus) {
  case SPI_JOB_OK:
    printf("SPI_JOB_OK\n");
    break;
  case SPI_JOB_PENDING:
    printf("SPI_JOB_PENDING\n");
    break;
  case SPI_JOB_FAILED:
    printf("SPI_JOB_FAILED\n");
    break;
  case SPI_JOB_QUEUED:
    printf("SPI_JOB_QUEUED\n");
    break;
  }
}

void Handle_Job_Finish_1 (void) {
  printf("Job 1: ");
  Print_Job_Status(Spi_GetJobResult(1));
}
void Handle_Job_Finish_2 (void) {
  printf("Job 2: ");
  Print_Job_Status(Spi_GetJobResult(2));
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
  spiDriver.SpiChannel[1].SpiDefaultData = 0x00000001;
  spiDriver.SpiChannel[2].SpiDefaultData = 0x0000FF01;
  /* job 1 */
  spiDriver.SpiChannel[3].SpiDefaultData = 0x00000002;
  spiDriver.SpiChannel[4].SpiDefaultData = 0x0000FF02;

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
  spiDriver.SpiSequence[1].JobLink[1] = 1;
  spiDriver.SpiSequence[1].JobLink[2] = 2;
}

