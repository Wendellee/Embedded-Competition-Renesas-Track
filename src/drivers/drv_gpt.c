/*
 * drv_gpt.c
 *
 *  Created on: 2023年4月23日
 *      Author: slhuan
 */

#include "drv_gpt.h"
#include <devices.h>
#include <errno.h>
#include <libs.h>
#include "hal_data.h"
#include <stdio.h>

static int GPTDrvInit(struct TimerDev *ptdev);
static int GPTDrvStart(struct TimerDev *ptdev);
static int GPTDrvStop(struct TimerDev *ptdev);
static int GPTDrvRead(struct TimerDev *ptdev, unsigned char *buf, unsigned int length);
static int GPTDrvTimeout(struct TimerDev *ptdev, unsigned int timeout);
static void GPTDrvWaitTimer0Overflow(void);

static struct TimerDev gGPTDevice = {
    .name = "Delay Timer",
    .channel = 0,
    .status = 0,
    .Init = GPTDrvInit,
    .Start = GPTDrvStart,
    .Stop = GPTDrvStop,
    .Read = GPTDrvRead,
    .Timeout = GPTDrvTimeout,
    .next = NULL
};
static volatile bool gTimer0OverFlow = false;

void GPTTimerDevicesCreate(void)
{
    TimerDeviceInsert(&gGPTDevice);
    GPTDrvInit(&gGPTDevice);
}

static int GPTDrvInit(struct TimerDev *ptdev)
{
    if(NULL==ptdev) return -EINVAL;
    switch(ptdev->channel)
    {
        case 0:
        {
            /* 打开GPT设备完成初始化 */
            fsp_err_t err = g_timer0.p_api->open(g_timer0.p_ctrl, g_timer0.p_cfg);
            assert(FSP_SUCCESS == err);
            break;
        }
        case 1:case 2:case 3:
        case 4:case 5:case 6:
        case 7:case 8:case 9:
            break;
        default:break;
    }
    
    return true;
}

static int GPTDrvStart(struct TimerDev *ptdev)
{
    if(NULL==ptdev) return -EINVAL;
    switch(ptdev->channel)
    {
        case 0:
        {
            /* 开启GPT的计数 */
            fsp_err_t err = g_timer0.p_api->start(g_timer0.p_ctrl);
            assert(FSP_SUCCESS == err);
            break;
        }
        case 1:case 2:case 3:
        case 4:case 5:case 6:
        case 7:case 8:case 9:
            break;
        default:break;
    }
    
    return ESUCCESS;
}

static int GPTDrvStop(struct TimerDev *ptdev)
{
    if(NULL==ptdev) return -EINVAL;
    switch(ptdev->channel)
    {
        case 0:
        {
            /* 停止GPT的计数 */
            fsp_err_t err = g_timer0.p_api->stop(g_timer0.p_ctrl);
            assert(FSP_SUCCESS == err);
            err = g_timer0.p_api->reset(g_timer0.p_ctrl);
            assert(FSP_SUCCESS == err);
            break;
        }
        case 1:case 2:case 3:
        case 4:case 5:case 6:
        case 7:case 8:case 9:
            break;
        default:break;
    }
    
    return ESUCCESS;
}

static int GPTDrvRead(struct TimerDev *ptdev, unsigned char *buf, unsigned int length)
{
    if(NULL == ptdev)   return -EINVAL;
    if(NULL == buf)     return -EINVAL;
    if(0 == length)     return -EINVAL;
    return (int)length;
}

static int GPTDrvTimeout(struct TimerDev *ptdev, unsigned int timeout)
{
    if(NULL == ptdev)   return -EINVAL;
    if(0 == timeout)    return -EINVAL;
    switch(ptdev->channel)
    {
        case 0:
        {
            fsp_err_t err = g_timer0.p_api->periodSet(g_timer0.p_ctrl, timeout*100);
            assert(FSP_SUCCESS == err);
            err = g_timer0.p_api->reset(g_timer0.p_ctrl);
            assert(FSP_SUCCESS == err);
            err = g_timer0.p_api->start(g_timer0.p_ctrl);
            assert(FSP_SUCCESS == err);
            GPTDrvWaitTimer0Overflow();
            break;
        }
        case 1:case 2:case 3:
        case 4:case 5:case 6:
        case 7:case 8:case 9:
            break;
        default:break;
    }
    return ESUCCESS;
}

/* Callback function */
void timer0_callback(timer_callback_args_t * p_args)
{
    switch(p_args->event)
    {
        case TIMER_EVENT_CYCLE_END:
        {
            gTimer0OverFlow = true;
            break;
        }
        case TIMER_EVENT_CAPTURE_A:
        case TIMER_EVENT_CAPTURE_B:
        {
            break;
        }
        default:break;
    }
}

static void GPTDrvWaitTimer0Overflow(void)
{
    while(!gTimer0OverFlow);
    gTimer0OverFlow = false;
}
