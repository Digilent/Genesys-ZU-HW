/*
 * ssm2603.h
 *
 *  Created on: Aug 31, 2015
 *      Author: rohegbec
 */

#ifndef SSM2603_H_
#define SSM2603_H_

#include "xparameters.h"
#include "xil_io.h"
#include "xiicps.h"
#include "xil_printf.h"
#include "xil_cache.h"
#include "xstatus.h"
#include "sleep.h"


//I2C Serial Clock frequency in Hertz
#define IIC_SCLK_RATE			400000

//SLave address of the ADAU audio controller
#define IIC_SLAVE_ADDR			0x1A

//SSM internal register addresses
enum ssmRegisterAdresses {
	R0_LEFT_ADC_IN_VOL      = 0x00,
		R1_RIGHT_ADC_IN_VOL     = 0x01,
		R2_LEFT_DAC_VOL         = 0x02,
		R3_RIGHT_DAC_VOL        = 0x03,
		R4_ANALOG_PATH          = 0x04,
		R5_DIGITAL_PATH         = 0x05,
		R6_POWER_MANAGEMENT     = 0x06,
		R7_DIGITAL_I_F          = 0x07,
		R8_SAMPLING_RATE        = 0x08,
		R9_ACTIVE               = 0x09,
		R15_SOFT_RESET          = 0x0F,
		R16_ALC_CONTROL_1       = 0x10,
		R17_ALC_CONTROL_2       = 0x11,
		R18_NOIS_GATE           = 0x12
};

/************************** Function Definitions *****************************/

u8 IicConfig(unsigned int DeviceId);
XStatus SSM2603_WriteToReg(u8 u8RegAddr, u8 Bit8, u8 u8Data);
XStatus SSM2603_ReadFromReg(u8 u8RegAddr, u8 *u8RxData);
XStatus SSM2603_InitAudio ();
void SSM2603_SetMicInput();
void SSM2603_SetLineInput();
void SSM2603_OutVolUp();
void SSM2603_OutVolDown();

#endif /* SSM2603_H_ */
