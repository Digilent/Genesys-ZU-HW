/*
 * mipi_csi_rx.c
 *
 *  Created on: Aug 21, 2019
 *      Author: bdeac
 */

#include "mipi_csi_rx.h"

#include "xcsiss.h"
#include "xcsiss_hw.h"

static XCsiSs g_csiss_instance;
volatile interrupt_counts;

/*****************************************************************************/
/**
*
* This function is called when a DPHY level error event is received by
* the MIPI CSI Rx Subsystem core.
*
* @param	InstancePtr is a pointer to the XCsiSs instance.
* @param	Mask of interrupt which caused this event
*
* @return	None.
*
* @note		Use the XCsiSs_SetCallback driver function to set this
*		function as the handler for DPHY level error event.
*
******************************************************************************/
void CsiSs_DphyEventHandler(void *InstancePtr, u32 Mask)
{
	xil_printf("+===> DPHY Level Error detected.\n\r");
	interrupt_counts++;

	if (Mask & XCSISS_ISR_SOTERR_MASK) {
		xil_printf("Start of Transmission Error\n\r");
	}

	if (Mask & XCSISS_ISR_SOTSYNCERR_MASK) {
		xil_printf("Start of Transmission "
		"Sync Error \n\r");
	}
}

/*****************************************************************************/
/**
*
* This function is called when a Packet level error event is received by
* the MIPI CSI Rx Subsystem core.
*
* @param	InstancePtr is a pointer to the XCsiSs instance.
* @param	Mask of interrupt which caused this event
*
* @return	None.
*
* @note		Use the XCsiSs_SetCallback driver function to set this
*		function as the handler for Packet level error event.
*
******************************************************************************/
void CsiSs_PktLvlEventHandler(void *InstancePtr, u32 Mask)
{
	xil_printf("+===> Packet Level Error detected.\n\r");
	interrupt_counts++;

	if (Mask & XCSISS_ISR_ECC2BERR_MASK) {
		xil_printf("2 bit ECC Error \n\r");
	}

	if (Mask & XCSISS_ISR_ECC1BERR_MASK) {
		xil_printf("1 bit ECC Error \n\r");
	}

	if (Mask & XCSISS_ISR_CRCERR_MASK) {
		xil_printf("Frame CRC Error \n\r");
	}

	if (Mask & XCSISS_ISR_DATAIDERR_MASK) {
		xil_printf("Data Id Error \n\r");
	}
}

/*****************************************************************************/
/**
*
* This function is called when a Protocol decoding level error event is
* received by the MIPI CSI Rx Subsystem core.
*
* @param	InstancePtr is a pointer to the XCsiSs instance.
* @param	Mask of interrupt which caused this event
*
* @return	None.
*
* @note		Use the XCsiSs_SetCallback driver function to set this
*		function as the handler for Protocol Decoding level error
*
******************************************************************************/
void CsiSs_ProtLvlEventHandler(void *InstancePtr, u32 Mask)
{
	xil_printf("+===> Packet Level Error detected.\n\r");
	interrupt_counts++;

	if (Mask & XCSISS_ISR_VC3FSYNCERR_MASK) {
		xil_printf("VC3 Frame Sync Error \n\r");
	}

	if (Mask & XCSISS_ISR_VC2FSYNCERR_MASK) {
		xil_printf("VC2 Frame Sync Error \n\r");
	}

	if (Mask & XCSISS_ISR_VC1FSYNCERR_MASK) {
		xil_printf("VC1 Frame Sync Error \n\r");
	}

	if (Mask & XCSISS_ISR_VC0FSYNCERR_MASK) {
		xil_printf("VC0 Frame Sync Error \n\r");
	}

	if (Mask & XCSISS_ISR_VC3FLVLERR_MASK) {
		xil_printf("VC3 Frame Level Error \n\r");
	}

	if (Mask & XCSISS_ISR_VC2FLVLERR_MASK) {
		xil_printf("VC2 Frame Level Error \n\r");
	}

	if (Mask & XCSISS_ISR_VC1FLVLERR_MASK) {
		xil_printf("VC1 Frame Level Error \n\r");
	}

	if (Mask & XCSISS_ISR_VC0FLVLERR_MASK) {
		xil_printf("VC0 Frame Level Error \n\r");
	}
}

/*****************************************************************************/
/**
*
* This function is called when a Other errors event is received by the
* MIPI CSI Rx Subsystem core.
*
* @param	InstancePtr is a pointer to the XCsiSs instance.
* @param	Mask of interrupt which caused this event
*
* @return	None.
*
* @note		Use the XCsiSs_SetCallback driver function to set this
*		function as the handler for Other error event.
*
******************************************************************************/
void CsiSs_ErrEventHandler(void *InstancePtr, u32 Mask)
{
	XCsiSs *CsiSsInstance = (XCsiSs *)InstancePtr;

	xil_printf("+===> Other Errors detected.\n\r");
	interrupt_counts++;

	if (Mask & XCSISS_ISR_WC_MASK) {
		xil_printf("Word count corruption Error\n\r");
	}

	if (Mask & XCSISS_ISR_ILC_MASK) {
		xil_printf("Incorrect Lane Count Error \n\r");
	}

	if (Mask & XCSISS_ISR_SLBF_MASK) {
		xil_printf("Stream line buffer full Error \n\r");
	}

	if (Mask & XCSISS_ISR_STOP_MASK) {
		xil_printf("Stop Error \n\r");
		XCsiSs_IntrDisable(CsiSsInstance, XCSISS_ISR_STOP_MASK);
	}
}

/*****************************************************************************/
/**
*
* This function is called when a Short Packet FIFO event is received by
* the MIPI CSI Rx Subsystem core.
*
* @param	InstancePtr is a pointer to the XCsiSs instance.
*
* @param	Mask of interrupt which caused this event
*
* @return	None.
*
* @note		Use the XCsiSs_SetCallback driver function to set this
*		function as the handler for Short Packet FIFO error event.
*
******************************************************************************/
void CsiSs_SPktEventHandler(void *InstancePtr, u32 Mask)
{
	XCsiSs *CsiSsInstance = (XCsiSs *)InstancePtr;
	u32 IntrMask;

	xil_printf("+===> Short Packet Event detected.\n\r");
	interrupt_counts++;

	if (Mask & XCSISS_ISR_SPFIFONE_MASK) {
		xil_printf("Fifo not empty \n\r");
		XCsiSs_GetShortPacket(InstancePtr);
		xil_printf("Data Type = 0x%x,"
		"Virtual Channel = 0x%x, Data = 0x%x",
			CsiSsInstance->SpktData.DataType,
			CsiSsInstance->SpktData.VirtualChannel,
			CsiSsInstance->SpktData.Data);
	}

	if (Mask & XCSISS_ISR_SPFIFOF_MASK) {
		xil_printf("Fifo Full \n\r");
	}
}

/*****************************************************************************/
/**
*
* This function is called when a Frame is received by the MIPI CSI Rx
* Subsystem core.
*
* @param	InstancePtr is a pointer to the XCsiSs instance.
*
* @param	Mask of interrupt which caused this event
*
* @return	None.
*
* @note		Use the XCsiSs_SetCallback driver function to set this
*		function as the handler for Frame Receieved event.
*
******************************************************************************/
void CsiSs_FrameRcvdEventHandler(void *InstancePtr, u32 Mask)
{
	xil_printf("+=> Frame Receieved Event detected.\n\r");
	interrupt_counts++;
}

XStatus mipi_csi_rx_init(void) {
	XStatus status = XST_SUCCESS;
	XCsiSs_Config *csiss_config = NULL;

	csiss_config = XCsiSs_LookupConfig(XPAR_CSI_0_DEVICE_ID);
	if (NULL == csiss_config) {
		print("CSI Lookup config FAILED!\n\r");
		return XST_FAILURE;
	}

	status = XCsiSs_CfgInitialize(&g_csiss_instance, csiss_config, csiss_config->BaseAddr);
	if (XST_SUCCESS != status) {
		print("CSI Initialization FAILED!\n\r");
		return XST_FAILURE;
	}

	/* Dump the configuration */
	XCsiSs_ReportCoreInfo(&g_csiss_instance);

	/* Reset the subsystem */
	status = XCsiSs_Reset(&g_csiss_instance);
	if (XST_SUCCESS != status) {
		print("CSI Reset FAILED!\n\r");
		return XST_FAILURE;
	}

	/* Disable the subsystem till the camera
	 * and interrupts are configured
	 */
	XCsiSs_Activate(&g_csiss_instance, 0);
	if (XST_SUCCESS != status) {
		print("CSI Disabling FAILED!\n\r");
		return XST_FAILURE;
	}

	/* Configure the subsystem for ActiveLanes and Interrupts
	 * The minimum value of ActiveLanes is 0 and max value is
	 * maximum lanes set in the design (max 3).
	 * The interrupt mask can be selected from the bitmasks in
	 * xcsiss_hw.h
	 */
	u8 ActiveLanes = XPAR_CSI_0_CSI_LANES;
	u32 IntrRequest = XCSISS_ISR_ALLINTR_MASK;

	status = XCsiSs_Configure(&g_csiss_instance, ActiveLanes, IntrRequest);
	if (XST_SUCCESS != status) {
		print("CSI Configuration FAILED!\n\r");
		return XST_FAILURE;
	}

	return XST_SUCCESS;
}

XStatus mipi_csi_rx_set_intr(void) {
	XStatus status = XST_SUCCESS;

	/* Set the HPD interrupt handlers. */
	XCsiSs_SetCallBack(&g_csiss_instance, XCSISS_HANDLER_DPHY,
				CsiSs_DphyEventHandler, &g_csiss_instance);
	XCsiSs_SetCallBack(&g_csiss_instance, XCSISS_HANDLER_PKTLVL,
				CsiSs_PktLvlEventHandler, &g_csiss_instance);
	XCsiSs_SetCallBack(&g_csiss_instance, XCSISS_HANDLER_PROTLVL,
				CsiSs_ProtLvlEventHandler, &g_csiss_instance);
	XCsiSs_SetCallBack(&g_csiss_instance, XCSISS_HANDLER_SHORTPACKET,
				CsiSs_SPktEventHandler, &g_csiss_instance);
	XCsiSs_SetCallBack(&g_csiss_instance, XCSISS_HANDLER_FRAMERECVD,
				CsiSs_FrameRcvdEventHandler, &g_csiss_instance);
	XCsiSs_SetCallBack(&g_csiss_instance, XCSISS_HANDLER_OTHERERROR,
				CsiSs_ErrEventHandler, &g_csiss_instance);

	return XST_SUCCESS;
}
