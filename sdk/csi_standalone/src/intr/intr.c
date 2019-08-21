/*
 * intr.c
 *
 *  Created on: Aug 20, 2019
 *      Author: bdeac
 */

#include "intr.h"

static volatile XScuGic_Config *gp_scugic_config = NULL;
static volatile XScuGic g_scugic_instance;

XStatus setup_interrupt_system(XIic *i2c_instance) {
    /* Configure the Interrupt System */
	gp_scugic_config = XScuGic_LookupConfig(XPAR_SCUGIC_0_DEVICE_ID);
    if (NULL == gp_scugic_config) {
    	return XST_FAILURE;
    }

    XStatus status = XST_SUCCESS;
    /* Initialize the Interrupt System */
	XScuGic_CfgInitialize(&g_scugic_instance, gp_scugic_config, gp_scugic_config->CpuBaseAddress);
	if (XST_SUCCESS != status) {
		return status;
	}

	/* SCUGIC Selftest */
	status = XScuGic_SelfTest(&g_scugic_instance);
	if (XST_SUCCESS != status) {
		return status;
	}

	/*
	 * Connect the interrupt handler that will be called when an
	 * interrupt occurs for the device.
	 */
	status = XScuGic_Connect(&g_scugic_instance, XPAR_FABRIC_IIC_0_VEC_ID, (Xil_InterruptHandler)XIic_InterruptHandler, i2c_instance);
	if (XST_SUCCESS != status) {
		return status;
	}

	/* Enable the interrupt for the i2c device */
	XScuGic_Enable(&g_scugic_instance, XPAR_FABRIC_IIC_0_VEC_ID);

    /* Connect the interrupt controller interrupt handler to the hardware
    	interrupt handling logic in the ARM processor. */
    Xil_ExceptionRegisterHandler(XIL_EXCEPTION_ID_INT, (Xil_ExceptionHandler)XScuGic_InterruptHandler, &g_scugic_instance);

    /* Enable the interrupts in the Processor */
    Xil_ExceptionEnable();

	return XST_SUCCESS;
}

