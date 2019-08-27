/*
 * i2c_mux.h
 *
 *  Created on: Aug 20, 2019
 *      Author: bdeac
 */

#ifndef SRC_I2C_MUX_I2C_MUX_H_
#define SRC_I2C_MUX_I2C_MUX_H_

#include "xstatus.h"
#include "xiic.h"

#define I2C_MUX_ADDR	112

/* What do we find on each MUX channel */
#define CAMERA_MIPI_A	0x01
#define CAMERA_MIPI_B	0X02

XStatus i2c_mux_init(XIic *i2c_instance, int slave_addr, u8 mux_channel);

#endif /* SRC_I2C_MUX_I2C_MUX_H_ */
