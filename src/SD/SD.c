/*
 * SD.c
 *
 *  Created on: 2026年3月26日
 *      Author: lingk
 */
#include "SD/SD.h"
#include "../src/debug_uart/debug_uart.h"

 __IO uint32_t g_transfer_complete = 0;
 __IO uint32_t g_card_erase_complete = 0;
 __IO bool g_card_inserted = false;
 uint8_t g_dest[SDHI_MAX_BLOCK_SIZE] BSP_ALIGN_VARIABLE(4);  //4字节对齐
 uint8_t g_src[SDHI_MAX_BLOCK_SIZE]  BSP_ALIGN_VARIABLE(4);
 sdmmc_device_t my_sdmmc_device = {0};


/* SDHI SD卡初始化函数 */
void SDCard_Init(void)
 {
     fsp_err_t err;

     /* 打开SDHI */
     err = R_SDHI_Open(&g_sdmmc0_ctrl, &g_sdmmc0_cfg);
     assert(FSP_SUCCESS == err);
 }

/* 如果启用了卡检测中断，则在发生卡检测事件时调用回调。 */
void g_sdmmc0_callback(sdmmc_callback_args_t *p_args)
{
    if (SDMMC_EVENT_TRANSFER_COMPLETE == p_args->event)  //读取或写入完成
    {
        g_transfer_complete = 1;
    }
    if (SDMMC_EVENT_CARD_INSERTED == p_args->event)  //卡插入中断
    {
        g_card_inserted = true;
    }
    if (SDMMC_EVENT_CARD_REMOVED == p_args->event)   //卡拔出中断
    {
        g_card_inserted = false;
    }
    if (SDMMC_EVENT_ERASE_COMPLETE == p_args->event)  //擦除完成
    {
        g_card_erase_complete = 1;
    }
    if (SDMMC_EVENT_ERASE_BUSY == p_args->event)  //擦除超时
    {
        g_card_erase_complete = 2;
    }
    if (SDMMC_EVENT_TRANSFER_ERROR == p_args->event) // 新增：捕获传输错误
        {
            g_transfer_complete = 2; // 用 2 代表发生错误，打破 while 死循环
            // 注意：如果在中断里用 printf 会卡顿，可以用断点或者简单的标志位代替
        }
}

