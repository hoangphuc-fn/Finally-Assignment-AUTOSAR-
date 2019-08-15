#include "Spi.h"

Spi_ConfigType spiDriver;

Spi_JobResultType jobResultStatus[SPI_MAX_JOB+1];
Spi_SeqResultType seqResultStatus[SPI_MAX_SEQUENCE+1];
Spi_StatusType    driverResultStatus;

void setJobStatus(Spi_JobType JobID, Spi_JobResultType JobStatus) {
  jobResultStatus[JobID] = JobStatus;
}
void setSequenceStatus(Spi_SequenceType SequenceID, Spi_SeqResultType SequenceStatus) {
  seqResultStatus[SequenceID] = SequenceStatus;
}
void setDriverStatus(Spi_StatusType DriverStatus) {
  driverResultStatus = DriverStatus;
}

Spi_StatusType Spi_GetStatus(void) {
  return driverResultStatus;
}

Spi_JobResultType Spi_GetJobResult(Spi_JobType Job) {
  return jobResultStatus[Job];
}

Spi_SeqResultType Spi_GetSequenceResult(Spi_SequenceType Sequence) {
  return seqResultStatus[Sequence];
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

void Spi_HW_Transaction(uint8 sendData, uint8 *readData) {
  *readData = SPI_HW_Transync(sendData);
}

void Spi_Transaction_Channel(Spi_ChannelType Channel, Spi_DataBufferType* DataBufferPtr) {
  for(uint8 iData=0; iData < spiDriver.SpiChannel[Channel].SpiIbNBuffers; iData++) {
    Spi_HW_Transaction(DataBufferPtr[iData], &DataBufferPtr[iData]);
  }
}

Std_ReturnType Spi_WriteIB(Spi_ChannelType Channel, const Spi_DataBufferType* DataBufferPtr) {
  uint8 i = 1;
  for(i=1; i <= spiDriver.Max_Channel; i++) {
    if(spiDriver.SpiChannel[i].SpiChannelId == Channel) {
      for(uint8 iData=0; iData < spiDriver.SpiChannel[i].SpiIbNBuffers; iData++) {
        spiDriver.SpiChannel[i].SpiBufferBase[iData]=DataBufferPtr[iData];
      }
      break;
    }
  }
  if(i>spiDriver.Max_Channel) {
    return E_NOT_OK;
  } else {
    return E_OK;
  }
}

Std_ReturnType Spi_ReadIB(Spi_ChannelType Channel, Spi_DataBufferType* DataBufferPtr) {
  uint8 i = 1;
  for(i=1; i <= spiDriver.Max_Channel; i++) {
    if(spiDriver.SpiChannel[i].SpiChannelId == Channel) {
      for(uint8 iData=0; iData < spiDriver.SpiChannel[i].SpiIbNBuffers; iData++) {
        DataBufferPtr[iData]=spiDriver.SpiChannel[i].SpiBufferBase[iData];
      }
      break;
    }
  }
  if(i>spiDriver.Max_Channel) {
    return E_NOT_OK;
  } else {
    return E_OK;
  }
}

/*
- lack:
  + [SWS_Spi_00135]
  + [SWS_Spi_00136]
  + [SWS_Spi_00136]
- completed:
  + rest...
*/

/*
- Note: 
  + ham nay dang handle SpiLevelDelivered=LEVEL 1
  + neu muon LEVEL 0 thi can execute cac job khong dua tren priority
  + LEVEL 2 -> bo
*/

Std_ReturnType Spi_AsyncTransmit(Spi_SequenceType Sequence) {
  setDriverStatus(SPI_BUSY);
  setSequenceStatus(Sequence, SPI_SEQ_PENDING);
  
#if (SpiLevelDelivered==1) //LEVEL 1
  while(spiDriver.SpiSequence[Sequence].JobNum > 0) {
    //sequence canceled
    if(spiDriver.SpiJob[Sequence].Canceled == ENABLE) {
      spiDriver.SpiJob[Sequence].Canceled = DISABLE;
      setDriverStatus(SPI_IDLE);
      setSequenceStatus(Sequence,SPI_SEQ_CANCELED);
      return E_OK;
    }
    /* luom ra job co priority cao nhat */
    uint16 iJob=1;
    Spi_JobType headJob;
    for(; iJob<=spiDriver.SpiSequence[Sequence].JobNum; iJob++) {
      if(spiDriver.SpiSequence[Sequence].JobLink[iJob] != 0) {
        headJob=spiDriver.SpiSequence[Sequence].JobLink[iJob];
        break;
      }
    }
    iJob++;
    for(; iJob<=spiDriver.SpiSequence[Sequence].JobNum; iJob++) {
      /* lay gia tri priority cua headjob so sanh voi cac job con lai cua sequence */
      //lay gia tri cua job can compare
      Spi_JobType jobCompare = spiDriver.SpiSequence[Sequence].JobLink[iJob];
      if(jobCompare != 0) {
        //bat dau so sanh priority
        if(spiDriver.SpiJob[jobCompare].SpiJobPriority > spiDriver.SpiJob[headJob].SpiJobPriority) {
          headJob = spiDriver.SpiJob[iJob].SpiJobId;
        }
      }
    }
    setJobStatus(headJob,SPI_JOB_PENDING);
    Control_CSN(headJob, LOW);
    /* thuc thi job sau khi co job co vi tri priority cao nhat */
    for(uint16 iChannel=1; iChannel<=255; iChannel++) {
      if(spiDriver.SpiJob[headJob].Canceled == ENABLE) {
        Control_CSN(headJob, HIGH);
        spiDriver.SpiJob[headJob].Canceled = DISABLE;
        setJobStatus(headJob, SPI_JOB_FAILED);
        setSequenceStatus(Sequence, SPI_SEQ_CANCELED);
        (*spiDriver.SpiJob[headJob].SpiJobEndNotification)();
        return E_OK;
      }
      //get channel ID of job
      Spi_ChannelType channelID = spiDriver.SpiJob[headJob].ChannelLink[iChannel];
      Spi_Transaction_Channel(spiDriver.SpiChannel[channelID].SpiChannelId,
                              spiDriver.SpiChannel[channelID].SpiBufferBase);
    }
    Control_CSN(headJob, HIGH);
    //set status for job
    setJobStatus(headJob, SPI_JOB_OK);
    //call the call-back funtion
    (*spiDriver.SpiJob[headJob].SpiJobEndNotification)();
    //remove job from sequence
    for(iJob=1; iJob<=spiDriver.SpiSequence[Sequence].JobNum; iJob++) {
      if(spiDriver.SpiSequence[Sequence].JobLink[iJob] == headJob) {
        spiDriver.SpiSequence[Sequence].JobLink[iJob]=0;
      }
    }
    spiDriver.SpiSequence[Sequence].JobNum--;
  }
#endif
  
#if (SpiLevelDelivered==0) //LEVEL 0
  for(uint16 iJob=1; iJob <= spiDriver.SpiSequence[Sequence].JobNum; iJob++) {
    //sequence canceled
    if(spiDriver.SpiSequence[Sequence].Canceled == ENABLE) {
      spiDriver.SpiSequence[Sequence].Canceled = DISABLE;
      setDriverStatus(SPI_IDLE);
      setSequenceStatus(Sequence,SPI_SEQ_CANCELED);
      return E_OK;
    }
    setJobStatus(iJob,SPI_JOB_PENDING);
    /* thuc thi job */
    Spi_JobType headJob = spiDriver.SpiSequence[Sequence].JobLink[iJob];
    Control_CSN(headJob, LOW);
    for(uint16 iChannel=1; iChannel<=255; iChannel++) {
      if(spiDriver.SpiJob[headJob].Canceled == ENABLE) {
        Control_CSN(headJob, HIGH);
        spiDriver.SpiJob[headJob].Canceled = DISABLE;
        setJobStatus(headJob, SPI_JOB_FAILED);
        setSequenceStatus(Sequence, SPI_SEQ_CANCELED);
        (*spiDriver.SpiJob[headJob].SpiJobEndNotification)();
        return E_OK;
      }
      //get channel ID of job
      Spi_ChannelType channelID = spiDriver.SpiJob[headJob].ChannelLink[iChannel];
      Spi_Transaction_Channel(spiDriver.SpiChannel[channelID].SpiChannelId,
                              spiDriver.SpiChannel[channelID].SpiBufferBase);
    }
    Control_CSN(headJob, HIGH);
    //set status for job
    setJobStatus(headJob, SPI_JOB_OK);
    //call the call-back funtion
    (*spiDriver.SpiJob[headJob].SpiJobEndNotification)();
  }
#endif
  setDriverStatus(SPI_IDLE);
  setSequenceStatus(Sequence, SPI_SEQ_OK);
  return E_OK;
}

void Spi_Cancel(Spi_SequenceType Sequence) {
  setSequenceStatus(Sequence, SPI_SEQ_CANCELED);
  spiDriver.SpiSequence[Sequence].Canceled = ENABLE;
}

void Spi_Job_Cancel(Spi_SequenceType Job) {
  spiDriver.SpiJob[Job].Canceled = ENABLE;
}
