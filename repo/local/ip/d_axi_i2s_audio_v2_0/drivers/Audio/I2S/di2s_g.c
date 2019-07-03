/*
 * di2s_g.c
 *
 *  Created on: Aug 28, 2015
 *      Author: rohegbec
 */

#include "di2s_l.h"
#include "di2s.h"

/*
* The configuration table for devices
*/

DI2s_Config DI2s_ConfigTable[] =
{
		{
				DI2S_BASSEADDR,
				DI2S_STREAM_ENABLE,
				DI2S_MASTER
		}
};
