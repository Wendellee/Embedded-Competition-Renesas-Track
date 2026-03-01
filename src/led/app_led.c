/*
 * app_led.c
 *
 *  Created on: 2026年1月13日
 *      Author: 李文学
 */
#include "app_led.h"



void ds_led_on(void)
{
    g_ioport.p_api->pinWrite(g_ioport.p_ctrl,BSP_IO_PORT_04_PIN_00,BSP_IO_LEVEL_LOW);
//    R_IOPORT_PinWrite(&g_ioport_ctrl, BSP_IO_PORT_04_PIN_00, BSP_IO_LEVEL_LOW);
}

void ds_led_off(void)
{
    g_ioport.p_api->pinWrite(g_ioport.p_ctrl,BSP_IO_PORT_04_PIN_00,BSP_IO_LEVEL_HIGH);
//    R_IOPORT_PinWrite(&g_ioport_ctrl, BSP_IO_PORT_04_PIN_00, BSP_IO_LEVEL_HIGH);
}

void ds_led_blink(uint16_t time)
{
//    R_IOPORT_PinWrite(&g_ioport_ctrl, BSP_IO_PORT_04_PIN_00, BSP_IO_LEVEL_HIGH);
//    R_BSP_SoftwareDelay(time,BSP_DELAY_UNITS_MILLISECONDS);
//    R_IOPORT_PinWrite(&g_ioport_ctrl, BSP_IO_PORT_04_PIN_00, BSP_IO_LEVEL_LOW);
//    R_BSP_SoftwareDelay(time,BSP_DELAY_UNITS_MILLISECONDS);
    g_ioport.p_api->pinWrite(g_ioport.p_ctrl,BSP_IO_PORT_04_PIN_00, BSP_IO_LEVEL_HIGH);
    R_BSP_SoftwareDelay(time,BSP_DELAY_UNITS_MILLISECONDS);
    g_ioport.p_api->pinWrite(g_ioport.p_ctrl,BSP_IO_PORT_04_PIN_00, BSP_IO_LEVEL_LOW);
    R_BSP_SoftwareDelay(time,BSP_DELAY_UNITS_MILLISECONDS);
}
