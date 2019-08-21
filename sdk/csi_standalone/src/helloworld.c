/******************************************************************************
*
* Copyright (C) 2009 - 2014 Xilinx, Inc.  All rights reserved.
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
*
* Use of the Software is limited solely to applications:
* (a) running on a Xilinx device, or
* (b) that interact with a Xilinx device through a bus or interconnect.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
* XILINX  BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
* WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF
* OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
*
* Except as contained in this notice, the name of the Xilinx shall not be used
* in advertising or otherwise to promote the sale, use or other dealings in
* this Software without prior written authorization from Xilinx.
*
******************************************************************************/

/*
 * helloworld.c: simple test application
 *
 * This application configures UART 16550 to baud rate 9600.
 * PS7 UART (Zynq) is not initialized by this application, since
 * bootrom/bsp configures it to baud rate 115200
 *
 * ------------------------------------------------
 * | UART TYPE   BAUD RATE                        |
 * ------------------------------------------------
 *   uartns550   9600
 *   uartlite    Configurable only in HW design
 *   ps7_uart    115200 (configured by bootrom/bsp)
 */

#include <stdio.h>
#include "platform.h"
#include "xil_printf.h"
#include "xcsiss.h"
#include "xscugic.h"
#include "xiicps.h"
#include "xil_io.h"
#include "sleep.h"
#include "xdphy.h"

#include "intr/intr.h"
#include "i2c_mux/i2c_mux.h"
#include "i2c_driver/i2c_driver.h"
#include "camera/camera.h"

int main()
{
    init_platform();

    print("MIPI CSI Standalone test\n\r");

    XCsiSs_Config *csi_config = NULL;
    XCsiSs csi_instance;
    u32 status = XST_SUCCESS;
    XIic i2c_camera;

    status = mipi_csi_rx_init();
    if (XST_SUCCESS != status) {
    	print("CSI module initialization FAILED!\n\r");
    	return -1;
    }

    status = i2c_init(&i2c_camera);
    if (XST_SUCCESS != status) {
    	print("i2c get instance FAILED!\n\r");
    	return -1;
    }

    status = setup_interrupt_system(&i2c_camera);
    if (XST_SUCCESS != status) {
    	print("Interrupt System initialization FAILED!\n\r");
    	return -1;
    }

    /* Setup the handlers for i2c */
    XIic_SetSendHandler(&i2c_camera, &i2c_camera, (XIic_Handler)i2c_send_handler);
    XIic_SetRecvHandler(&i2c_camera, &i2c_camera, (XIic_Handler)i2c_receive_handler);
    XIic_SetStatusHandler(&i2c_camera, &i2c_camera, (XIic_Handler)i2c_status_handler);

    u16 camera_id = 0x0;
    status = camera_get_id(&i2c_camera, MIPI_A, &camera_id);

    printf("Camera ID: %x\n\r", camera_id);

    /* Initialize the camera module */
    status = camera_init(&i2c_camera, MIPI_A);
    if (XST_SUCCESS != status) {
    	print("Camera init FAILED\n\r");
    	return -1;
    }
    print("Camera init DONE!\n\r");

    /* Set the camera mode */
    status = camera_set_mode(&i2c_camera, MODE_HALFX_1080P_1920_1080_30fps_336M_MIPI);
    if (XST_SUCCESS != status) {
    	print("Camera set mode FAILED!\n\r");
    	return -1;
    }
    print("Camera set mode DONE!\n\r");

    usleep(100000);

    /* Get the data lane status */
    u32 dl0_status = Xil_In32(XPAR_DPHY_0_BASEADDR + 0x1C);
    u32 dl1_status = Xil_In32(XPAR_DPHY_0_BASEADDR + 0x20);
    u32 dl2_status = Xil_In32(XPAR_DPHY_0_BASEADDR + 0x24);
    u32 dl3_status = Xil_In32(XPAR_DPHY_0_BASEADDR + 0x28);

    printf("Data Lane 0 status: %x\n\r", dl0_status);
    printf("Data Lane 1 status: %x\n\r", dl1_status);
    printf("Data Lane 2 status: %x\n\r", dl2_status);
    printf("Data Lane 3 status: %x\n\r", dl3_status);

    /* Get the clock lane status */
    u32 cl_status = Xil_In32(XPAR_DPHY_0_BASEADDR + 0x18);

    printf("Clock Lane status: %x\n\r", cl_status);

    /* Get some info about the MIPI CSI subsystem */
    XCsiSs_ReportCoreInfo(&csi_instance);
    XCsiSs_GetLaneInfo(&csi_instance);

    cleanup_platform();
    return 0;
}
