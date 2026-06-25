#include "dev_timer.h"
#include <drivers.h>
#include <libs.h>
#include "../src/debug_uart/debug_uart.h"
#include "hal_data.h"
#include <stdio.h>
#include <stdlib.h>

static struct TimerDev *gHeadTimerDev;

void TimerDevicesRegister(void)
{
#if DRV_USE_SYSTICK
    SystickTimerDevicesCreate();
#endif
    
#if DRV_USE_GPT
    GPTTimerDevicesCreate();
#endif
    
    TimerDeviceList();
}

void TimerDeviceInsert(struct TimerDev *ptdev)
{
    if(NULL == gHeadTimerDev)
        gHeadTimerDev = ptdev;
    else
    {
        ptdev->next = gHeadTimerDev;
        gHeadTimerDev = ptdev;
    }
}

struct TimerDev *TimerDeviceFind(const char *name)
{
    struct TimerDev *ptdev = gHeadTimerDev;
    while(ptdev)
    {
        if(strstr(ptdev->name, name))
        {
            return ptdev;
        }
        ptdev = ptdev->next;
    }
    return NULL;
}

void TimerDeviceList(void)
{
    struct TimerDev *ptdev = gHeadTimerDev;
    Serial_Printf(g_debug_uart4.p_ctrl,"\r\nTimer Device List:\r\n");
    while(ptdev)
    {
        Serial_Printf(g_debug_uart4.p_ctrl,"\t%s\r\n", ptdev->name);
        ptdev = ptdev->next;
    }
    Serial_Printf(g_debug_uart4.p_ctrl,"\r\n");
}

