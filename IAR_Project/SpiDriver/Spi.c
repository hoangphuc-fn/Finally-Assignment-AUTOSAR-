#include "Spi.h"

void setJobStatus(Spi_JobType JobID, Spi_JobResultType JobStatus) {
}
void setSequenceStatus(Spi_SequenceType SequenceID, Spi_SeqResultType SequenceStatus) {
}
void setDriverStatus(Spi_StatusType DriverStatus) {
}

void Control_CSN (Spi_JobType jobID, GPIO_State state) {
	/* control state of csn */
	if (state == LOW) {
		GPIO_Write_Bit(spiDriver.SpiJob[jobID].GPIO_port, spiDriver.SpiJob[jobID].CS_pin, 0);
	} else {
		GPIO_Write_Bit(spiDriver.SpiJob[jobID].GPIO_port, spiDriver.SpiJob[jobID].CS_pin, 1);
	}
}

void Spi_Init(const Spi_ConfigType* ConfigPtr) {
	/* default */
	SPI_HW_Init(SPI_FirstBit_MSB, SPI_BaudRatePrescaler_8, SPI_Mode_Master, SPI_CPOL_Low, \
           SPI_CPHA_1Edge, SPI_Direction_2Lines_FullDuplex, SPI_NSS_Hard, 0x07);
	/* set status */
	setJobStatus(spiDriver.SpiJob[1].SpiJobId, SPI_JOB_OK);
	setJobStatus(spiDriver.SpiJob[2].SpiJobId, SPI_JOB_OK);
	setSequenceStatus(spiDriver.SpiSequence[1].SpiSequenceId, SPI_SEQ_OK);
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
		*readData = SPI_HW_Transync(sendData);
	} else {
		//chi goi ham truyen
		SPI_HW_Transync(sendData);
	}
}

Std_ReturnType Spi_Transaction(Spi_ChannelType Channel, Spi_DataBufferType* DataBufferPtr,uint8 CopiData) {
	uint8 i = 1;
	for(i=1; i <= spiDriver.Max_Channel; i++) {
		if(spiDriver.SpiChannel[i].SpiChannelId == Channel) {
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
                                                printf("value %d", spiDriver.SpiChannel[i].SpiBufferBase[iData]);
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

Std_ReturnType Spi_WriteIB(Spi_ChannelType Channel, Spi_DataBufferType* DataBufferPtr) {
	return Spi_Transaction(Channel, DataBufferPtr, DISABLE);
}

Std_ReturnType Spi_ReadIB(Spi_ChannelType Channel, Spi_DataBufferType* DataBufferPtr) {
	return Spi_Transaction(Channel, DataBufferPtr, ENABLE);
}

/*
Std_ReturnType Spi_AsyncTransmit(Spi_SequenceType Sequence) {
	
}
*/
