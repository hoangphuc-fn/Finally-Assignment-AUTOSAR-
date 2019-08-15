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
#define SPI_MAX_CHANNEL 	4
#define SPI_MAX_JOB		2


/******************************************************************************/
/*                          Define Standard Type	            	      */
/******************************************************************************/

typedef uint8 Std_ReturnType;

typedef uint8_t uint8;
typedef uint16_t uint16;
typedef uint16_t uint16;

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

  // Chip Select Pin
  uint8 CS_pin;
  //port pin
  GPIO_TypeDef GPIO_port;
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
  const struct Spi_ChannelConfig * spiChannel;

  //	All data needed to configure one SPI-Job, amongst others the
  //	connection between the internal SPI unit and the special set-
  //	tings for an external device is done.
  const struct Spi_JobConfig * spiJob;

  // All data needed to configure one SPI-sequence
  const struct Spi_SequenceConfig * spiSequence;
} Spi_DriverType;
/* @req SWS_Spi_00372 */
typedef Spi_DriverType Spi_ConfigType;

Spi_ChannelConfigType spiChannel[SPI_MAX_CHANNEL+1];
Spi_JobConfigType spiJob[SPI_MAX_JOB+1];
Spi_ConfigType spiDriver;

/* function declaration */
void Handle_Job_Finish_0 (void);
void Handle_Job_Finish_1 (void);
void Control_CSN (Spi_JobType jobID, GPIO_State state);
void Spi_Init(const Spi_ConfigType* ConfigPtr);
Std_ReturnType Spi_DeInit(void);
Std_ReturnType Spi_WriteIB(Spi_ChannelType Channel,const Spi_DataBufferType* DataBufferPtr);
Std_ReturnType Spi_ReadIB(Spi_ChannelType Channel,const Spi_DataBufferType* DataBufferPtr);

/*--------------------------------standard lib defination--------------------*/
typedef enum {
  SPI_Direction_2Lines_FullDuplex = (uint8)0x00, /*!< 2-line uni-directional data mode enable */
  SPI_Direction_2Lines_RxOnly     = (uint8)0x04, /*!< Receiver only in 2 line uni-directional data mode */
  SPI_Direction_1Line_Rx          = (uint8)0x80, /*!< Receiver only in 1 line bi-directional data mode */
  SPI_Direction_1Line_Tx          = (uint8)0xC0  /*!< Transmit only in 1 line bi-directional data mode */
} SPI_DirectionMode_TypeDef;
 
typedef enum
{
  SPI_NSS_Soft  = (uint8)0x02, /*!< Software slave management disabled */
  SPI_NSS_Hard  = (uint8)0x00  /*!< Software slave management enabled */
} SPI_NSS_TypeDef;

typedef enum 
{
  SPI_Direction_Rx = (uint8)0x00, /*!< Select Rx receive direction in bi-directional mode */
  SPI_Direction_Tx = (uint8)0x01  /*!< Select Tx transmission direction in bi-directional mode */
} SPI_Direction_TypeDef;

typedef enum
{
  SPI_Mode_Master = (uint8)0x04, /*!< SPI Master configuration */
  SPI_Mode_Slave  = (uint8)0x00  /*!< SPI Slave configuration */
} SPI_Mode_TypeDef;

typedef enum {
  SPI_BaudRatePrescaler_2   = (uint8)0x00, /*!< SPI frequency = frequency(CPU)/2 */
  SPI_BaudRatePrescaler_4   = (uint8)0x08, /*!< SPI frequency = frequency(CPU)/4 */
  SPI_BaudRatePrescaler_8   = (uint8)0x10, /*!< SPI frequency = frequency(CPU)/8 */
  SPI_BaudRatePrescaler_16  = (uint8)0x18, /*!< SPI frequency = frequency(CPU)/16 */
  SPI_BaudRatePrescaler_32  = (uint8)0x20, /*!< SPI frequency = frequency(CPU)/32 */
  SPI_BaudRatePrescaler_64  = (uint8)0x28, /*!< SPI frequency = frequency(CPU)/64 */
  SPI_BaudRatePrescaler_128 = (uint8)0x30, /*!< SPI frequency = frequency(CPU)/128 */
  SPI_BaudRatePrescaler_256 = (uint8)0x38  /*!< SPI frequency = frequency(CPU)/256 */
} SPI_BaudRatePrescaler_TypeDef;

typedef enum 
{
  SPI_CPOL_Low  = (uint8)0x00, /*!< Clock to 0 when idle */
  SPI_CPOL_High = (uint8)0x02  /*!< Clock to 1 when idle */
} SPI_CPOL_TypeDef;

typedef enum 
{
  SPI_CPHA_1Edge = (uint8)0x00, /*!< The first clock transition is the first data capture edge */
  SPI_CPHA_2Edge = (uint8)0x01  /*!< The second clock transition is the first data capture edge */
} SPI_CPHA_TypeDef;

typedef enum 
{
  SPI_FirstBit_MSB = (uint8)0x00, /*!< MSB bit will be transmitted first */
  SPI_FirstBit_LSB = (uint8)0x80  /*!< LSB bit will be transmitted first */
} SPI_FirstBit_TypeDef;

typedef enum {
  SPI_DMAReq_RX = (uint8)0x01,    /*!< SPI DMA Rx transfer requests */
  SPI_DMAReq_TX = (uint8)0x02     /*!< SPI DMA Tx transfer requests */
} SPI_DMAReq_TypeDef;

typedef enum {
  SPI_CRC_RX = (uint8)0x00, /*!< Select Tx CRC register */
  SPI_CRC_TX = (uint8)0x01  /*!< Select Rx CRC register */
} SPI_CRC_TypeDef;

typedef enum {
  SPI_FLAG_BSY    = (uint8)0x80, /*!< Busy flag */
  SPI_FLAG_OVR    = (uint8)0x40, /*!< Overrun flag */
  SPI_FLAG_MODF   = (uint8)0x20, /*!< Mode fault */
  SPI_FLAG_CRCERR = (uint8)0x10, /*!< CRC error flag */
  SPI_FLAG_WKUP   = (uint8)0x08, /*!< Wake-up flag */
  SPI_FLAG_TXE    = (uint8)0x02, /*!< Transmit buffer empty */
  SPI_FLAG_RXNE   = (uint8)0x01  /*!< Receive buffer empty */
} SPI_FLAG_TypeDef;

typedef enum
{
  SPI_IT_WKUP   = (uint8)0x34, /*!< Wake-up interrupt*/
  SPI_IT_OVR    = (uint8)0x65,  /*!< Overrun interrupt*/
  SPI_IT_MODF   = (uint8)0x55, /*!< Mode fault interrupt*/
  SPI_IT_CRCERR = (uint8)0x45, /*!< CRC error interrupt*/
  SPI_IT_TXE    = (uint8)0x17, /*!< Transmit buffer empty interrupt*/
  SPI_IT_RXNE   = (uint8)0x06, /*!< Receive buffer not empty interrupt*/
  SPI_IT_ERR    = (uint8)0x05  /*!< Error interrupt*/
} SPI_IT_TypeDef;

typedef struct SPI_struct
{
  volatile uint8 CR1;    /*!< SPI control register 1 */
  volatile uint8 CR2;    /*!< SPI control register 2 */
  volatile uint8 CR3;    /*!< SPI DMA and interrupt control register */
  volatile uint8 SR;     /*!< SPI status register */
  volatile uint8 DR;     /*!< SPI data I/O register */
  volatile uint8 CRCPR;  /*!< SPI CRC polynomial register */
  volatile uint8 RXCRCR; /*!< SPI Rx CRC register */
  volatile uint8 TXCRCR; /*!< SPI Tx CRC register */
}
SPI_TypeDef;

typedef enum {
  LOW = (uint8)0x00;
  HIGH = (uint8)0x01;
}

#define SPI_BASE                   (uint16_t)0x5200
#define SPI                        ((SPI_TypeDef *) SPI_BASE)

#define DISABLE 0
#define ENABLE !DISABLE

void SPI_HW_Init(SPI_FirstBit_TypeDef SPI_FirstBit,
              SPI_BaudRatePrescaler_TypeDef SPI_BaudRatePrescaler,
              SPI_Mode_TypeDef SPI_Mode, SPI_CPOL_TypeDef SPI_CPOL,
              SPI_CPHA_TypeDef SPI_CPHA, SPI_DirectionMode_TypeDef SPI_Data_Direction,
              SPI_NSS_TypeDef SPI_Slave_Management, uint8 CRCPolynomial);
void SPI_HW_Cmd(uint8 NewState);
void Spi_HW_Transaction(uint8 sendData, uint8 *readData, uint8 CopiData);

#endif /*_SPI_DRIVER_H_*/