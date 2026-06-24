/*
 * app_key.c
 *
 *  Created on: 2026年1月14日
 *      Author: 李文学
 */
#include "app_key.h"

void ds_Get_key(key_st_t * key_status)
{
    bsp_io_level_t level;
    g_ioport.p_api->pinRead(&g_ioport_ctrl, BSP_IO_PORT_00_PIN_00, &level);
    if(level == 0)
    {
        *key_status = down;
    }
    else
    {
        *key_status = up;
    }

}
