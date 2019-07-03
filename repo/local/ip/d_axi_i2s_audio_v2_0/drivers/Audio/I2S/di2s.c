/*
 * di2s.c
 *
 *  Created on: Aug 28, 2015
 *      Author: rohegbec
 */



/***************************** Include Files ********************************/

#include "di2s.h"
#include "xstatus.h"

/************************** Constant Definitions ****************************/

/**************************** Type Definitions ******************************/

/***************** Macros (Inline Functions) Definitions ********************/

/************************** Variable Definitions ****************************/


/************************** Function Prototypes *****************************/


/****************************************************************************/
/**
* Initialize the DI2s instance provided by the caller based on the
* given configuration data.
*
* Nothing is done except to initialize the InstancePtr.
*
* @param	InstancePtr is a pointer to an DI2s instance. The memory the
*		pointer references must be pre-allocated by the caller. Further
*		calls to manipulate the driver through the DI2s API must be
*		made with this pointer.
* @param	Config is a reference to a structure containing information
*		about a specific I2S device. This function initializes an
*		InstancePtr object for a specific device specified by the
*		contents of Config. This function can initialize multiple
*		instance objects with the use of multiple calls giving different
*		Config information on each call.
* @param 	EffectiveAddr is the device base address in the virtual memory
*		address space. The caller is responsible for keeping the address
*		mapping from EffectiveAddr to the device physical base address
*		unchanged once this function is invoked. Unexpected errors may
*		occur if the address mapping changes after this function is
*		called. If address translation is not used, use
*		Config->BaseAddress for this parameters, passing the physical
*		address instead.
*
* @return
* 		- XST_SUCCESS	Initialization was successfull.
*
* @note		None.
*
*****************************************************************************/

XStatus DI2s_CfgInitialize(DI2s * InstancePtr, DI2s_Config * Config,
		u32 EffectiveAddr)
{
	/*
	 * Assert arguments
	 */
	Xil_AssertNonvoid(InstancePtr != NULL);

	InstancePtr->BaseAddress = EffectiveAddr;
	InstancePtr->StreamCaplepable = Config->StreamCaplepable;
	InstancePtr->MastreMode = Config->MastreMode;

	/*
	 * Indicate the instance is now ready to use, initialized without error
	 */
	InstancePtr->IsReady = XIL_COMPONENT_IS_READY;
	return (XST_SUCCESS);

}

 /****************************************************************************/
 /**
 * Read out the status of the FIFO flags. This function will read out the
 * status of the FIFO's and return the value.
 *
 * @param	InstancePtr is a pointer to an DI2s instance. The memory the
 *		pointer references must be pre-allocated by the caller. Further
 *		calls to manipulate the driver through the DI2s API must be
 *		made with this pointer.
 *
 * @return
 * 		The status register values.
 *
 * @note		None.
 *
 *****************************************************************************/
u32 DI2s_ReadStatusReg(DI2s * InstancePtr)
{
	Xil_AssertVoid(InstancePtr != NULL);
	Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

	return DI2s_ReadReg(InstancePtr->BaseAddress, DI2S_STATUS_OFFSET);

}

/****************************************************************************/
/**
* Read out the status of the transmit FIFO empty flag.
*
* @param	InstancePtr is a pointer to an DI2s instance. The memory the
*		pointer references must be pre-allocated by the caller. Further
*		calls to manipulate the driver through the DI2s API must be
*		made with this pointer.
*
* @return
* 		The value of the transmit FIFO empty flag
*
* @note		None.
*
*****************************************************************************/
u8 DI2s_GetTxFifoEmpty(DI2s * InstancePtr)
{
	Xil_AssertVoid(InstancePtr != NULL);
	Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

	DI2s_BitField.l = DI2s_ReadStatusReg(InstancePtr);

	return DI2s_BitField.bit.u32bit0;
}

/****************************************************************************/
/**
* Read out the status of the transmit FIFO full flag.
*
* @param	InstancePtr is a pointer to an DI2s instance. The memory the
*		pointer references must be pre-allocated by the caller. Further
*		calls to manipulate the driver through the DI2s API must be
*		made with this pointer.
*
* @return
* 		The value of the transmit FIFO full flag
*
* @note		None.
*
*****************************************************************************/
u8 DI2s_GetTxFifoFull(DI2s * InstancePtr)
{
	Xil_AssertVoid(InstancePtr != NULL);
	Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

	DI2s_BitField.l = DI2s_ReadStatusReg(InstancePtr);

	return DI2s_BitField.bit.u32bit1;
}

/****************************************************************************/
/**
* Read out the status of the receive FIFO empty flag.
*
* @param	InstancePtr is a pointer to an DI2s instance. The memory the
*		pointer references must be pre-allocated by the caller. Further
*		calls to manipulate the driver through the DI2s API must be
*		made with this pointer.
*
* @return
* 		The value of the receive FIFO empty flag
*
* @note		None.
*
*****************************************************************************/
u8 DI2s_GetRxFifoEmpty(DI2s * InstancePtr)
{
	Xil_AssertVoid(InstancePtr != NULL);
	Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

	DI2s_BitField.l = DI2s_ReadStatusReg(InstancePtr);

	return DI2s_BitField.bit.u32bit16;
}

/****************************************************************************/
/**
* Read out the status of the receive FIFO full flag.
*
* @param	InstancePtr is a pointer to an DI2s instance. The memory the
*		pointer references must be pre-allocated by the caller. Further
*		calls to manipulate the driver through the DI2s API must be
*		made with this pointer.
*
* @return
* 		The value of the receive FIFO full flag
*
* @note		None.
*
*****************************************************************************/
u8 DI2s_GetRxFifoFull(DI2s * InstancePtr)
{
	Xil_AssertVoid(InstancePtr != NULL);
	Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

	DI2s_BitField.l = DI2s_ReadStatusReg(InstancePtr);

	return DI2s_BitField.bit.u32bit17;
}

/****************************************************************************/
/**
* Read out the sample acquired by the I2S core. This is only used when the
* The core operates in non-stream mode.
*
* @param	InstancePtr is a pointer to an DI2s instance. The memory the
*		pointer references must be pre-allocated by the caller. Further
*		calls to manipulate the driver through the DI2s API must be
*		made with this pointer.
*
* @return
* 		The last acquired sample.
*
* @note		None.
*
*****************************************************************************/
u32 DI2s_GetSample(DI2s * InstancePtr)
{

	Xil_AssertVoid(InstancePtr != NULL);
	Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

	return DI2s_ReadReg(InstancePtr->BaseAddress, DI2S_DATA_IN_OFFSET);

}

void DI2s_SetNrSamples(DI2s * InstancePtr, u32 NrSamples)
{

	Xil_AssertVoid(InstancePtr != NULL);
	Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

	DI2s_WriteReg(InstancePtr->BaseAddress, DI2S_WORD_CNT_OFFSET, NrSamples);
}

/****************************************************************************/
/**
* Send the desired sample value to the I2S core. This is only used when the
* The core operates in non-stream mode.
*
* @param	InstancePtr is a pointer to an DI2s instance. The memory the
*		pointer references must be pre-allocated by the caller. Further
*		calls to manipulate the driver through the DI2s API must be
*		made with this pointer.
*
* @param	Sample is the numeric sample to be sent to the I2S core. The value is on
* 		32 bits	and it has no signed value.
*
* @return
* 			None.
*
* @note		None.
*
*****************************************************************************/
void DI2s_SendSample(DI2s * InstancePtr, u32 Sample)
{
	Xil_AssertVoid(InstancePtr != NULL);
	Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

	DI2s_WriteReg(InstancePtr->BaseAddress, DI2S_DATA_OUT_OFFSET, Sample);
}

/****************************************************************************/
/**
* Resets the I2S core from all perspectives. It will send a General reset signal
* and will individualy reset the FIFOs
*
* @param	InstancePtr is a pointer to an DI2s instance. The memory the
*		pointer references must be pre-allocated by the caller. Further
*		calls to manipulate the driver through the DI2s API must be
*		made with this pointer.
*
* @return
* 			None.
*
* @note		None.
*
*****************************************************************************/
void DI2s_Reset(DI2s * InstancePtr)
{
	Xil_AssertVoid(InstancePtr != NULL);
	Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

	// set global reset register
	DI2s_WriteReg(InstancePtr->BaseAddress, DI2S_RESET_OFFSET, 1);

	DI2s_BitField.l = DI2s_ReadReg(InstancePtr->BaseAddress, DI2S_FIFO_CTL_OFFSET);
	DI2s_BitField.bit.u32bit30 = 1; // set DI2S_TX_FIFO_RST bit
	DI2s_BitField.bit.u32bit31 = 1; // set DI2S_RX_FIFO_RST bit
	DI2s_WriteReg(InstancePtr->BaseAddress, DI2S_RESET_OFFSET, DI2s_BitField.l);

	// clear global reset register
	DI2s_WriteReg(InstancePtr->BaseAddress, DI2S_RESET_OFFSET, 0);
	DI2s_BitField.bit.u32bit30 = 0; // clear DI2S_TX_FIFO_RST bit
	DI2s_BitField.bit.u32bit31 = 0; // set DI2S_RX_FIFO_RST bit
	DI2s_WriteReg(InstancePtr->BaseAddress, DI2S_RESET_OFFSET, DI2s_BitField.l);
}

/****************************************************************************/
/**
* Starts the transmission in a specific direction. if the first bit is set
* the transmission will start if the second bit is set the reception will
* start. Optionally both bits can be set in order to start both transactions
*
* @param	InstancePtr is a pointer to an DI2s instance. The memory the
*		pointer references must be pre-allocated by the caller. Further
*		calls to manipulate the driver through the DI2s API must be
*		made with this pointer.
*
* @param	Direction will set if the transmission or the reception is started
*		for the I2S core. The options are : DI2S_TRANSMIT and DI2S_RECEIVE
*		For a full duplex transmission both can be set
*
* @return
* 			None.
*
* @note		None.
*
*****************************************************************************/
void DI2s_StartI2s(DI2s * InstancePtr, u8 Direction)
{
	Xil_AssertVoid(InstancePtr != NULL);
	Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

	DI2s_BitField.l = DI2s_ReadReg(InstancePtr->BaseAddress, DI2S_TRANSFER_OFFSET);

	if (Direction & DI2S_TRANSMIT)
	{
		DI2s_BitField.bit.u32bit0 = 1; // DI2S_TX_RS bit
	}
	if (Direction & DI2S_RECEIVE)
	{
		DI2s_BitField.bit.u32bit1 = 1; // DI2S_RX_RS bit
	}

	DI2s_WriteReg(InstancePtr->BaseAddress, DI2S_TRANSFER_OFFSET, DI2s_BitField.l);

}

/****************************************************************************/
/**
* Stops the transmission in a specific direction. if the first bit is set
* the transmission will stop if the second bit is set the reception will
* stop. Optionally both bits can be set in order to stop both transactions
*
* @param	InstancePtr is a pointer to an DI2s instance. The memory the
*		pointer references must be pre-allocated by the caller. Further
*		calls to manipulate the driver through the DI2s API must be
*		made with this pointer.
*
* @param	Direction will set if the transmission or the reception is stoped
*		for the I2S core. The options are : DI2S_TRANSMIT and DI2S_RECEIVE
*		For a full duplex transmission both can be set
*
* @return
* 			None.
*
* @note		None.
*
*****************************************************************************/
void DI2s_StopI2s(DI2s * InstancePtr, u8 Direction)
{
	Xil_AssertVoid(InstancePtr != NULL);
	Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

	DI2s_BitField.l = DI2s_ReadReg(InstancePtr->BaseAddress, DI2S_TRANSFER_OFFSET);

	if (Direction & DI2S_TRANSMIT)
	{
		DI2s_BitField.bit.u32bit0 = 0; // DI2S_TX_RS bit
	}
	if (Direction & DI2S_RECEIVE)
	{
		DI2s_BitField.bit.u32bit1 = 0; // DI2S_RX_RS bit
	}

	DI2s_WriteReg(InstancePtr->BaseAddress, DI2S_TRANSFER_OFFSET, DI2s_BitField.l);

}

/****************************************************************************/
/**
* Starts the transmission stream in a specific direction. if the first bit is set
* the transmission will start if the second bit is set the reception will
* start. Optionally both bits can be set in order to start both transactions
*
* @param	InstancePtr is a pointer to an DI2s instance. The memory the
*		pointer references must be pre-allocated by the caller. Further
*		calls to manipulate the driver through the DI2s API must be
*		made with this pointer.
*
* @param	Direction will set if the transmission or the reception is started
*		for the I2S core. The options are : DI2S_TRANSMIT and DI2S_RECEIVE
*		For a full duplex transmission both can be set
*
* @return
* 			None.
*
* @note		None.
*
*****************************************************************************/
void DI2s_StartI2sStream(DI2s * InstancePtr, u8 Direction)
{
	Xil_AssertVoid(InstancePtr != NULL);
	Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

	if (InstancePtr->StreamCaplepable)
	{

		DI2s_BitField.l = DI2s_ReadReg(InstancePtr->BaseAddress, DI2S_STREAM_CTL_OFFSET);

		if (Direction & DI2S_TRANSMIT)
		{
			DI2s_BitField.bit.u32bit1 = 1; // DI2S_TX_STREAM_ENABLE bit
		}
		if (Direction & DI2S_RECEIVE)
		{
			DI2s_BitField.bit.u32bit0 = 1; // DI2S_RX_STREAM_ENABLE bit
		}

		DI2s_WriteReg(InstancePtr->BaseAddress, DI2S_STREAM_CTL_OFFSET, DI2s_BitField.l);
	}

}

/****************************************************************************/
/**
* Stops the transmission in a specific direction. if the first bit is set
* the transmission will stop if the second bit is set the reception will
* stop. Optionally both bits can be set in order to stop both transactions
*
* @param	InstancePtr is a pointer to an DI2s instance. The memory the
*		pointer references must be pre-allocated by the caller. Further
*		calls to manipulate the driver through the DI2s API must be
*		made with this pointer.
*
* @param	Direction will set if the transmission or the reception is stoped
*		for the I2S core. The options are : DI2S_TRANSMIT and DI2S_RECEIVE
*		For a full duplex transmission both can be set
*
* @return
* 			None.
*
* @note		None.
*
*****************************************************************************/
void DI2s_StopI2sStream(DI2s * InstancePtr, u8 Direction)
{
	Xil_AssertVoid(InstancePtr != NULL);
	Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

	if (InstancePtr->StreamCaplepable)
	{

		DI2s_BitField.l = DI2s_ReadReg(InstancePtr->BaseAddress, DI2S_STREAM_CTL_OFFSET);
		if (Direction & DI2S_TRANSMIT)
		{
			DI2s_BitField.bit.u32bit1 = 0; // DI2S_TX_STREAM_ENABLE bit
		}
		if (Direction & DI2S_RECEIVE)
		{
			DI2s_BitField.bit.u32bit0 = 0; // DI2S_RX_STREAM_ENABLE bit
		}

		DI2s_WriteReg(InstancePtr->BaseAddress, DI2S_STREAM_CTL_OFFSET, DI2s_BitField.l);
	}

}

/****************************************************************************/
/**
* This function will set the clock configuration of the I2S core.
* Depending on the desired sampling frequency the sampling rate bits
* will be set. It will check if the core is set to ouptut only and then
* determine if the it can operate in slave mode.
*
* @param	InstancePtr is a pointer to an DI2s instance. The memory the
*		pointer references must be pre-allocated by the caller. Further
*		calls to manipulate the driver through the DI2s API must be
*		made with this pointer.
*
* @param	SamplingRate can be set to either of the sampling options
*
* @param	SlaveMode sets the I2S core in to slave mode, allowing the
* 		codec to generate the BCLK and the LRCLK. Regardless of this bit
* 		the SamplingRate value must be set
*
* @return
* 			None.
*
* @note		None.
*
*****************************************************************************/
void DI2s_SetClockOptions(DI2s *InstancePtr, u8 SamplingRate, u8 SlaveMode)
{
	Xil_AssertVoid(InstancePtr != NULL);
	Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);
	DI2s_BitField.l = 0;

	//Set sampling rate bits
	if (SamplingRate > DI2S_SAMPLING_RATE_96KHZ)
	{
		DI2s_BitField.rgu8[0] = DI2S_SAMPLING_RATE_48KHZ;
	}
	else
	{
		DI2s_BitField.rgu8[0] = SamplingRate;
	}

	//Activate slave mode in the I2S core
	if (!InstancePtr->MastreMode)
	{
		if (SlaveMode)
		{
			DI2s_BitField.bit.u32bit16 = 1;
		}
		else
		{
			DI2s_BitField.bit.u32bit16 = 0;
		}
	}
	else
	{
		DI2s_BitField.bit.u32bit16 = 0;
	}

	DI2s_WriteReg(InstancePtr->BaseAddress, DI2S_CLOCK_CTL_OFFSET, DI2s_BitField.l);
}

/****************************************************************************/
/**
* This function will start recording the number of specified samples. It is
* a simple acquisition mode and will not utilize either the DMA or the
* streaming capabilities of the core
*
* @param	InstancePtr is a pointer to an DI2s instance. The memory the
*		pointer references must be pre-allocated by the caller. Further
*		calls to manipulate the driver through the DI2s API must be
*		made with this pointer.
*
* @param	NrSamples Number of samples to be recorded
*
* @param	SampleAddr specifies the Address in the memory where the
* 		acquired samples will be store.
*
* @return
* 			None.
*
* @note		None.
*
*****************************************************************************/
void DI2s_SimpleRecord(DI2s *InstancePtr, u32 u32NrSamples, u32 *SampleAddr)
{
	u32 i = 0;

	//Reset I2S core
	DI2s_Reset(InstancePtr);

	//Set the direction and start the transactions
	DI2s_StartI2s(InstancePtr, DI2S_RECEIVE);

	while (i < u32NrSamples)
	{
		if (! DI2s_GetRxFifoEmpty(InstancePtr))
		{
			//activate RX_FIFO_RD_EN to load sample register
			DI2s_WriteReg(InstancePtr->BaseAddress, DI2S_FIFO_CTL_OFFSET, 0x02);
			DI2s_WriteReg(InstancePtr->BaseAddress, DI2S_FIFO_CTL_OFFSET, 0x00);

			//store the samples
			DI2s_BitField.l = DI2s_ReadReg(InstancePtr->BaseAddress, DI2S_DATA_OUT_OFFSET);
			*(SampleAddr + i) = DI2s_BitField.l;

			i++;
		}
	}

	//Stop the transactions
	DI2s_StopI2s(InstancePtr, DI2S_RECEIVE);
}

/****************************************************************************/
/**
* This function will start playing the number of specified samples. It is
* a simple playback mode and will not utilize either the DMA or the
* streaming capabilities of the core
*
* @param	InstancePtr is a pointer to an DI2s instance. The memory the
*		pointer references must be pre-allocated by the caller. Further
*		calls to manipulate the driver through the DI2s API must be
*		made with this pointer.
*
* @param	NrSamples Number of samples to be sent
*
* @param	SampleAddr specifies the Address in the memory where the
* 		stored samples are located.
*
* @return
* 			None.
*
* @note		None.
*
*****************************************************************************/
void DI2s_SimplePlayBack(DI2s *InstancePtr, u32 u32NrSamples, u32 *SampleAddr)
{
	u32 i = 0;

	//Reset I2S core
	DI2s_Reset(InstancePtr);

	//Set the direction and start the transactions
	DI2s_StartI2s(InstancePtr, DI2S_TRANSMIT);

	while (i < u32NrSamples)
	{
		if (! DI2s_GetTxFifoFull(InstancePtr))
		{
			//store the samples
			DI2s_BitField.l = *(SampleAddr + i);
			DI2s_WriteReg(InstancePtr->BaseAddress, DI2S_DATA_IN_OFFSET, DI2s_BitField.l);

			//activate TX_FIFO_WR_EN to load samples to the I2S bus
			DI2s_WriteReg(InstancePtr->BaseAddress, DI2S_FIFO_CTL_OFFSET, 0x01);
			DI2s_WriteReg(InstancePtr->BaseAddress, DI2S_FIFO_CTL_OFFSET, 0x00);

			i++;
		}
	}

	//Stop the transactions
	DI2s_StopI2s(InstancePtr, DI2S_TRANSMIT);
}

#ifdef DI2S_DMA_CAPABLE

/****************************************************************************/
/**
* This function will start playing the number of specified samples. It is
* a DMA playback mode and can only be used if the DMA is in the system
*
* @param	InstancePtr is a pointer to an DI2s instance. The memory the
*		pointer references must be pre-allocated by the caller. Further
*		calls to manipulate the driver through the DI2s API must be
*		made with this pointer.
*
* @param	DMAInstPtr is a pointer to an DMA instance. The memory the
*		pointer references must be pre-allocated by the caller. Further
*		calls to manipulate the driver through the DI2s API must be
*		made with this pointer.
*
* @param	NrSamples Number of samples to be sent
*
* @param	SampleAddr specifies the Address in the memory where the
* 		stored samples are located.
*
* @return
* 			None.
*
* @note		None.
*
*****************************************************************************/

void DI2s_DmaPlayBack(DI2s *InstancePtr,XAxiDma *DMAInstPtr,
			u32 u32NrSamples, u32 SampleAddr)
{
	u32 Status;

	Status = XAxiDma_SimpleTransfer(DMAInstPtr, SampleAddr, 5*u32NrSamples, XAXIDMA_DMA_TO_DEVICE);
	if (Status != XST_SUCCESS)
	{
		xil_printf("\n fail @ rec; ERROR: %d", Status);
	}

	//Set the direction and start the transactions
	DI2s_StartI2sStream(InstancePtr, DI2S_TRANSMIT);
}

/****************************************************************************/
/**
* This function will start recording the number of specified samples. It is
* a DMA playback mode and can only be used if the DMA is in the system
*
* @param	InstancePtr is a pointer to an DI2s instance. The memory the
*		pointer references must be pre-allocated by the caller. Further
*		calls to manipulate the driver through the DI2s API must be
*		made with this pointer.
*
* @param	DMAInstPtr is a pointer to an DMA instance. The memory the
*		pointer references must be pre-allocated by the caller. Further
*		calls to manipulate the driver through the DI2s API must be
*		made with this pointer.
*
* @param	NrSamples Number of samples to be received
*
* @param	SampleAddr specifies the Address in the memory where the
* 		stored samples are located.
*
* @return
* 			None.
*
* @note		None.
*
*****************************************************************************/

void DI2s_DmaRecord(DI2s *InstancePtr,XAxiDma *DMAInstPtr,
			u32 u32NrSamples, u32 SampleAddr)
{
	u32 Status;

	Status = XAxiDma_SimpleTransfer(DMAInstPtr, SampleAddr, 4*u32NrSamples, XAXIDMA_DEVICE_TO_DMA);
	if (Status != XST_SUCCESS)
	{
		xil_printf("\n fail @ rec; ERROR: %d", Status);
	}

	//Set the direction and start the transactions
	DI2s_StartI2sStream(InstancePtr, DI2S_RECEIVE);
}

#endif
