/*
 * i2c_driver.h
 *
 *  Created on: Aug 20, 2019
 *      Author: bdeac
 */

#ifndef SRC_I2C_DRIVER_I2C_DRIVER_H_
#define SRC_I2C_DRIVER_I2C_DRIVER_H_

#include "XIic.h"

void i2c_send_handler(XIic *i2c_instance);

void i2c_receive_handler(XIic *i2c_instance);

void i2c_status_handler(XIic *i2c_instance);

XStatus i2c_send_buffer(XIic *i2c_instance, int slave_addr, u8 *tx_buffer, int byte_count);

XStatus i2c_read_buffer(XIic *i2c_instance, int slave_addr, u8 *rx_buffer, int byte_count, u16 read_start_address);

XStatus i2c_init(XIic *i2c_instance);

#endif /* SRC_I2C_DRIVER_I2C_DRIVER_H_ */
