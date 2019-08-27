/*
 * i2c_mux.c
 *
 *  Created on: Aug 20, 2019
 *      Author: bdeac
 */

#include "i2c_mux.h"
#include "xil_types.h"
#include "i2c_driver/i2c_driver.h"

#define ONE_BYTE_SIZE_BUFFER	1

XStatus i2c_mux_init(XIic *i2c_instance, int slave_addr, u8 mux_channel) {
	XStatus status = XST_SUCCESS;

	u8 tx_buffer[ONE_BYTE_SIZE_BUFFER] = {mux_channel};
	status = i2c_send_buffer(i2c_instance, slave_addr, tx_buffer, sizeof(tx_buffer));
	if (XST_SUCCESS != status) {
		return status;
	}

	return XST_SUCCESS;
}
