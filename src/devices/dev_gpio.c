#include "dev_gpio.h"
#include <drivers.h>
#include <libs.h>
<<<<<<< HEAD
#include "../src/debug_uart/debug_uart.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

static struct IODev *gHeadIODev;

void IODevicesRegister(void)
{
    IODevicesCreate();
    IODeviceList();
}

void IODeviceInsert(struct IODev *ptdev)
{
    if(NULL == gHeadIODev)
        gHeadIODev = ptdev;
    else
    {
        ptdev->next = gHeadIODev;
        gHeadIODev = ptdev;
    }
}

struct IODev *IODeviceFind(const char *name)
{
    struct IODev *ptdev = gHeadIODev;
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

void IODeviceList(void)
{
    struct IODev *ptdev = gHeadIODev;
    Serial_Printf(g_debug_uart4.p_ctrl,"\r\nIO Device List:\r\n");
    while(ptdev)
    {
        Serial_Printf(g_debug_uart4.p_ctrl,"\t%s\r\n", ptdev->name);
        ptdev = ptdev->next;
    }
    Serial_Printf(g_debug_uart4.p_ctrl,"\r\n");
=======
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

static struct IODev *gHeadIODev;

void IODevicesRegister(void)
{
    IODevicesCreate();
    IODeviceList();
}

void IODeviceInsert(struct IODev *ptdev)
{
    if(NULL == gHeadIODev)
        gHeadIODev = ptdev;
    else
    {
        ptdev->next = gHeadIODev;
        gHeadIODev = ptdev;
    }
}

struct IODev *IODeviceFind(const char *name)
{
    struct IODev *ptdev = gHeadIODev;
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

void IODeviceList(void)
{
    struct IODev *ptdev = gHeadIODev;
    xprintf("\r\nIO Device List:\r\n");
    while(ptdev)
    {
        xprintf("\t%s\r\n", ptdev->name);
        ptdev = ptdev->next;
    }
    xprintf("\r\n");
>>>>>>> branch 'main' of git@github.com:Wendellee/Embedded-Competition-Renesas-Track.git
}
