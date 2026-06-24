/*
 * W800.c
 *
 * Created on: 2026年3月11日
 * Author: lingk
 */
/**
    ************************************************************
    * 文件名：    W800.c
    * 说明：         W800的简单驱动 (瑞萨 RA6M5 FSP 版)
    ************************************************************
**/

// 瑞萨 FSP 底层头文件
#include <Net/device/ESP8266.h>
#include "hal_data.h"
#include "../src/debug_uart/debug_uart.h"

// 网络设备驱动
#include <string.h>
#include <stdio.h>

// W800 连 WiFi AT 指令格式
#define W800_WIFI_INFO       "AT+WJAP=\"123\",\"20060117\"\r\n"

uint8_t rev_ok = 0;
unsigned char w800_buf[512];
unsigned short w800_cnt = 0;

// FSP 发送完成标志位
volatile bool g_w800_tx_done = false;
//==========================================================
//  函数名称：   W800_UART6_CallBack (修改自 esp8266_CallBack)
//  函数功能：   W800 UART6 回调函数
//==========================================================
void W800_UART6_CallBack(uart_callback_args_t * p_args)
{
    switch (p_args->event)
    {
        case UART_EVENT_RX_CHAR: // 接收到单字节数据
            if (w800_cnt < sizeof(w800_buf))
            {
                w800_buf[w800_cnt++] = (unsigned char)p_args->data;
            }
            else
            {
                w800_cnt = 0; // 防止溢出
            }
            rev_ok = 1; // 标记有数据接收，触发后续的 WaitReceive 判断
            break;

        case UART_EVENT_TX_COMPLETE: // 发送完成
            g_w800_tx_done = true;
            break;

        default:
            break;
    }
}

//==========================================================
//  函数名称：   W800_Clear
//  函数功能：   清空缓存
//==========================================================
void W800_Clear(void)
{
    memset(w800_buf, 0, sizeof(w800_buf));
    w800_cnt = 0;
}

//==========================================================
//  函数名称：   W800_WaitRecive
//  函数功能：   等待接收完成
//==========================================================
_Bool W800_WaitRecive(void)
{
    if(rev_ok == 1)             // 如果有数据更新
    {
        rev_ok = 0;
        return REV_OK;          // 返回接收完成标志
    }
    return REV_WAIT;            // 返回接收未完成标志
}

//==========================================================
//  函数名称：   W800_SendCmd
//  函数功能：   发送命令
//==========================================================

_Bool W800_SendCmd(char *cmd, char *res)
{
    unsigned char timeOut = 200;

    W800_Clear(); //
    rev_ok = 0;//

    // FSP 串口发送逻辑 (修改为 g_W800_uart6_ctrl)
    g_w800_tx_done = false;
    R_SCI_UART_Write(&g_W800_uart6_ctrl, (uint8_t *)cmd, strlen((const char *)cmd));
    while (!g_w800_tx_done); // 等待发送完成 (模拟原 HAL 阻塞)

    while(timeOut--)
    {
        if(W800_WaitRecive() == REV_OK)                          // 如果收到数据
        {
            if(strstr((const char *)w800_buf, res) != NULL)      // 如果检索到关键词
            {
                W800_Clear();                                    // 清空缓存
                return 0;
            }
        }
        R_BSP_SoftwareDelay(10,BSP_DELAY_UNITS_MILLISECONDS);
    }

    return 1;
}//26/3/12第一次修改

_Bool W800_SendCmd_WIFI(char *cmd, char *res)
{
    unsigned char timeOut = 1000;

    W800_Clear(); //
    rev_ok = 0;//

    // FSP 串口发送逻辑 (修改为 g_W800_uart6_ctrl)
    g_w800_tx_done = false;
    R_SCI_UART_Write(&g_W800_uart6_ctrl, (uint8_t *)cmd, strlen((const char *)cmd));
    while (!g_w800_tx_done); // 等待发送完成 (模拟原 HAL 阻塞)

    while(timeOut--)
    {
        if(W800_WaitRecive() == REV_OK)                          // 如果收到数据
        {
            if(strstr((const char *)w800_buf, res) != NULL)      // 如果检索到关键词
            {
                W800_Clear();                                    // 清空缓存
                return 0;
            }
        }
        R_BSP_SoftwareDelay(20,BSP_DELAY_UNITS_MILLISECONDS);
    }

    return 1;
}


//==========================================================
//  函数名称：   W800_SendData (修改了 AT 指令)
//  函数功能：   发送数据
//==========================================================

void W800_SendData(unsigned char *data, unsigned short len)
{
    char cmdBuf[32];

    W800_Clear();                                // 清空接收缓存
    // W800 发送数据指令为 AT+SKSND=<socket>,<size> (假设创建的 Socket 号为 1)
    sprintf(cmdBuf, "AT+SKSND=1,%d\r\n", len);

    // 手册规定：接收到模块的响应消息(+OK)之后再开始发送原始数据
    if(!W800_SendCmd(cmdBuf, "OK"))
    {
        // --- 核心修复 1：加个微小延时，防止串口数据追尾 ---
        R_BSP_SoftwareDelay(20, BSP_DELAY_UNITS_MILLISECONDS);
        // FSP 串口发送数据
        g_w800_tx_done = false;
        R_SCI_UART_Write(&g_W800_uart6_ctrl, (uint8_t *)data, len);
        while (!g_w800_tx_done); // 等待发送完成
    }
}

//==========================================================
//  函数名称：   W800_GetIPD (修改了特征码匹配)
//  函数功能：   获取平台返回的数据
//==========================================================
/*
unsigned char *W800_GetIPD(unsigned short timeOut)
{
    char *ptrIPD = NULL;

    do
    {
        if(W800_WaitRecive() == REV_OK)
        {
            // W800 开启 SKRPTM=1 后，上报的数据通常带有 "+SKRCV" 标志
            ptrIPD = strstr((char *)w800_buf, "+SKRCV");
            if(ptrIPD == NULL)
            {
                // 如果没有找到标志，但接收到了纯数据（如 MQTT 报文），可以直接把指针指向 buf
                // 因为 MQTT 报文第一字节通常是 0x20(CONNACK) 或 0x30(PUBLISH)
                if(w800_buf[0] == 0x20 || w800_buf[0] == 0x30 || w800_buf[0] == 0x90)
                {
                     return (unsigned char *)w800_buf;
                }
            }
            else
            {
                // 如果找到了 +SKRCV 标志，跳过该头部 (格式通常为 +SKRCV:1,len,data...)
                ptrIPD = strchr(ptrIPD, ','); // 找到第一个逗号 (跳过 socket 号)
                if(ptrIPD != NULL) {
                    ptrIPD = strchr(ptrIPD + 1, ','); // 找到第二个逗号 (跳过 len)
                    if(ptrIPD != NULL) {
                        ptrIPD++;
                        return (unsigned char *)(ptrIPD);
                    }
                }
            }
        }
        R_BSP_SoftwareDelay(5,BSP_DELAY_UNITS_MILLISECONDS);
    } while(timeOut--);

    return NULL;
}
*///第一次修改
unsigned char *W800_GetIPD(unsigned short timeOut)
{
    char *ptrIPD = NULL;
    do
    {
        if(W800_WaitRecive() == REV_OK)
        {
            // 1. 尝试匹配 W800 官方的报文头
            ptrIPD = strstr((char *)w800_buf, "+SKRCV");
            if(ptrIPD != NULL)
            {
                ptrIPD = strchr(ptrIPD, ','); // 跳过 Socket
                if(ptrIPD != NULL) {
                    ptrIPD = strchr(ptrIPD + 1, ','); // 跳过长度
                    if(ptrIPD != NULL) {
                        return (unsigned char *)(ptrIPD + 1);
                    }
                }
            }

            // 2. 严格特征码扫描 (已剔除 0x30 误判)
            for(int i = 0; i < w800_cnt - 1; i++)
            {
                // 0x02 是不可见的控制字符，绝不会在普通的 AT 返回文本里出现，因此非常安全
                if(w800_buf[i] == 0x20 && w800_buf[i+1] == 0x02) return &w800_buf[i];
                if(w800_buf[i] == 0x90 && w800_buf[i+1] == 0x03) return &w800_buf[i];
            }
        }
        R_BSP_SoftwareDelay(5, BSP_DELAY_UNITS_MILLISECONDS);
    } while(timeOut--);

    if (w800_cnt > 0) {
        Serial_Printf(g_debug_uart4.p_ctrl,"WARN: GetIPD Timeout! Raw RX: ");
        for(int i = 0; i < w800_cnt; i++) {
            Serial_Printf(g_debug_uart4.p_ctrl,"%02X ", w800_buf[i]);
        }
        Serial_Printf(g_debug_uart4.p_ctrl,"\r\n");
    }

    return NULL;
}
//==========================================================
//  函数名称：   W800_Init (修改了初始化 AT 序列)
//  函数功能：   初始化W800
//==========================================================
void W800_Init(void)
{
    W800_Clear();

    Serial_Printf(g_debug_uart4.p_ctrl,"1. AT+\r\n");
    // 注意：W800 的测试指令是 AT+，且回复 +OK
    while(W800_SendCmd("AT+\r\n", "OK"))
        R_BSP_SoftwareDelay(500,BSP_DELAY_UNITS_MILLISECONDS);

    Serial_Printf(g_debug_uart4.p_ctrl,"2. WPRT=0 (STA Mode)\r\n");
    while(W800_SendCmd("AT+WPRT=0\r\n", "OK"))
        R_BSP_SoftwareDelay(500,BSP_DELAY_UNITS_MILLISECONDS);

    Serial_Printf(g_debug_uart4.p_ctrl,"3. SSID\r\n");
    // 替换为你的 WiFi 名称
    while(W800_SendCmd("AT+SSID=\"123\"\r\n", "OK"))
        R_BSP_SoftwareDelay(500,BSP_DELAY_UNITS_MILLISECONDS);

    Serial_Printf(g_debug_uart4.p_ctrl,"4. KEY\r\n");
    // 1表示ASCII格式，0表示索引，后面是你的 WiFi 密码
    while(W800_SendCmd("AT+KEY=1,0,\"20060117\"\r\n", "OK"))
        R_BSP_SoftwareDelay(500,BSP_DELAY_UNITS_MILLISECONDS);

    Serial_Printf(g_debug_uart4.p_ctrl,"5. NIP=0 (Enable DHCP)\r\n");
    while(W800_SendCmd("AT+NIP=0\r\n", "OK"))
        R_BSP_SoftwareDelay(500,BSP_DELAY_UNITS_MILLISECONDS);

    Serial_Printf(g_debug_uart4.p_ctrl,"6. WJOIN (Join Network)\r\n");
    while(W800_SendCmd_WIFI("AT+WJOIN\r\n", "OK"))
        R_BSP_SoftwareDelay(1000,BSP_DELAY_UNITS_MILLISECONDS);

    Serial_Printf(g_debug_uart4.p_ctrl,"7. SKRPTM=1 (Enable Auto Report)\r\n");
    // 开启 Socket 接收数据主动上报功能
    while(W800_SendCmd("AT+SKRPTM=1\r\n", "OK"))
        R_BSP_SoftwareDelay(500,BSP_DELAY_UNITS_MILLISECONDS);

    Serial_Printf(g_debug_uart4.p_ctrl,"8. Clean Socket 1\r\n");
        W800_SendCmd("AT+SKCLS=1\r\n", "OK");

    Serial_Printf(g_debug_uart4.p_ctrl,"9. W800 Init OK\r\n");
        R_BSP_SoftwareDelay(500,BSP_DELAY_UNITS_MILLISECONDS);
}



