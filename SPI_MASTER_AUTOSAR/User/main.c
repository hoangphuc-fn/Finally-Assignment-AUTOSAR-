#include <stdint.h>
#include <intrinsics.h>
#include "clk.h"
#include "tim4.h"
#include "gpio.h"
#include "exti.h"
#include "itc.h"
#include "spi.h"
#include "mymacro.h"

#define SLAVE_1_JOB 1
#define SLAVE_2_JOB 2

uint8 bt1_flag = DISABLE;
uint8 bt2_flag = DISABLE;
uint8 btstop_flag = DISABLE;

Spi_ChannelConfigType spiChannel[SPI_MAX_CHANNEL+1];
Spi_JobConfigType spiJob[SPI_MAX_JOB+1];
Spi_SequenceConfigType spiSequence[SPI_MAX_SEQUENCE+1];

Spi_DataBufferType spiBuffer[4][4];

void Print_Job_Status (Spi_JobResultType jobStatus);
void Handle_Job_Finish_1 (void);
void Handle_Job_Finish_2 (void);
void SPI_Config(void);

Spi_BufferType user_buffer[4];

void main (void) {
  Clk_Init();
  GPIO_Init();
  //Itc_Init();
  Exti_Init();
  Tim4_Init();
  /* SPI Driver - suppose they are configed in advance*/
  SPI_Config();
  Spi_Init(&spiDriver);
  __enable_interrupt(); //enable global interrupt
  while(1) {
    
    if(bt1_flag) {
      user_buffer[3]='b'; //['b',0,0,0]
      user_buffer[2]='b';
      user_buffer[1]='b';
      user_buffer[0]='b';
    
      Spi_WriteIB(4, user_buffer);
      //Spi_WriteIB(2, user_buffer);
      spiDriver.SpiSequence[1].JobLink[1] = SLAVE_1_JOB;
      spiDriver.SpiSequence[1].JobLink[2] = SLAVE_2_JOB;
      spiDriver.SpiSequence[1].JobNum = 2;
      Spi_AsyncTransmit(1);
      
      bt1_flag = DISABLE;
    }

    if(bt2_flag) {
      
      user_buffer[3]='b'; //['b',0,0,0]
      user_buffer[2]='b';
      user_buffer[1]='b';
      user_buffer[0]='b';
    
      //Spi_WriteIB(4, user_buffer);
      Spi_WriteIB(2, user_buffer);
      spiDriver.SpiSequence[1].JobLink[1] = SLAVE_1_JOB;
      spiDriver.SpiSequence[1].JobLink[2] = SLAVE_2_JOB;
      spiDriver.SpiSequence[1].JobNum = 2;
      Spi_AsyncTransmit(1);
      bt2_flag = DISABLE;
    }

    if(btstop_flag) {
      user_buffer[3]='s'; //['b',0,0,0]
      user_buffer[2]='s';
      user_buffer[1]='s';
      user_buffer[0]='s';
    
      Spi_WriteIB(4, user_buffer);
      Spi_WriteIB(2, user_buffer);
      spiDriver.SpiSequence[1].JobLink[1] = SLAVE_1_JOB;
      spiDriver.SpiSequence[1].JobLink[2] = SLAVE_2_JOB;
      spiDriver.SpiSequence[1].JobNum = 2;
      Spi_AsyncTransmit(1);
      
      btstop_flag = DISABLE;
    }
  }
}

#pragma vector = 11 //9+2
__interrupt void EXTI_Handle_Bit_button_1 (void) {
  
  bt1_flag=ENABLE;
  
  sbi(EXTI->SR1, 1); //clear flag by set this bit
}

#pragma vector = 12
__interrupt void EXTI_Handle_Bit_button_2 (void) {
  if(bt1_flag==ENABLE) {
    Spi_Job_Cancel(SLAVE_1_JOB);
    bt2_flag=ENABLE;
  }
  if(Spi_GetStatus() == SPI_IDLE) {
    bt2_flag=ENABLE;
  }
  sbi(EXTI->SR1, 2); //clear flag by set this bit
}

#pragma vector = 13
__interrupt void EXTI_Handle_Bit_button_stop (void) {
  if(Spi_GetStatus() == SPI_IDLE) {
    btstop_flag=ENABLE;
  }
  sbi(EXTI->SR1, 3); //clear flag by set this bit
}

#pragma vector = 14
__interrupt void EXTI_Handle_Bit_button_cancel (void) {
  sbi(EXTI->SR1, 4); //clear flag by set this bit
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
  spiDriver.SpiJob[1].CS_pin = 2;
  spiDriver.SpiJob[2].CS_pin = 3;
  /* port chip select */
  spiDriver.SpiJob[1].GPIO_port = GPIOB;
  spiDriver.SpiJob[2].GPIO_port = GPIOB;
  /* priority of each job */
  spiDriver.SpiJob[1].SpiJobPriority = 0;
  spiDriver.SpiJob[2].SpiJobPriority = 1;
  /* handler for job's finishing */
  spiDriver.SpiJob[1].SpiJobEndNotification = &Handle_Job_Finish_1;
  spiDriver.SpiJob[2].SpiJobEndNotification = &Handle_Job_Finish_2;
  //muon goi ham thi (*(spiJob[0]->SpiJobEndNotification)) ()
  /* fixed link of channel */
  /* 1,3 2,4 is channel's id */
  for(uint16 i=1; i<=254; i++) {
    spiDriver.SpiJob[1].ChannelLink[i]=1;
    spiDriver.SpiJob[2].ChannelLink[i]=3;
  }
  spiDriver.SpiJob[1].ChannelLink[255]=2;
  spiDriver.SpiJob[2].ChannelLink[255]=4;
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

