/*
 * camera.c
 *
 *  Created on: Aug 20, 2019
 *      Author: bdeac
 */

#include "camera.h"

#include <stdio.h>
#include "sleep.h"

#include "i2c_mux/i2c_mux.h"
#include "i2c_driver/i2c_driver.h"

const camera_config_t g_camera_config[] = {
	//[7]=1 Software reset; [6]=1 Software power down; Default=0x02
	// We apply software power down
	{0x3008, 0x42},
	//[1]=1 System input clock from PLL; Default read = 0x11
	{0x3103, 0x03},
	//[3:0]=0000 MD2P,MD2N,MCP,MCN input; Default=0x00
	{0x3017, 0x00},
	//[7:2]=000000 MD1P,MD1N, D3:0 input; Default=0x00
	{0x3018, 0x00},
	//[6:4]=001 PLL charge pump, [3:0]=1000 MIPI 8-bit mode
	{0x3034, 0x18},
	//PLL1 configuration
	//[7:4]=0001 System clock divider /1, [3:0]=0001 Scale divider for MIPI /1
	{0x3035, 0x11},
	//[7:0]=56 PLL multiplier
	{0x3036, 0x38},
	//[4]=1 PLL root divider /2, [3:0]=1 PLL pre-divider /1
	{0x3037, 0x11},
	//[5:4]=00 PCLK root divider /1, [3:2]=00 SCLK2x root divider /1, [1:0]=01 SCLK root divider /2
	{0x3108, 0x01},
	//PLL2 configuration
	//[5:4]=01 PRE_DIV_SP /1.5, [2]=1 R_DIV_SP /1, [1:0]=00 DIV12_SP /1
	{0x303D, 0x10},
	//[4:0]=11001 PLL2 multiplier DIV_CNT5B = 25
	{0x303B, 0x19},

	{0x3630, 0x2e},
	{0x3631, 0x0e},
	{0x3632, 0xe2},
	{0x3633, 0x23},
	{0x3621, 0xe0},
	{0x3704, 0xa0},
	{0x3703, 0x5a},
	{0x3715, 0x78},
	{0x3717, 0x01},
	{0x370b, 0x60},
	{0x3705, 0x1a},
	{0x3905, 0x02},
	{0x3906, 0x10},
	{0x3901, 0x0a},
	{0x3731, 0x02},
	//VCM debug mode
	{0x3600, 0x37},
	{0x3601, 0x33},
	//System control register changing not recommended
	{0x302d, 0x60},
	//??
	{0x3620, 0x52},
	{0x371b, 0x20},
	//?? DVP
	{0x471c, 0x50},

	{0x3a13, 0x43},
	{0x3a18, 0x00},
	{0x3a19, 0xf8},
	{0x3635, 0x13},
	{0x3636, 0x06},
	{0x3634, 0x44},
	{0x3622, 0x01},
	{0x3c01, 0x34},
	{0x3c04, 0x28},
	{0x3c05, 0x98},
	{0x3c06, 0x00},
	{0x3c07, 0x08},
	{0x3c08, 0x00},
	{0x3c09, 0x1c},
	{0x3c0a, 0x9c},
	{0x3c0b, 0x40},

	//[7]=1 color bar enable, [3:2]=00 eight color bar
	{0x503d, 0x00},
	//[2]=1 ISP vflip, [1]=1 sensor vflip
	{0x3820, 0x46},

	//[7:5]=010 Two lane mode, [4]=0 MIPI HS TX no power down, [3]=0 MIPI LP RX no power down, [2]=1 MIPI enable, [1:0]=10 Debug mode; Default=0x58
	{0x300e, 0x45},
	//[5]=0 Clock free running, [4]=1 Send line short packet, [3]=0 Use lane1 as default, [2]=1 MIPI bus LP11 when no packet; Default=0x04
	{0x4800, 0x14},
	{0x302e, 0x08},
	//[7:4]=0x3 YUV422, [3:0]=0x0 YUYV
	//{0x4300, 0x30},
	//[7:4]=0x6 RGB565, [3:0]=0x0 {b[4:0],g[5:3],g[2:0],r[4:0]}
	{0x4300, 0x6f},
	{0x501f, 0x01},

	{0x4713, 0x03},
	{0x4407, 0x04},
	{0x440e, 0x00},
	{0x460b, 0x35},
	//[1]=0 DVP PCLK divider manual control by 0x3824[4:0]
	{0x460c, 0x20},
	//[4:0]=1 SCALE_DIV=INT(3824[4:0]/2)
	{0x3824, 0x01},

	//MIPI timing
	//		{0x4805, 0x10}, //LPX global timing select=auto
	//		{0x4818, 0x00}, //hs_prepare + hs_zero_min ns
	//		{0x4819, 0x96},
	//		{0x482A, 0x00}, //hs_prepare + hs_zero_min UI
	//
	//		{0x4824, 0x00}, //lpx_p_min ns
	//		{0x4825, 0x32},
	//		{0x4830, 0x00}, //lpx_p_min UI
	//
	//		{0x4826, 0x00}, //hs_prepare_min ns
	//		{0x4827, 0x32},
	//		{0x4831, 0x00}, //hs_prepare_min UI

	//[7]=1 LENC correction enabled, [5]=1 RAW gamma enabled, [2]=1 Black pixel cancellation enabled, [1]=1 White pixel cancellation enabled, [0]=1 Color interpolation enabled
	{0x5000, 0x07},
	//[7]=0 Special digital effects, [5]=0 scaling, [2]=0 UV average disabled, [1]=1 Color matrix enabled, [0]=1 Auto white balance enabled
	{0x5001, 0x03}
};

static const camera_config_t g_halfx_1080p_30fps_336M_mipi_[] = {
	//1920 x 1080 @ 30fps, RAW10, MIPISCLK=672, SCLK=67.2MHz, PCLK=134.4M
	//PLL1 configuration
	//[7:4]=0001 System clock divider /1, [3:0]=0001 Scale divider for MIPI /1
	{0x3035, 0x11}, // 30fps setting
	//[7:0]=84 PLL multiplier
	{0x3036, 0x54},
	//[4]=1 PLL root divider /2, [3:0]=5 PLL pre-divider /1.5
	{0x3037, 0x15},
	//[5:4]=00 PCLK root divider /1, [3:2]=00 SCLK2x root divider /1, [1:0]=01 SCLK root divider /2
	{0x3108, 0x01},

	//[6:4]=001 PLL charge pump, [3:0]=1010 MIPI 10-bit mode
	{0x3034, 0x1A},

	//[3:0]=0 X address start high byte
	{0x3800, (336 >> 8) & 0x0F},
	//[7:0]=0 X address start low byte
	{0x3801, 336 & 0xFF},
	//[2:0]=0 Y address start high byte
	{0x3802, (426 >> 8) & 0x07},
	//[7:0]=0 Y address start low byte
	{0x3803, 426 & 0xFF},

	//[3:0] X address end high byte
	{0x3804, (2287 >> 8) & 0x0F},
	//[7:0] X address end low byte
	{0x3805, 2287 & 0xFF},
	//[2:0] Y address end high byte
	{0x3806, (1529 >> 8) & 0x07},
	//[7:0] Y address end low byte
	{0x3807, 1529 & 0xFF},

	//[3:0]=0 timing hoffset high byte
	{0x3810, (496 >> 8) & 0x0F},
	//[7:0]=0 timing hoffset low byte
	{0x3811, 496 & 0xFF},
	//[2:0]=0 timing voffset high byte
	{0x3812, (12 >> 8) & 0x07},
	//[7:0]=0 timing voffset low byte
	{0x3813, 12 & 0xFF},

	//[3:0] Output horizontal width high byte
	{0x3808, (960 >> 8) & 0x0F},
	//[7:0] Output horizontal width low byte
	{0x3809, 960 & 0xFF},
	//[2:0] Output vertical height high byte
	{0x380a, (1080 >> 8) & 0x7F},
	//[7:0] Output vertical height low byte
	{0x380b, 1080 & 0xFF},

	//HTS line exposure time in # of pixels Tline=HTS/sclk
	{0x380c, (2500 >> 8) & 0x1F},
	{0x380d, 2500 & 0xFF},
	//VTS frame exposure time in # lines
	{0x380e, (1120 >> 8) & 0xFF},
	{0x380f, 1120 & 0xFF},

	//[7:4]=0x1 horizontal odd subsample increment, [3:0]=0x1 horizontal even subsample increment
	{0x3814, 0x11},
	//[7:4]=0x1 vertical odd subsample increment, [3:0]=0x1 vertical even subsample increment
	{0x3815, 0x11},

	//[2]=0 ISP mirror, [1]=0 sensor mirror, [0]=0 no horizontal binning
	{0x3821, 0x00},

	//little MIPI shit: global timing unit, period of PCLK in ns * 2(depends on # of lanes)
	{0x4837, 14}, // 1/84M*2

	//Undocumented anti-green settings
	{0x3618, 0x00}, // Removes vertical lines appearing under bright light
	{0x3612, 0x59},
	{0x3708, 0x64},
	{0x3709, 0x52},
	{0x370c, 0x03},

	//[7:4]=0x0 Formatter RAW, [3:0]=0x0 BGBG/GRGR
	{0x4300, 0x00},
	//[2:0]=0x3 Format select ISP RAW (DPC)
	{0x501f, 0x03}
};

XStatus camera_get_id(XIic *i2c_instance, u8 camera_module, u16 *camera_id) {
	XStatus status = XST_SUCCESS;

	/* Initialize the i2c mux taking into account which camera module
	 * do we want to address
	 */
	switch (camera_module) {
	case 0: {
		status = i2c_mux_init(i2c_instance, I2C_MUX_ADDR, CAMERA_MIPI_A);
		if (XST_SUCCESS != status) {
			return XST_FAILURE;
		}
		break;
	}
	case 1: {
		status = i2c_mux_init(i2c_instance, I2C_MUX_ADDR, CAMERA_MIPI_B);
		if (XST_SUCCESS != status) {
			return XST_FAILURE;
		}
		break;
	}
	default: {
		return XST_FAILURE;
	}
	}

	/* Read the camera id */
	u8 rx_buffer[2] = {0};
	status = i2c_read_buffer(i2c_instance, CAMERA_I2C_ADDRESS, rx_buffer, sizeof(rx_buffer), CAMERA_ID_REG_ADDR);
	if (XST_SUCCESS != status) {
		return XST_FAILURE;
	}

	*camera_id = (rx_buffer[0] << 8) | rx_buffer[1];

	return XST_SUCCESS;
}

XStatus camera_write_reg(XIic *i2c_instance, u16 reg_addr, u8 reg_data) {
	XStatus status = XST_SUCCESS;

	u8 reg_addr_l = (reg_addr & 0xFF);
	u8 reg_addr_h = ((reg_addr >> 8) & 0xFF);
	u8 tx_buffer[] = {reg_addr_h, reg_addr_l, reg_data};

	status = i2c_send_buffer(i2c_instance, CAMERA_I2C_ADDRESS, tx_buffer, sizeof(tx_buffer));

	return status;
}

XStatus camera_set_mode(XIic *i2c_instance, u8 camera_module, camera_mode_t camera_mode) {
	XStatus status = XST_SUCCESS;

	if (camera_mode >= MODE_END) {
		return XST_FAILURE;
	}

	/* Initialize the i2c mux taking into account which camera module
	 * do we want to address
	 */
	switch (camera_module) {
	case 0: {
		status = i2c_mux_init(i2c_instance, I2C_MUX_ADDR, CAMERA_MIPI_A);
		if (XST_SUCCESS != status) {
			return XST_FAILURE;
		}
		break;
	}
	case 1: {
		status = i2c_mux_init(i2c_instance, I2C_MUX_ADDR, CAMERA_MIPI_B);
		if (XST_SUCCESS != status) {
			return XST_FAILURE;
		}
		break;
	}
	default: {
		return XST_FAILURE;
	}
	}


	switch (camera_mode) {
		case MODE_HALFX_1080P_1920_1080_30fps_336M_MIPI:
		{
			u16 index = 0;
			for (index = 0; index < sizeof(g_halfx_1080p_30fps_336M_mipi_)/sizeof(g_halfx_1080p_30fps_336M_mipi_[0]); ++index)
			{
				status = camera_write_reg(i2c_instance, g_halfx_1080p_30fps_336M_mipi_[index].addr, g_halfx_1080p_30fps_336M_mipi_[index].data);
				if (XST_SUCCESS != status) {
					return XST_FAILURE;
				}
			}
			break;
		}
		default:
			return XST_FAILURE;
	}
	// We power up the camera
	camera_write_reg(i2c_instance, 0x3008, 0x02);

	return XST_SUCCESS;
}

XStatus camera_init(XIic *i2c_instance, u8 camera_module) {
	XStatus status = XST_SUCCESS;

	/* Get camera ID */
	u16 camera_id = 0x0;
	status = camera_get_id(i2c_instance, camera_module, &camera_id);
	if (XST_SUCCESS != status) {
		return XST_FAILURE;
	}

	/* Check if we have a valid camera id */
	if (CAMERA_DEFAULT_ID != camera_id) {
		printf("Got %x. Expected %x\n\r", camera_id, CAMERA_DEFAULT_ID);
		return XST_FAILURE;
	}

	status = camera_write_reg(i2c_instance, 0x3103, 0x11);
	if (XST_SUCCESS != status) {
		return XST_FAILURE;
	}

	status = camera_write_reg(i2c_instance, 0x3008, 0x82);
	if (XST_SUCCESS != status) {
		return XST_FAILURE;
	}

	usleep(1000000);

	u16 index = 0;
	for (index = 0; index < sizeof(g_camera_config)/sizeof(g_camera_config[0]); ++index) {
		status = camera_write_reg(i2c_instance, g_camera_config[index].addr, g_camera_config[index].data);
		if (XST_SUCCESS != status) {
			return XST_FAILURE;
		}
	}

	return XST_SUCCESS;
}
