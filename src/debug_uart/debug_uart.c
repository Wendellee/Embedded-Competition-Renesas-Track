/*
 * debug_uart.c
 *
 *  Created on: 2026年1月14日
 *      Author: 李文学
 */
#include "debug_uart.h"

uint8_t Serial_TxPacket1[6];                //定义发送数据包数组，数据包格式：FF AA [Page] [Function] [01] [02] [03] [04] FE 55
uint8_t Serial_RxPacket1[6];                //定义接收数据包数组
uint8_t Serial_RxFlag1 = 0;                 //定义接收数据包标志位
uint8_t USART_RX_BUF[USART_REC_LEN]; //接收缓冲,最大USART_REC_LEN个字节.末字节为换行符
uint16_t USART_RX_STA = 0;          //接收状态标记

volatile bool uart_send_complete_flag = false;
void debug_uart7_init(void)
{
    g_uart7.p_api->open(g_uart7.p_ctrl, g_uart7.p_cfg);
}
void hmi_uart9_init(void)
{
    g_uart9.p_api->open(g_uart9.p_ctrl, g_uart9.p_cfg);
}
void hmi_uart9_callback(uart_callback_args_t *p_args)
{
    switch(p_args->event)
    {
        case UART_EVENT_RX_CHAR:
            if (USART_RX_STA < USART_REC_LEN)
            {
                USART_RX_BUF[USART_RX_STA++] = (uint8_t) p_args->data;
            }
            Serial_RxFlag1 = 1;
            break;
        case UART_EVENT_TX_COMPLETE:
            uart_send_complete_flag = true;
            break;
        default:
            break;
    }
}



#pragma import(__use_no_semihosting)
//标准库需要的支持函数
struct __FILE
{
    int handle;
};

FILE __stdout;

/**
  * 函    数：串口发送一个字节
  * 参    数：Byte 要发送的一个字节
  * 返 回 值：无
  */
void Serial_SendByte(uart_ctrl_t * const p_api_ctrl,uint8_t Byte)
{
    g_uart9.p_api->write(p_api_ctrl,(uint8_t *)&Byte,1);        //将字节数据写入数据寄存器，写入后USART自动生成时序波形
    while (uart_send_complete_flag == false);    //等待发送完成
    uart_send_complete_flag = false;
    /*下次写入数据寄存器会自动清除发送完成标志位，故此循环后，无需清除标志位*/
}

/**
  * 函    数：串口发送一个数组
  * 参    数：Array 要发送数组的首地址
  * 参    数：Length 要发送数组的长度
  * 返 回 值：无
  */
void Serial_SendArray(uart_ctrl_t * const p_api_ctrl,uint8_t *Array, uint16_t Length)
{
    uint16_t i;
    for (i = 0; i < Length; i ++)       //遍历数组
    {
        Serial_SendByte(p_api_ctrl,Array[i]);        //依次调用Serial_SendByte发送每个字节数据
    }
}

/**
  * 函    数：串口发送一个字符串
  * 参    数：String 要发送字符串的首地址
  * 返 回 值：无
  */
void Serial_SendString(uart_ctrl_t * const p_api_ctrl,char *String)
{
    uint8_t i;
    for (i = 0; String[i] != '\0'; i ++)//遍历字符数组（字符串），遇到字符串结束标志位后停止
    {
        Serial_SendByte(p_api_ctrl,String[i]);       //依次调用Serial_SendByte发送每个字节数据
    }
}


/**
  * 函    数：次方函数（内部使用）
  * 返 回 值：返回值等于X的Y次方
  */
uint32_t Serial_Pow(uint32_t X, uint32_t Y)
{
    uint32_t Result = 1;    //设置结果初值为1
    while (Y --)            //执行Y次
    {
        Result *= X;        //将X累乘到结果
    }
    return Result;
}

/**
  * 函    数：串口发送数字
  * 参    数：Number 要发送的数字，范围：0~4294967295
  * 参    数：Length 要发送数字的长度，范围：0~10
  * 返 回 值：无
  */
void Serial_SendNumber(uart_ctrl_t * const p_api_ctrl,uint32_t Number, uint8_t Length)
{
    uint8_t i;
    for (i = 0; i < Length; i ++)       //根据数字长度遍历数字的每一位
    {
        Serial_SendByte(p_api_ctrl,Number / Serial_Pow(10, Length - i - 1) % 10 + '0');  //依次调用Serial_SendByte发送每位数字
    }
}

/**
  * 函    数：使用printf需要重定向的底层函数
  * 参    数：保持原始格式即可，无需变动
  * 返 回 值：保持原始格式即可，无需变动
  */
int my_fputc(uart_ctrl_t * const p_api_ctrl,int ch, FILE *f)
{
    Serial_SendByte(p_api_ctrl,ch);          //将printf的底层重定向到自己的发送字节函数
    return ch;
}

/**
  * 函    数：自己封装的prinf函数
  * 参    数：format 格式化字符串
  * 参    数：... 可变的参数列表
  * 返 回 值：无
  */
void Serial_Printf(uart_ctrl_t * const p_api_ctrl,char *format, ...)
{
    char String[100];               //定义字符数组
    va_list arg;                    //定义可变参数列表数据类型的变量arg
    va_start(arg, format);          //从format开始，接收参数列表到arg变量
    vsprintf(String, format, arg);  //使用vsprintf打印格式化字符串和参数列表到字符数组中
    va_end(arg);                    //结束变量arg
    Serial_SendString(p_api_ctrl,String);        //串口发送字符数组（字符串）
}
uint8_t Serial_GetRxFlag1(void)
{
	if (Serial_RxFlag1 == 1)			//如果标志位为1
	{
		Serial_RxFlag1 = 0;
		return 1;					//则返回1，并自动清零标志位
	}
	return 0;						//如果标志位为0，则返回0
}
