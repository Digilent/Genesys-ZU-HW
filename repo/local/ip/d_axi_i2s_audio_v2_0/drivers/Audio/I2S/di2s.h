/*
 * di2s.h
 *
 *  Created on: Aug 28, 2015
 *      Author: rohegbec
 */

#ifndef DI2S_H_
#define DI2S_H_


/***************************** Include Files ********************************/

#include "xil_types.h"
#include "xil_assert.h"
#include "xstatus.h"
#include "di2s_l.h"

/************************** Constant Definitions ****************************/

#define DI2S_TRANSMIT				0x01
#define DI2S_RECEIVE				0x02

#define DI2S_SAMPLING_RATE_8KHZ		0x00
#define DI2S_SAMPLING_RATE_12KHZ	0x01
#define DI2S_SAMPLING_RATE_16KHZ	0x02
#define DI2S_SAMPLING_RATE_24KHZ	0x03
#define DI2S_SAMPLING_RATE_32KHZ	0x04
#define DI2S_SAMPLING_RATE_48KHZ	0x05
#define DI2S_SAMPLING_RATE_96KHZ	0x06

#define DI2S_MASTER_MODE			0x00
#define DI2S_SLAVE_MODE				0x01

/**************************** Type Definitions ******************************/

/**
 * This typedef contains configuration information for the device.
 */
typedef struct {
	u32 BaseAddress;	/* Device base address */
	int StreamCaplepable; /* Is the Streaming port active in the h/w*/
	int MastreMode;	/* Is it set the I2S core master */
}DI2s_Config;

/**
 * The DI2s driver instance data. The user is required to allocate a
 * variable of this type for every I2S device in the system.
 */
typedef struct {
	u32 BaseAddress;	/* Device base address */
	u32 IsReady;		/* Device is initialized and ready */
	int StreamCaplepable; /* Is the Streaming port active in the h/w*/
	int MastreMode; /* Is it set the I2S core master */
}DI2s;


/***************** Macros (Inline Functions) Definitions ********************/


#ifdef DI2S_DMA_CAPABLE

#include "../../dma/dma.h"

#endif

/************************** Function Prototypes *****************************/

XStatus DI2s_Initialize(DI2s *InstancePtr);
XStatus DI2s_CfgInitialize(DI2s * InstancePtr, DI2s_Config * Config, u32 EffectiveAddr);

u32 DI2s_ReadStatusReg(DI2s * InstancePtr);
u8 DI2s_GetTxFifoEmpty(DI2s * InstancePtr);
u8 DI2s_GetTxFifoFull(DI2s * InstancePtr);
u8 DI2s_GetRxFifoEmpty(DI2s * InstancePtr);
u8 DI2s_GetRxFifoFull(DI2s * InstancePtr);
u32 DI2s_GetSample(DI2s * InstancePtr);
void DI2s_SendSample(DI2s * InstancePtr, u32 Sample);
void DI2s_Reset(DI2s * InstancePtr);
void DI2s_StartI2s(DI2s * InstancePtr, u8 Direction);
void DI2s_StopI2s(DI2s * InstancePtr, u8 Direction);
void DI2s_StartI2sStream(DI2s * InstancePtr, u8 Direction);
void DI2s_StopI2sStream(DI2s * InstancePtr, u8 Direction);
void DI2s_SetClockOptions(DI2s *InstancePtr, u8 SamplingRate, u8 SlaveMode);
void DI2s_SimpleRecord(DI2s *InstancePtr, u32 u32NrSamples, u32 *SampleAddr);
void DI2s_SimplePlayBack(DI2s *InstancePtr, u32 u32NrSamples, u32 *SampleAddr);

#ifdef DI2S_DMA_CAPABLE
void DI2s_DmaRecord(DI2s *InstancePtr,XAxiDma *DMAInstPtr,u32 u32NrSamples, u32 SampleAddr);
void DI2s_DmaPlayBack(DI2s *InstancePtr,XAxiDma *DMAInstPtr,u32 u32NrSamples, u32 SampleAddr);
void DI2s_SetNrSamples(DI2s * InstancePtr, u32 NrSamples);
#endif

#endif /* DI2S_H_ */
