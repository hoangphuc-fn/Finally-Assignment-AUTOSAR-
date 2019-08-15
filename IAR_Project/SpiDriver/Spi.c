#include "Spi.h"

Spi_ChannelConfigType spiChannel[SPI_MAX_CHANNEL+1];
Spi_JobConfigType spiJob[SPI_MAX_JOB+1];
Spi_ConfigType spiDriver;

void SPI_HW_Init(SPI_FirstBit_TypeDef SPI_FirstBit,
              SPI_BaudRatePrescaler_TypeDef SPI_BaudRatePrescaler,
              SPI_Mode_TypeDef SPI_Mode, SPI_CPOL_TypeDef SPI_CPOL,
              SPI_CPHA_TypeDef SPI_CPHA, SPI_DirectionMode_TypeDef SPI_Data_Direction,
              SPI_NSS_TypeDef SPI_Slave_Management, uint8 CRCPolynomial)
{
  /* Frame Format, BaudRate, Clock Polarity and Phase configuration */
  SPI->CR1 = (uint8)((uint8)((uint8)SPI_FirstBit |
                                  (uint8)SPI_BaudRatePrescaler) |
                        (uint8)((uint8)SPI_CPOL |
                                  SPI_CPHA));

  /* Data direction configuration: BDM, BDOE and RXONLY bits */
  SPI->CR2 = (uint8)((uint8)(SPI_Data_Direction) | (uint8)(SPI_Slave_Management));

  if (SPI_Mode == SPI_Mode_Master)
  {
    SPI->CR2 |= (uint8)(1<<0);
  }
  else
  {
    SPI->CR2 &= (uint8)~(1<<0);
  }

  /* Master/Slave mode configuration */
  SPI->CR1 |= (uint8)(SPI_Mode);

  /* CRC configuration */
  SPI->CRCPR = (uint8)CRCPolynomial;
}

void SPI_HW_Cmd(uint8 NewState)
{
  if (NewState != DISABLE)
  {
    SPI->CR1 |= (1<<6); /* Enable the SPI peripheral*/
  }
  else
  {
    SPI->CR1 &= (uint8)(~(1<<6)); /* Disable the SPI peripheral*/
  }
}

void Control_CSN (Spi_JobType jobID, GPIO_State state) {
	/* control state of csn */
	if (state == LOW) {
		GPIO_Write_Bit(spiJob[jobID].GPIO_port, spiJob[jobID].CS_pin, 0);
	} else {
		GPIO_Write_Bit(spiJob[jobID].GPIO_port, spiJob[jobID].CS_pin, 1);
	}
}

void Handle_Job_Finish_1 (void) {
	/* do something */
}

void Handle_Job_Finish_2 (void) {
	/* do something */
}

void Spi_Init(const Spi_ConfigType* ConfigPtr) {
	/* 
	* - for each channel must config
	* - config master or slave
	* - transmit data width
	* - by default
	*  + all channel use the same properties
	*  + we have 32bits for transaction of each channel -> data width = 32 bits
	*  + MSB/LSB, default value = {MSB, 0x00}
	*/
	SPI_HW_Init(SPI_FirstBit_MSB, SPI_BaudRatePrescaler_8, SPI_Mode_Master, SPI_CPOL_Low, \
           SPI_CPHA_1Edge, SPI_Direction_2Lines_FullDuplex, SPI_NSS_Hard, 0x07);

	/* channel id */
	/* job 0 */
	spiChannel[1].SpiChannelId=1; //dinh danh
	spiChannel[2].SpiChannelId=2; //254 buffer phia sau thi ke me no
	/* job 1 */
	spiChannel[3].SpiChannelId=3;
	spiChannel[4].SpiChannelId=4;

	/* data width */
	/* job 0 */
	spiChannel[1].SpiDataWidth = 1;
	spiChannel[2].SpiDataWidth = 1;
	/* job 1 */
	spiChannel[3].SpiDataWidth = 1;
	spiChannel[4].SpiDataWidth = 1;
	/* number of buffer each channel */
	/* job 0 */
	spiChannel[1].SpiIbNBuffers = 4;
	spiChannel[2].SpiIbNBuffers = 4;
	/* job 1 */
	spiChannel[3].SpiIbNBuffers = 4;
	spiChannel[4].SpiIbNBuffers = 4;

	/* fixed address buffer base */
	spiChannel[1].SpiBufferBase = NULL;
	spiChannel[2].SpiBufferBase = NULL;
	spiChannel[3].SpiBufferBase = NULL;
	spiChannel[4].SpiBufferBase = NULL;

	/* default transmit value (32bits) */
	spiChannel[1].SpiDefaultData = 0x0001;
	spiChannel[2].SpiDefaultData = 0x0000;
	/* job 1 */
	spiChannel[3].SpiDefaultData = 0x0002;
	spiChannel[4].SpiDefaultData = 0x0000;

	/* job's id */
	spiJob[1].SpiJobId=1;
	spiJob[2].SpiJobId=2;
	/* chip select pin */
	spiJob[1].CS_pin = 0;
	spiJob[2].CS_pin = 1;
	/* port chip select */
	spiJob[1].GPIO_port = 0;
	spiJob[2].GPIO_port = 1;
	/* priority of each job */
	spiJob[1].JobPriority = 0;
	spiJob[2].JobPriority = 1;
	/* handler for job's finishing */
	spiJob[1].SpiJobEndNotification = &Handle_Job_Finish_1;
	spiJob[2].SpiJobEndNotification = &Handle_Job_Finish_2;
	//muon goi ham thi (*(spiJob[0]->SpiJobEndNotification)) ()
	/* fixed link of channel */
	/* 1,3 2,4 is channel's id */
	spiJob[1].ChannelLink[1]=1;
	spiJob[2].ChannelLink[1]=3;
	for(uint8 i=2; i<=255; i++) {
		spiJob[1].ChannelLink[i]=2;
		spiJob[2].ChannelLink[i]=4;
	}
	/*SPI sequence
	* by default:
	* - we have only 1 sequence
	* - 2 job in this sequence
	*/
	/* sequence's id */
	spiSequence.SpiSequenceId=1;
	/* number of spi's job */
	spiSequence.JobNum = 2;
	/* job target (0 is defined as none job)*/
	spiSequence.JobLink[1] = 0;
	spiSequence.JobLink[2] = 0;

	/* SPI Driver */
	spiDriver.SpiChannelConfig=spiChannel;
	spiDriver.SpiJobConfig=spiJob;
	spiDriver.SpiSequenceConfig=&spiSequence;
	/* set status */
	setJobStatus(spiJob[1], SPI_JOB_OK);
	setJobStatus(spiJob[2], SPI_JOB_OK);
	setSquenceStatus(spiSequence, SPI_SEQ_OK);

	/* HW enable */
	SPI_HW_Cmd(ENABLE);
	setDriverStatus(SPI_IDLE);
}

Std_ReturnType Spi_DeInit(void) {
	SPI_HW_Cmd(DISABLE);
	setDriverStatus(SPI_UNINIT);
	return E_OK;
}

void Spi_HW_Transaction(uint8 sendData, uint8 *readData, uint8 CopiData) {
	if(CopiData==ENABLE) {
		//goi ham truyen va gan gia tri cho data
		*readData = SPI_HW_Send(sendData);
	} else {
		//chi goi ham truyen
		SPI_HW_Send(sendData);
	}
}

Std_ReturnType Spi_Transaction(Spi_ChannelType Channel,const Spi_DataBufferType* DataBufferPtr,uint8 CopiData) {
	uint8 i = 1;
	for(i=1; i <= spiDriver.Max_Channel; i++) {
		if(spiDriver.SpiChannel[i].Channel_ID == Channel) {
			/* neu nguoi dung truyen vao databuffer */
			if(DataBufferPtr != NULL) {
				for(uint8 iData=0; iData < spiDriver.SpiChannel[i].SpiIbNBuffers; iData++) {
					//goi ham truyen spi 8 bit
					//DataBufferPtr[iData]
					Spi_HW_Transaction(DataBufferPtr[iData], &DataBufferPtr[iData], CopiData);
				}
			/* neu nguoi dung khong truyen vao */
			} else {
				/* neu vung nho mac dinh khac null -> truyen gia tri cua vung nho do*/
				if(spiDriver.SpiChannel[i].SpiBufferBase != NULL) {
					for(uint8 iData=0; iData < spiDriver.SpiChannel[i].SpiIbNBuffers; iData++) {
						//goi ham truyen spi 8 bit
						//spiDriver->SpiChannel[i]->SpiBufferBase[iData]
						Spi_HW_Transaction(spiDriver.SpiChannel[i].SpiBufferBase[iData], 
							&spiDriver.SpiChannel[i].SpiBufferBase[iData], CopiData);
					}
				/* neu vung nho mac dinh khong duoc cap -> truyen gia tri mac dinh*/
				} else {
					/* neu cho phep ghi du lieu nhung buffer duoc cap de ghi = null -> error */
					if(CopiData == ENABLE) { 
						return E_NOT_OK; 
					}
					for(uint8 iData=0; iData < spiDriver.SpiChannel[i].SpiIbNBuffers; iData++) {
						//goi ham truyen spi 8 bit
						//spiDriver->SpiChannel[i]->SpiDefaultData
						Spi_HW_Transaction(spiDriver.SpiChannel[i].SpiDefaultData, 
							&spiDriver.SpiChannel[i].SpiBufferBase[iData] , CopiData);
					}
				}
			}
			break;
		}
	}
	//dont match any channel's id
	if(i>spiDriver.Max_Channel) {
		return E_NOT_OK;
	} else {
		return E_OK;
	}
}

Std_ReturnType Spi_WriteIB(Spi_ChannelType Channel,const Spi_DataBufferType* DataBufferPtr) {
	return Spi_Transaction(Channel, DataBufferPtr, DISABLE);
}

Std_ReturnType Spi_ReadIB(Spi_ChannelType Channel,const Spi_DataBufferType* DataBufferPtr) {
	return Spi_Transaction(Channel, DataBufferPtr, ENABLE);
}

/*
Std_ReturnType Spi_AsyncTransmit(Spi_SequenceType Sequence) {
	
}
*/
