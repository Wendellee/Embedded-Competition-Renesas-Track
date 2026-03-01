#ifndef __LIBS_H
#define __LIBS_H

#include <config.h>
#include "../src/debug_uart/debug_uart.h"
#if LIBS_USE_DELAY
#include "./delay/delay.h"
#endif /* LIBS_USE_DELAY */

#if LIBS_USE_PRINTF
    #define xprintf(...)    Serial_Printf(g_uart9.p_ctrl,__VA_ARGS__)
#else
    #define xprintf(...)
#endif /* DEV_USE_UART */

#if LIBS_USE_BUFFER
#include "./ring_buffer/ring_buffer.h"
#endif /* LIBS_USE_BUFFER */

#endif /* __LIBS_H */
