#include "dev_spi.h"
#include <drivers.h>
#include "../src/debug_uart/debug_uart.h"
#include <libs.h>
#include <string.h>
#include <stdio.h>

static struct SPIDev *gHeadSPIDev;

void SPIDevicesRegister(void)
{
#if DRV_USE_SCI_SPI
    SCISPIDevicesCreate();
#endif

#if DRV_USE_SPI
    SPIDevicesCreate();
#endif
    SPIDeviceList();
}

void SPIDeviceInsert(struct SPIDev *ptdev)
{
    if(NULL == gHeadSPIDev)
        gHeadSPIDev = ptdev;
    else
    {
        ptdev->next = gHeadSPIDev;
        gHeadSPIDev = ptdev;
    }
}

struct SPIDev *SPIDeviceFind(const char *name)
{
    struct SPIDev *ptdev = gHeadSPIDev;
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

void SPIDeviceList(void)
{
    struct SPIDev *ptdev = gHeadSPIDev;
    Serial_Printf(g_debug_uart4.p_ctrl,"\r\nSPI Device List:\r\n");
    while(ptdev)
    {
        Serial_Printf(g_debug_uart4.p_ctrl,"\t%s\r\n", ptdev->name);
        ptdev = ptdev->next;
    }
    Serial_Printf(g_debug_uart4.p_ctrl,"\r\n");
}