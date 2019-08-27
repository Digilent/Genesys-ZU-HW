/*
 * i2c_driver.c
 *
 *  Created on: Aug 20, 2019
 *      Author: bdeac
 */

#include "i2c_driver.h"

static volatile u8 g_transmit_complete = 0;
static volatile u8 g_receive_complete = 0;

void i2c_send_handler(XIic *i2c_instance){
	g_transmit_complete = 0;
}

void i2c_receive_handler(XIic *i2c_instance){
	g_receive_complete = 0;
}

void i2c_status_handler(XIic *i2c_instance){

}

XStatus i2c_send_buffer(XIic *i2c_instance, int slave_addr, u8 *tx_buffer, int byte_count) {
	XStatus status = XST_SUCCESS;

	g_transmit_complete = 1;

	/* Set the slave address */
	status = XIic_SetAddress(i2c_instance, XII_ADDR_TO_SEND_TYPE, slave_addr);
	if (XST_SUCCESS != status) {
		return status;
	}

	/* Start the i2c device */
	status = XIic_Start(i2c_instance);
	if (XST_SUCCESS != status) {
		return status;
	}

	/* Send the data */
	status = XIic_MasterSend(i2c_instance, tx_buffer, byte_count);
	if (XST_SUCCESS != status) {
		return status;
	}

	/* Wait until the transmission is completed */
	while (g_transmit_complete || (XIic_IsIicBusy(i2c_instance) == TRUE)) {
		;
	}

	/* Stop the i2c device */
	status = XIic_Stop(i2c_instance);
	if (XST_SUCCESS != status) {
		return status;
	}

	return XST_SUCCESS;
}

XStatus i2c_read_buffer(XIic *i2c_instance, int slave_addr, u8 *rx_buffer, int byte_count, u16 read_start_address) {
	g_receive_complete = 1;

	XStatus status = XST_SUCCESS;

	u8 tx_buffer[2] = {0};
	tx_buffer[0] = ((read_start_address >> 8) & 0xFF);
	tx_buffer[1] = (read_start_address & 0xFF);
	/* Set the address that we want to read from */
	status = i2c_send_buffer(i2c_instance, slave_addr, tx_buffer, sizeof(tx_buffer));
	if (XST_SUCCESS != status) {
		return XST_FAILURE;
	}

	/* Start the i2c device */
	status = XIic_Start(i2c_instance);
	if (XST_SUCCESS != status) {
		return status;
	}

	/* Receive the data */
	status = XIic_MasterRecv(i2c_instance, rx_buffer, byte_count);
	if (XST_SUCCESS != status) {
		return XST_FAILURE;
	}

	/* Wait till all the data is received */
	while (g_receive_complete || (XIic_IsIicBusy(i2c_instance) == TRUE)) {
		;
	}

	/* Stop the i2c device */
	status = XIic_Stop(i2c_instance);
	if (XST_SUCCESS != status) {
		return status;
	}

	return XST_SUCCESS;
}

XStatus i2c_init(XIic *i2c_instance) {
	XIic_Config *i2c_config = NULL;

	i2c_config = XIic_LookupConfig(XPAR_IIC_0_DEVICE_ID);
	if (NULL == i2c_config) {
		return XST_FAILURE;
	}

	XStatus status = XST_SUCCESS;
	status = XIic_CfgInitialize(i2c_instance, i2c_config, i2c_config->BaseAddress);
	if (XST_SUCCESS != status) {
		return XST_FAILURE;
	}

	return XST_SUCCESS;
}
