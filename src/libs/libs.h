#ifndef __LIBS_H
#define __LIBS_H

#include <config.h>
#include "../src/debug_uart/debug_uart.h"
#if LIBS_USE_DELAY
#include "./delay/delay.h"
#endif /* LIBS_USE_DELAY */

#if LIBS_USE_PRINTF
<<<<<<< HEAD
    #define xprintf(...)    Serial_Printf(g_debug_uart4.p_ctrl,__VA_ARGS__)
    // #define eprintf(...)    Serial_Printf(g_uart7.p_ctrl,__VA_ARGS__)
#else
    #define xprintf(...)
    #define eprintf(...)
=======
    #define xprintf(...)    Serial_Printf(g_uart9.p_ctrl,__VA_ARGS__)
#else
    #define xprintf(...)
>>>>>>> branch 'main' of git@github.com:Wendellee/Embedded-Competition-Renesas-Track.git
#endif /* DEV_USE_UART */

#if LIBS_USE_BUFFER
#include "./ring_buffer/ring_buffer.h"
#endif /* LIBS_USE_BUFFER */

#endif /* __LIBS_H */
