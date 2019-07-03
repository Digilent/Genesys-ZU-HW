/*
 * ssm2603.c
 *
 *  Created on: Aug 31, 2015
 *      Author: rohegbec
 */


#include "ssm2603.h"

/************************** Variable Definitions *****************************/

extern u8 u8Verbose;
XIicPs Iic;

/******************************************************************************
 * Configures the I2C controller.
 *
 * @param	DeviceId is the controller's ID (from xparameters_ps.h)
 *
 * @return	none.
 *****************************************************************************/
u8 IicConfig(unsigned int DeviceId)
{

	XIicPs_Config *Config;
	int Status;

	//Initialize the IIC driver so that it's ready to use
	//Look up the configuration in the config table, then initialize it.
	Config = XIicPs_LookupConfig(DeviceId);
	if(NULL == Config) {
		return XST_FAILURE;
	}

	Status = XIicPs_CfgInitialize(&Iic, Config, Config->BaseAddress);
	if(Status != XST_SUCCESS) {
		return XST_FAILURE;
	}

	//Set the IIC serial clock rate.
	XIicPs_SetSClk(&Iic, IIC_SCLK_RATE);

	return XST_SUCCESS;
}

/******************************************************************************
 * Function to write one byte (8-bits) to one of the registers from the audio
 * controller.
 *
 * @param	u8RegAddr is the LSB part of the register address (0x40xx).
 * @param	u8Data is the data byte to write.
 *
 * @return	XST_SUCCESS if all the bytes have been sent to Controller.
 * 			XST_FAILURE otherwise.
 *****************************************************************************/
XStatus SSM2603_WriteToReg(u8 u8RegAddr, u8 Bit8, u8 u8Data)
{
	u8 u8TxData[2];
	u8 u8BytesSent;

	u8TxData[0] = (u8RegAddr<<1);
	u8TxData[0] |= Bit8;
	u8TxData[1] = u8Data;

	u8BytesSent = XIicPs_MasterSendPolled(&Iic, u8TxData, 2, (u16)IIC_SLAVE_ADDR);
	while(XIicPs_BusIsBusy(&Iic));

	return u8BytesSent;
}

/******************************************************************************
 * Function to read one byte (8-bits) from the register space of audio controller.
 *
 * @param	u8RegAddr is the LSB part of the register address (0x40xx).
 * @param	u8RxData is the returned value
 *
 * @return	XST_SUCCESS if the desired number of bytes have been read from the controller
 * 			XST_FAILURE otherwise
 *****************************************************************************/
XStatus SSM2603_ReadFromReg(u8 u8RegAddr, u8 *u8RxData) {

	u8 u8TxData;
	u8 u8BytesSent;

	u8TxData = (u8RegAddr<<1);

	u8BytesSent = XIicPs_MasterSendPolled(&Iic, &u8TxData, 1, (u16) IIC_SLAVE_ADDR);
	while(XIicPs_BusIsBusy(&Iic));

	u8BytesSent = XIicPs_MasterRecvPolled(&Iic, u8RxData, 2,(u16) IIC_SLAVE_ADDR);
	while(XIicPs_BusIsBusy(&Iic));

	return u8BytesSent;
}
/******************************************************************************
 * Configure the initial settings of the audio controller, the majority of
 * these will remain unchanged during the normal functioning of the code.
 * In order to generate a correct BCLK and LRCK, which are crucial for the
 * correct operating of the controller, the sampling rate must me set in the
 * I2S_TRANSFER_CONTROL_REG. The sampling rate options are:
 *    "000" -  8 KHz
 *    "001" - 12 KHz
 *    "010" - 16 KHz
 *    "011" - 24 KHz
 *    "100" - 32 KHz
 *    "101" - 48 KHz
 *    "110" - 96 KHz
 * These options are valid only if the I2S controller is in slave mode.
 * When In master mode the ADAU will generate the appropriate BCLK and LRCLK
 * internally, and the sampling rates which will be set in the I2S_TRANSFER_CONTROL_REG
 * are ignored.
 *
 * @param	none.
 *
 * @return	XST_SUCCESS if the configuration is successful
 *****************************************************************************/
XStatus SSM2603_InitAudio ()
{

	int Status;

	Status = IicConfig(XPAR_PS7_I2C_0_DEVICE_ID);
	if(Status != XST_SUCCESS)
	{
		xil_printf("\nError: 0x%x", Status);
		return XST_FAILURE;
	}

	Status = SSM2603_WriteToReg(R15_SOFT_RESET, 0x00, 0x00);
	if (Status == XST_FAILURE)
	{
		xil_printf("\r\nError: could not write R15_SOFT_RESET (0x00, 0x00)");
		return XST_FAILURE;
	}

	//Recommended time to wait until reste is done
	usleep(1000);

	// Power-up everything except the output and the oscillator
	Status = SSM2603_WriteToReg(R6_POWER_MANAGEMENT, 0x00, 0x30);
	if (Status == XST_FAILURE)
	{
		xil_printf("\r\nError: could not write R6_POWER_MANAGEMENT (0x00, 0x30)");
		return XST_FAILURE;
	}

	//set LIN volume to 0 dB
	Status = SSM2603_WriteToReg(R0_LEFT_ADC_IN_VOL, 0x00, 0x17); // 0x97
	if (Status == XST_FAILURE)
	{
		xil_printf("\r\nError: could not write R0_LEFT_ADC_IN_VOL (0x00, 0x17)");
		return XST_FAILURE;
	}

	//set LIN volume to 0 dB
	Status = SSM2603_WriteToReg(R1_RIGHT_ADC_IN_VOL, 0x00, 0x17);// 0x97
	if (Status == XST_FAILURE)
	{
		xil_printf("\r\nError: could not write R1_RIGHT_ADC_IN_VOL (0x00, 0x17)");
		return XST_FAILURE;
	}

	//set HP volume to 0dB
	Status = SSM2603_WriteToReg(R2_LEFT_DAC_VOL, 0x01, 0x79);// 0x79
	if (Status == XST_FAILURE)
	{
		xil_printf("\r\nError: could not write R2_LEFT_DAC_VOL (0x01, 0x79)");
		return XST_FAILURE;
	}

	//set LIN as input and activate DAC output
	Status = SSM2603_WriteToReg(R4_ANALOG_PATH, 0x00, 0x10);// 0x0A
	if (Status == XST_FAILURE)
	{
		xil_printf("\r\nError: could not write R4_ANALOG_PATH (0x00, 0x10)");
		return XST_FAILURE;
	}

	//set the de-emphasis to 48 kHz sampling rate
	Status = SSM2603_WriteToReg(R5_DIGITAL_PATH, 0x00, 0x06);//0x08
	if (Status == XST_FAILURE)
	{
		xil_printf("\r\nError: could not write R5_DIGITAL_PATH (0x00, 0x06)");
		return XST_FAILURE;
	}

	//set to slave mode 32 bits and I2S mode
	Status = SSM2603_WriteToReg(R7_DIGITAL_I_F, 0x00, 0x0E);//0x0A
	if (Status == XST_FAILURE)
	{
		xil_printf("\r\nError: could not write R7_DIGITAL_I_F (0x00, 0x0E)");
		return XST_FAILURE;
	}

	//set sampling rate and clock control to 12,288MHz 48kHz
	Status = SSM2603_WriteToReg(R8_SAMPLING_RATE, 0x00, 0x00);//0x1c
	if (Status == XST_FAILURE)
	{
		xil_printf("\r\nError: could not write R8_SAMPLING_RATE (0x00, 0x00)");
		return XST_FAILURE;
	}

	//wait for changes to take effect
	usleep(75000);

	// activate the digital core
	Status = SSM2603_WriteToReg(R9_ACTIVE, 0x00, 0x01);//0x00
	if (Status == XST_FAILURE)
	{
		xil_printf("\r\nError: could not write R9_ACTIVE (0x00, 0x01)");
		return XST_FAILURE;
	}

	// enable all transactions
	Status = SSM2603_WriteToReg(R6_POWER_MANAGEMENT, 0x00, 0x00);//0x9F
	if (Status == XST_FAILURE)
	{
		xil_printf("\r\nError: could not write R6_POWER_MANAGEMENT (0x00, 0x00)");
		return XST_FAILURE;
	}

	return XST_SUCCESS;

}

/******************************************************************************
 * Configure the input path to MIC and disables all other input paths.
 * For additional information pleas refer to the SSM2603 datasheet
 *
 * @param	none
 *
 * @return	none.
 *****************************************************************************/
void SSM2603_SetMicInput()
{
	SSM2603_WriteToReg(R4_ANALOG_PATH, 0x00, 0x14);
}

/******************************************************************************
 * Configure the input path to Line and disables all other input paths
 * For additional information pleas refer to the SSM2603 datasheet
 *
 * @param	none
 *
 * @return	none.
 *****************************************************************************/
void SSM2603_SetLineInput()
{
	SSM2603_WriteToReg(R4_ANALOG_PATH, 0x00, 0x10);
}

/******************************************************************************
 * This function increases the Head phones output volume by 1dB
 * For additional information pleas refer to the SSM2603 datasheet
 *
 * @param	none
 *
 * @return	none.
 *****************************************************************************/
void SSM2603_OutVolUp()
{
	u8 TempVal[2];
	SSM2603_ReadFromReg(R2_LEFT_DAC_VOL, TempVal);
	if (TempVal[0] < 0x7F)
	{
		TempVal[0]++;
	}

	SSM2603_WriteToReg(R2_LEFT_DAC_VOL, TempVal[1], TempVal[0]);
	SSM2603_WriteToReg(R3_RIGHT_DAC_VOL, TempVal[1], TempVal[0]);
}

/******************************************************************************
 * This function decreases the Head phones output volume by 1dB
 * For additional information pleas refer to the SSM2603 datasheet
 *
 * @param	none
 *
 * @return	none.
 *****************************************************************************/
void SSM2603_OutVolDown()
{
	u8 TempVal[2];
	SSM2603_ReadFromReg(R2_LEFT_DAC_VOL, TempVal);
	if (TempVal[0] > 0x00)
	{
		TempVal[0]--;
	}

	SSM2603_WriteToReg(R2_LEFT_DAC_VOL, TempVal[1], TempVal[0]);
	SSM2603_WriteToReg(R3_RIGHT_DAC_VOL, TempVal[1], TempVal[0]);
}
