/*
 * libs.h
 *
 *  Created on: 2026年6月20日
 *      Author: Little Prince
 */

#ifndef APPLICATIONS_LIBS_H_
#define APPLICATIONS_LIBS_H_

#include <config.h>
#include "debug_uart.h"
#if LIBS_USE_DELAY
#include "delay.h"
#endif /* LIBS_USE_DELAY */

#if LIBS_USE_PRINTF
    #define xprintf(...)    Serial_Printf(g_uart9.p_ctrl,__VA_ARGS__)
#else
    #define xprintf(...)
#endif /* DEV_USE_UART */

#if LIBS_USE_BUFFER
#include "./ring_buffer/ring_buffer.h"
#endif /* LIBS_USE_BUFFER */


#endif /* APPLICATIONS_LIBS_H_ */
