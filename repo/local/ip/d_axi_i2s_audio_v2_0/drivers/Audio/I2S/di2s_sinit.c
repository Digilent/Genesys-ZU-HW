/*
 * di2s_sinit.c
 *
 *  Created on: Aug 28, 2015
 *      Author: rohegbec
 */


/***************************** Include Files ********************************/

#include "di2s_i.h"

/************************** Constant Definitions ****************************/

/**************************** Type Definitions ******************************/

/***************** Macros (Inline Functions) Definitions ********************/

/************************** Variable Definitions ****************************/

/************************** Function Prototypes *****************************/

/****************************************************************************/
/**
* Initialize the DI2s instance.*Nothing is done except to initialize the InstancePtr.
*
* @param	InstancePtr is a pointer to an DI2s instance. The memory the
*		pointer references must be pre-allocated by the caller. Further
*		calls to manipulate the instance/driver through the XGpio API
*		must be made with this pointer.
*
* @return
*		- XST_SUCCESS if the initialization was successful.
* 		- XST_DEVICE_NOT_FOUND  if the device configuration data was not
*		found for a device with the supplied device ID.
*
* @note		None.
*
*****************************************************************************/
XStatus DI2s_Initialize(DI2s *InstancePtr)
{
	DI2s_Config *ConfigPtr = &DI2s_ConfigTable[0];

	/*
	 * Assert arguments
	 */
	Xil_AssertNonvoid(InstancePtr != NULL);

	if (ConfigPtr == (DI2s_Config *) NULL)
	{
		InstancePtr->IsReady = 0;
		return (XST_DEVICE_NOT_FOUND);
	}

	return DI2s_CfgInitialize(InstancePtr, ConfigPtr,
			   ConfigPtr->BaseAddress);
}

