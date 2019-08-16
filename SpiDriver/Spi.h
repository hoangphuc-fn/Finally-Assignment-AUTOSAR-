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

/******* WARNING:  Need to define before using*/
#define SPI_MAX_CHANNEL 	x /* x = ? */
#define SPI_MAX_JOB		y /* y = ? */


/******************************************************************************/
/*                          Define Standard Type	            	      */
/******************************************************************************/

typedef uint8 Std_ReturnType;

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
  // This parameter is the default value to transmit.
  uint32 SpiDefaultData;
  // This parameter contains the maximum number of data buffers in case of IB
  // Channels and only.
  Spi_NumberOfDataType SpiIbNBuffers;

  // This parameter defines the first starting bit for transmission.
  Spi_TransferStartType SpiTransferStart;
} Spi_ChannelConfigType;

typedef void (*SpiJobEndNotification_Type)(void);

// All data needed to configure one SPI-Job, amongst others the connection
// between the internal SPI unit and the special settings for an external de-
// vice is done.
/* @req SWS_Spi_00009 */
typedef struct Spi_JobConfig
{

  Spi_JobType SpiJobId;

  //	This parameter is the symbolic name to identify the HW SPI Hardware micro-
  //	controller peripheral allocated to this Job.
  uint32 SpiHwUnit;

  // This parameter is a reference to a notification function.
  SpiJobEndNotification_Type SpiJobEndNotification;

  // Chip Select Pin
  uint8 CS_pin;
  // Chip select functionality on/off
  Spi_StatusOffOn CS_pin_status;
  // Chip select pin polarity high or low
  Spi_StatusLowHigh CS_pin_active;
  // Priority of the Job
  // range 0..3
  unsigned SpiJobPriority;

  // A job references several channels. IMPROVEMENT: Optimize this...
#if defined(SPI_CONFIG_VERSION)
  const Spi_ChannelConfigType *SpiChannelAssignment[SPI_MAX_CHANNEL+1];
#else
  Spi_ChannelType ChannelAssignment[SPI_MAX_CHANNEL+1];
#endif

  // Reference to the external device used by this job
  /* @req SWS_Spi_00010 */
#if defined(SPI_CONFIG_VERSION)
  const struct Spi_ExternalDevice * SpiDeviceAssignment;
#else
  Spi_ExternalDeviceTypeType DeviceAssignment;
#endif

} Spi_JobConfigType;

/* @req SWS_Spi_00048 */
typedef void (*Spi_CallbackType)(void);

typedef struct Spi_SequenceConfig
{
  Spi_SequenceType SpiSequenceId;
  //
  const sint16 * SpiSeqSharingCommonJobsTablePtr;
  // This parameter allows or not this Sequence to be suspended by another
  // one.
  unsigned SpiInterruptibleSequence;
  // This parameter is a reference to a notification function.
  Spi_CallbackType SpiSeqEndNotification;

  // A sequence references several jobs, which are executed during a commu-
  // nication sequence
#if defined(SPI_CONFIG_VERSION)
  const Spi_JobConfigType *SpiJobAssignment[SPI_MAX_JOB+1];
#else
  Spi_JobType JobAssignment[SPI_MAX_JOB+1];
#endif
} Spi_SequenceConfigType;


/* @req SWS_Spi_00344 */
/* @req SWS_Spi_00008 */
typedef struct Spi_Driver
{
  //	This parameter contains the number of Channels configured. It will be
  //	gathered by tools during the configuration stage.
  uint8 SpiMaxChannel;

  uint8 SpiMaxJob;

  uint8 SpiMaxSequence;

  // All data needed to configure one SPI-channel
  /* @req SWS_Spi_00063 */
  const struct Spi_ChannelConfig * SpiChannelConfig;

  //	All data needed to configure one SPI-Job, amongst others the
  //	connection between the internal SPI unit and the special set-
  //	tings for an external device is done.
  const struct Spi_JobConfig * SpiJobConfig;

  // All data needed to configure one SPI-sequence
  const struct Spi_SequenceConfig * SpiSequenceConfig;
} Spi_DriverType;
/* @req SWS_Spi_00372 */
typedef Spi_DriverType Spi_ConfigType;

#endif /*_SPI_DRIVER_H_*/