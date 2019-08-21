/*
 * intr.h
 *
 *  Created on: Aug 20, 2019
 *      Author: bdeac
 */

#ifndef SRC_INTR_INTR_H_
#define SRC_INTR_INTR_H_

#include "xscugic.h"

#include "xstatus.h"
#include "xiic.h"

XStatus setup_interrupt_system(XIic *i2c_instance);

#endif /* SRC_INTR_INTR_H_ */
