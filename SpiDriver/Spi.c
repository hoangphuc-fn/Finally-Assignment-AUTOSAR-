void SPI_HW_Init(SPI_FirstBit_TypeDef SPI_FirstBit,
              SPI_BaudRatePrescaler_TypeDef SPI_BaudRatePrescaler,
              SPI_Mode_TypeDef SPI_Mode, SPI_CPOL_TypeDef SPI_CPOL,
              SPI_CPHA_TypeDef SPI_CPHA, SPI_DirectionMode_TypeDef SPI_Data_Direction,
              SPI_NSS_TypeDef SPI_Slave_Management, uint8_t CRCPolynomial)
{
  /* Frame Format, BaudRate, Clock Polarity and Phase configuration */
  SPI->CR1 = (uint8_t)((uint8_t)((uint8_t)SPI_FirstBit |
                                  (uint8_t)SPI_BaudRatePrescaler) |
                        (uint8_t)((uint8_t)SPI_CPOL |
                                  SPI_CPHA));

  /* Data direction configuration: BDM, BDOE and RXONLY bits */
  SPI->CR2 = (uint8_t)((uint8_t)(SPI_Data_Direction) | (uint8_t)(SPI_Slave_Management));

  if (SPI_Mode == SPI_Mode_Master)
  {
    SPI->CR2 |= (uint8_t)(1<<0);
  }
  else
  {
    SPI->CR2 &= (uint8_t)~(1<<0);
  }

  /* Master/Slave mode configuration */
  SPI->CR1 |= (uint8_t)(SPI_Mode);

  /* CRC configuration */
  SPI->CRCPR = (uint8_t)CRCPolynomial;
}

void SPI_HW_Cmd(uint8_t NewState)
{
  if (NewState != DISABLE)
  {
    SPI->CR1 |= (1<<6); /* Enable the SPI peripheral*/
  }
  else
  {
    SPI->CR1 &= (uint8_t)(~(1<<6)); /* Disable the SPI peripheral*/
  }
}

void Control_CSN (Spi_JobType jobID, GPIO_State state) {
	/* control state of csn */
	if (state == LOW) {

	} else {

	}
}

void Handle_Job_Finish_0 (void) {
	/* do something */
}

void Handle_Job_Finish_1 (void) {
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
	spiChannel[0]->SpiChannelId=0; //channel start (0)
	spiChannel[1]->SpiChannelId=1; //253 channel (0)
	spiChannel[2]->SpiChannelId=2; //channel end (0)
	/* job 1 */
	spiChannel[3]->SpiChannelId=3; //channel start (1)
	spiChannel[4]->SpiChannelId=4; //253 channel (1)
	spiChannel[5]->SpiChannelId=5; //channel end (1)

	/* data width */
	/* job 0 */
	spiChannel[0]->SpiDataWidth = 1;
	spiChannel[1]->SpiDataWidth = 1;
	spiChannel[2]->SpiDataWidth = 1;
	/* job 1 */
	spiChannel[3]->SpiDataWidth = 1;
	spiChannel[4]->SpiDataWidth = 1;
	spiChannel[5]->SpiDataWidth = 1;
	/* number of buffer each channel */
	/* job 0 */
	spiChannel[0]->SpiIbNBuffers = 2;
	spiChannel[1]->SpiIbNBuffers = 2;
	spiChannel[2]->SpiIbNBuffers = 2;
	/* job 1 */
	spiChannel[3]->SpiIbNBuffers = 2;
	spiChannel[4]->SpiIbNBuffers = 2;
	spiChannel[5]->SpiIbNBuffers = 2;

	/* fixed address buffer base */
	spiChannel[0]->SpiBufferBase = NULL;
	spiChannel[1]->SpiBufferBase = NULL;
	spiChannel[2]->SpiBufferBase = NULL;
	spiChannel[3]->SpiBufferBase = NULL;
	spiChannel[4]->SpiBufferBase = NULL;
	spiChannel[5]->SpiBufferBase = NULL;

	/* default transmit value (32bits) */
	spiChannel[0]->SpiDefaultData = 0x0F0F;
	spiChannel[1]->SpiDefaultData = 0x0000; //cai nay se cho biet la button hay stop
	spiChannel[2]->SpiDefaultData = 0x0000; //253 foo value
	/* job 1 */
	spiChannel[3]->SpiDefaultData = 0x0F0F;
	spiChannel[4]->SpiDefaultData = 0x0000; //cai nay se cho biet la button hay stop
	spiChannel[5]->SpiDefaultData = 0x0000; //253 foo value

	/*
	* - for each job must config
	* - baudrate -> fixed value
	* - timing clk-select pin -> don't care
	* - by default
	*  + we only have 1 SPI HW unit -> defined in advance
	*  + 2 job must config
	*  + set CSN low in idle state -> same for 2 job
	*  + config CPOL = 0, CPHA = 0
	*  + we have 255 channel but only take care of 2 (start channel and end channel)
	*/
	/* chip select pin */
	spiJob[0]->CsnPin = ...;
	spiJob[1]->CsnPin = ...;
	/* function to control chip select pin -> don't care*/
	/* set CSN low in idle state */
	/* config CPOL CPHA */
	/* priority of each job */
	spiJob[0]->JobPriority = 0;
	spiJob[1]->JobPriority = 1;
	/* handler for job's finishing */
	spiJob[0]->JobFinishHandler = &Handle_Job_Finish_0;
	spiJob[1]->JobFinishHandler = &Handle_Job_Finish_1;
	/* fixed link of channel */
	/* [start][temp][end] */
	spiJob[0]->ChannelLink = spiChannel[0];
	spiJob[0]->ChannelNum = 255;

	spiJob[1]->ChannelLink = spiChannel[3];
	spiJob[1]->ChannelNum = 255;

	/*SPI sequence
	* by default:
	* - we have only 1 sequence
	* - 2 job in this sequence
	*/
	/* number of spi's job */
	spiSequence->JobNum = 2;
	/* job target */
	spiSequence->JobLink = NULL;

	/* set status */
	setJobStatus(spiJob[0], SPI_JOB_OK);
	setJobStatus(spiJob[1], SPI_JOB_OK);
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
		*readData = hamtruyen(sendData);
	} else {
		//chi goi ham truyen
		hamtruyen(sendData);
	}
}

Std_ReturnType Spi_Transaction(Spi_ChannelType Channel,const Spi_DataBufferType* DataBufferPtr,uint8 CopiData) {
	uint8 i = 0;
	for(i=0; i < spiDriver->Max_Channel; i++) {
		if(spiDriver->SpiChannel[i]->Channel_ID == Channel) {
			/* neu nguoi dung truyen vao databuffer */
			if(DataBufferPtr != NULL) {
				for(uint8 iData=0; iData < spiDriver->SpiChannel[i]->SpiIbNBuffers; iData++) {
					//goi ham truyen spi 8 bit
					//DataBufferPtr[iData]
					Spi_HW_Transaction(DataBufferPtr[iData], &DataBufferPtr[iData], CopiData);
				}
			/* neu nguoi dung khong truyen vao */
			} else {
				/* neu vung nho mac dinh khac null -> truyen gia tri cua vung nho do*/
				if(spiDriver->SpiChannel[i]->SpiBufferBase != NULL) {
					for(uint8 iData=0; iData < spiDriver->SpiChannel[i]->SpiIbNBuffers; iData++) {
						//goi ham truyen spi 8 bit
						//spiDriver->SpiChannel[i]->SpiBufferBase[iData]
						Spi_HW_Transaction(spiDriver->SpiChannel[i]->SpiBufferBase[iData], 
							&spiDriver->SpiChannel[i]->SpiBufferBase[iData], CopiData);
					}
				/* neu vung nho mac dinh khong duoc cap -> truyen gia tri mac dinh*/
				} else {
					/* neu cho phep ghi du lieu nhung buffer duoc cap de ghi = null -> error */
					if(CopiData == ENABLE) { 
						return E_NOT_OK; 
					}
					for(uint8 iData=0; iData < spiDriver->SpiChannel[i]->SpiIbNBuffers; iData++) {
						//goi ham truyen spi 8 bit
						//spiDriver->SpiChannel[i]->SpiDefaultData
						Spi_HW_Transaction(spiDriver->SpiChannel[i]->SpiDefaultData, 
							&spiDriver->SpiChannel[i]->SpiBufferBase[iData] , CopiData);
					}
				}
			}
			break;
		}
	}
	if(i==spiDriver->Max_Channel) {
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

Std_ReturnType Spi_AsyncTransmit(Spi_SequenceType Sequence) {
	
}
