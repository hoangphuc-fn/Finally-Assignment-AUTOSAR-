/**
  ******************************************************************************
  * @author  LeHoangPhuc
  * @date    June-2019
  * @course  FGA-UIT training course
  ******************************************************************************
  * @attention
  * I wrote this libary to use in course only.
  * Any problem, please contact me at:
  *        https://github.com/hoangphuc-fn
	   gmail: hoangphuc.fn@gmail.com
  ******************************************************************************
  */ 

#ifndef _SPI_DRIVER_H_
#define _SPI_DRIVER_H_

#include <stdio.h>
#include "spi_hw.h"
#include "gpio.h"
#include "mymacro.h"

/******* WARNING:  Need to define before using*/
#define SPI_MAX_CHANNEL 	4
#define SPI_MAX_JOB		2
#define SPI_MAX_SEQUENCE        1


/******************************************************************************/
/*                          Define Standard Type	            	      */
/******************************************************************************/

typedef uint8 Std_ReturnType;
typedef uint8 Spi_BufferType;

#define E_OK 					0u
#define E_NOT_OK 				(Std_ReturnType)1u

#define E_NO_DTC_AVAILABLE			(Std_ReturnType)2u
#define E_SESSION_NOT_ALLOWED			(Std_ReturnType)4u
#define E_PROTOCOL_NOT_ALLOWED			(Std_ReturnType)5u
#define E_REQUEST_NOT_ACCEPTED			(Std_ReturnType)8u
#define E_REQUEST_ENV_NOK			(Std_ReturnType)9u
#define E_PENDING				(Std_ReturnType)10u
#define E_COMPARE_KEY_FAILED			(Std_ReturnType)11u
#define E_FORCE_RCRRP				(Std_ReturnType)12u

#define STD_HIGH				0x01u
#define STD_LOW					0x00u

#define STD_ACTIVE				0x01u
#define STD_IDLE				0x00u

#define STD_ON					0x01u
#define STD_OFF					0x00u

typedef enum {
    SPI_OFF=0, 
    SPI_ON	
} Spi_StatusOffOn;
typedef enum {
    SPI_LOW=0, 
    SPI_HIGH	
} Spi_StatusLowHigh;

/******************************************************************************/
/*                          Types according to standard	            	      */
/******************************************************************************/
/* @req SWS_Spi_00373 */
typedef enum {
    SPI_UNINIT=0, // The SPI Handler/Driver is not initialized or not usable.
                // SPI011: This shall be the default value after reset. This
                // status shall have the value 0.

    SPI_IDLE,	// The SPI Handler/Driver is not currently transmitting any
                // Job.

    SPI_BUSY, 	// The SPI Handler/Driver is performing a SPI Job( transmit )
} Spi_StatusType;
/* @req SWS_Spi_00374 */
typedef enum {
    SPI_JOB_OK=0, 		// The last transmission of the Job has been finished
                        // successfully.
                        // SPI012: This shall be the default value after reset.
                        // This status shall have the value 0.

    SPI_JOB_PENDING,	// The SPI Handler/Driver is performing a SPI Job.
                        // The meaning of this status is equal to SPI_BUSY.

    SPI_JOB_FAILED, 	// The last transmission of the Job has failed.

    SPI_JOB_QUEUED, 	// Job accepted but not started.

} Spi_JobResultType;
/* @req SWS_Spi_00375 */
typedef enum {

    SPI_SEQ_OK, 	//	The last transmission of the Sequence has been
                    //    finished successfully.
                    //    SPI017: This shall be the default value after reset.
                    //    This status shall have the value 0.

    SPI_SEQ_PENDING, // The SPI Handler/Driver is performing a SPI
                // Sequence. The meaning of this status is equal to
                // SPI_BUSY.

    SPI_SEQ_FAILED, 	// The last transmission of the Sequence has failed.

    SPI_SEQ_CANCELED, 	// The last transmission of the Sequence has been
                    //    cancelled by user.
} Spi_SeqResultType;
/* @req SWS_Spi_00376 */
typedef uint8 Spi_DataBufferType;
/* @req SWS_Spi_00377 */
typedef uint16 Spi_NumberOfDataType;
/* @req SWS_Spi_00378 */
typedef uint8 Spi_ChannelType ;
/* @req SWS_Spi_00379 */
typedef uint16 Spi_JobType;
/*@req SWS_Spi_00380 */
typedef uint8 Spi_SequenceType;
/* @req SWS_Spi_00381 */
typedef uint8 Spi_HWUnitType;
/* @req SWS_Spi_00382 */
typedef enum {

    //	  The asynchronous mechanism is ensured by
    //    polling, so interrupts related to SPI busses
    //    handled asynchronously are disabled.
    SPI_POLLING_MODE,

    //    The asynchronous mechanism is ensured by
    //    interrupt, so interrupts related to SPI busses
    //    handled asynchronously are enabled.

    SPI_INTERRUPT_MODE,
} Spi_AsyncModeType;

// All data needed to configure one SPI-channel
typedef struct Spi_ChannelConfig
{
  // Symbolic name
  Spi_ChannelType SpiChannelId;
  // Buffer usage with EB/IB channel
  Spi_BufferType SpiChannelType;
  // This parameter is the width of a transmitted data unit.
  uint32 SpiDataWidth;
  // This parameter contains the maximum number of data buffers in case of IB
  // Channels and only.
  Spi_NumberOfDataType SpiIbNBuffers;
  // Default buffer address base
  Spi_DataBufferType* SpiBufferBase;
  // This parameter is the default value to transmit.
  uint32 SpiDefaultData;
} Spi_ChannelConfigType;

typedef void (*SpiJobEndNotification_Type)(void);

// All data needed to configure one SPI-Job, amongst others the connection
// between the internal SPI unit and the special settings for an external de-
// vice is done.
/* @req SWS_Spi_00009 */
typedef struct Spi_JobConfig
{

  Spi_JobType SpiJobId;

  // Chip Select Pin
  uint8 CS_pin;
  //port pin
  GPIO_TypeDef* GPIO_port;
  // This parameter is a reference to a notification function.
  void (*SpiJobEndNotification)();
  // Priority of the Job
  // range 0..3
  unsigned SpiJobPriority;

  // A job references several channels. IMPROVEMENT: Optimize this...
  Spi_ChannelType ChannelLink[255+1];
} Spi_JobConfigType;

typedef struct Spi_SequenceConfig
{
  Spi_SequenceType SpiSequenceId;
  // This parameter is a reference to a notification function.
  void (*SpiSeqEndNotification)();
  uint16_t JobNum;
  // A sequence references several jobs, which are executed during a commu-
  // nication sequence
  Spi_JobType JobLink[2+1];
} Spi_SequenceConfigType;


/* @req SWS_Spi_00344 */
/* @req SWS_Spi_00008 */
typedef struct Spi_Driver
{
  uint8 Max_Channel;
  uint16 Max_Job;
  uint8 Max_Sequence;
  // All data needed to configure one SPI-channel
  /* @req SWS_Spi_00063 */
  struct Spi_ChannelConfig * SpiChannel;

  //	All data needed to configure one SPI-Job, amongst others the
  //	connection between the internal SPI unit and the special set-
  //	tings for an external device is done.
  struct Spi_JobConfig * SpiJob;

  // All data needed to configure one SPI-sequence
  struct Spi_SequenceConfig * SpiSequence;
} Spi_DriverType;
/* @req SWS_Spi_00372 */
typedef Spi_DriverType Spi_ConfigType;

extern Spi_ConfigType spiDriver;

/* function declaration */
void Control_CSN (Spi_JobType jobID, GPIO_State state);
void Spi_Init(const Spi_ConfigType* ConfigPtr);
Std_ReturnType Spi_DeInit(void);
Std_ReturnType Spi_WriteIB(Spi_ChannelType Channel, Spi_DataBufferType* DataBufferPtr);
Std_ReturnType Spi_ReadIB(Spi_ChannelType Channel, Spi_DataBufferType* DataBufferPtr);
void Spi_HW_Transaction(uint8 sendData, uint8 *readData, uint8 CopiData);
void setJobStatus(Spi_JobType JobID, Spi_JobResultType JobStatus);
void setSequenceStatus(Spi_SequenceType SequenceID, Spi_SeqResultType SequenceStatus);
void setDriverStatus(Spi_StatusType DriverStatus);

#endif /*_SPI_DRIVER_H_*/