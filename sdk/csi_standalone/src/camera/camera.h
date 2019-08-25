/*
 * camera.h
 *
 *  Created on: Aug 20, 2019
 *      Author: bdeac
 */

#ifndef SRC_CAMERA_CAMERA_H_
#define SRC_CAMERA_CAMERA_H_

#include "xstatus.h"
#include "xiic.h"

/* Camera modules */
#define MIPI_A	0
#define MIPI_B	1

/* Camera related addresses */
#define CAMERA_I2C_ADDRESS	0x3C
#define CAMERA_ID_REG_ADDR	0x300A

#define CAMERA_DEFAULT_ID 	0x5640

typedef struct {
	u16 addr;
	u8 data;
}camera_config_t;

typedef enum {	MODE_720P_1280_720_60fps = 0,
				MODE_1080P_1920_1080_15fps,
				MODE_1080P_1920_1080_30fps,
				MODE_1080P_1920_1080_30fps_336M_MIPI,
				MODE_1080P_1920_1080_30fps_336M_1LANE_MIPI,
				MODE_HALFX_1080P_1920_1080_30fps_336M_MIPI,
				MODE_END } camera_mode_t;

extern const camera_config_t g_camera_config[];

XStatus camera_get_id(XIic *i2c_instance, u8 camera_module, u16 *camera_id);
XStatus camera_init(XIic *i2c_instance, u8 camera_module);
XStatus camera_write_reg(XIic *i2c_instance, u16 reg_addr, u8 reg_data);
XStatus camera_set_mode(XIic *i2c_instance, u8 camera_module, camera_mode_t camera_mode);

#endif /* SRC_CAMERA_CAMERA_H_ */
