/*
 * mipi_csi_rx.h
 *
 *  Created on: Aug 21, 2019
 *      Author: bdeac
 */

#ifndef SRC_MIPI_CSI_RX_MIPI_CSI_RX_H_
#define SRC_MIPI_CSI_RX_MIPI_CSI_RX_H_

#include "xstatus.h"
#include "xcsiss.h"

XStatus mipi_csi_rx_init(XCsiSs* p_drv_csiss, u32 dev_id);


#endif /* SRC_MIPI_CSI_RX_MIPI_CSI_RX_H_ */
