#include "drv_gpio.h"
#include "hal_data.h"
#include <devices.h>
#include <errno.h>
#include <stdio.h>

static int IODrvInit(struct IODev *ptdev);
static int IODrvWrite(struct IODev *ptdev, unsigned char level);
static int IODrvRead(struct IODev *ptdev);

static struct IODev gADXLCSDev = {
    .name = "ADXL345 CS",
    .port = BSP_IO_PORT_02_PIN_05,
    .Init = IODrvInit,
    .Read = IODrvRead,
    .Write = IODrvWrite,
    .next = NULL
};
static struct IODev gADXINT1Dev = {
    .name = "ADXL345 INT1",
    .port = BSP_IO_PORT_05_PIN_03,
    .Init = IODrvInit,
    .Read = IODrvRead,
    .Write = IODrvWrite,
    .next = NULL
};

void IODevicesCreate(void)
{
    /* ADXL345 CS: software-controlled via P4.01 (NOT P2.05 which is claimed by SPI peripheral). */
    IODeviceInsert(&gADXLCSDev);
    IODeviceInsert(&gADXINT1Dev);
}

static int IODrvInit(struct IODev *ptdev)
{
    if(ptdev == NULL)       return -EINVAL;
    if(ptdev->name == NULL) return -EINVAL;
    
    fsp_err_t err = g_ioport.p_api->open(g_ioport.p_ctrl, g_ioport.p_cfg);
    assert(FSP_SUCCESS == err);

    return ESUCCESS;
}

static int IODrvWrite(struct IODev *ptdev, unsigned char level)
{
    if(ptdev == NULL)       return -EINVAL;
    if(ptdev->name == NULL) return -EINVAL;
    
    fsp_err_t err = g_ioport.p_api->pinCfg(g_ioport.p_ctrl, ptdev->port, IOPORT_CFG_PORT_DIRECTION_OUTPUT);
    assert(FSP_SUCCESS == err);
    err = g_ioport.p_api->pinWrite(g_ioport.p_ctrl, ptdev->port, (bsp_io_level_t)level);
    assert(FSP_SUCCESS == err);

    return ESUCCESS;
}

static int IODrvRead(struct IODev *ptdev)
{
    if(ptdev == NULL)       return -EINVAL;
    if(ptdev->name == NULL) return -EINVAL;

    fsp_err_t err = g_ioport.p_api->pinCfg(g_ioport.p_ctrl, ptdev->port, IOPORT_CFG_PORT_DIRECTION_INPUT);
    assert(FSP_SUCCESS == err);
    err = g_ioport.p_api->pinRead(g_ioport.p_ctrl, ptdev->port, (bsp_io_level_t*)&ptdev->value);
    assert(FSP_SUCCESS == err);

    return ESUCCESS;
}
