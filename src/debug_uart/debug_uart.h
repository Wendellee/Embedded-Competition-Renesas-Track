/*
 * debug_uart.h
 *
 *  Created on: 2026年1月14日
 *      Author: 李文学
 */

#ifndef DEBUG_UART_DEBUG_UART_H_
#define DEBUG_UART_DEBUG_UART_H_
/* Include ------------------------------------------- */
#include "hal_data.h"
#include "stdbool.h"
#include "string.h"
#include <stdio.h>
#include <stdarg.h>
#include "stdio.h"
/* User Define --------------------------------------- */
/* User Variable ------------------------------------- */
typedef enum{
  Hex = 1,
  Str = 2
}ReceiveType;
typedef enum{
  StateNone,
	StateHeader,
	StateHeader1,
  StateData,
	StateTailer,
	StateTailer1
}State;
typedef enum{
  ReceivePacketNone = 0x00,
	ReceivePacketHeader = 0xFF,
	ReceivePacketHeader1 = 0xAA,
	ReceivePacketTailer = 0xFE,
	ReceivePacketTailer1 = 0x55
}ReceiveState;
typedef enum //Page
{
	Page_Debug = 0x00, // 调试功能
	Page_one = 0x01,   // 功能1
	Page_two = 0x02,   // 功能2
	Page_three = 0x03, // 功能3
	Page_four = 0x04,  // 功能4
	Page_five = 0x05,  // 功能5
	Page_six = 0x06,   // 功能6
	Page_seven = 0x11, // 拓展1
	Page_eight = 0x22, // 拓展2
	Page_nine = 0x33   // 拓展3
}Page;
typedef enum //Function
{
	Function_Debug = 0x00, // 调试功能
	Function_one = 0x01,   // 功能1
	Function_two = 0x02,   // 功能2
	Function_three = 0x03, // 功能3
	Function_four = 0x04,  // 功能4
	Function_five = 0x05,  // 功能5
	Function_six = 0x06,   // 功能6
	Function_seven = 0x11, // 拓展1
	Function_eight = 0x22, // 拓展2
	Function_nine = 0x33   // 拓展3
}Function;
/* User Extern --------------------------------------- */
extern uint8_t hmi_Serial_TxPacket[6];				//定义发送数据包数组，数据包格式：FF AA [Page] [Function] [01] [02] [03] [04] FE 55
extern uint8_t hmi_Serial_RxPacket[6];				//定义接收数据包数组
extern uint8_t hmi_Serial_RxFlag;					//定义接收数据包标志位
/* User Function ------------------------------------- */
void debug_uart7_init(void);
void hmi_uart9_init(void);
void debug_uart4_init(void);
void W800_uart6_init(void);

void hmi_uart9_callback(uart_callback_args_t *p_args);
uint8_t hmi_Serial_GetRxFlag(void);
/* Basic --------------------------------------------- */
/**
  * 函    数：串口发送一个字节
  * 参    数：Byte 要发送的一个字节
  * 返 回 值：无
  */
void Serial_SendByte(uart_ctrl_t * const p_api_ctrl,uint8_t Byte);
/**
  * 函    数：串口发送一个数组
  * 参    数：Array 要发送数组的首地址
  * 参    数：Length 要发送数组的长度
  * 返 回 值：无
  */
void Serial_SendArray(uart_ctrl_t * const p_api_ctrl,uint8_t *Array, uint16_t Length);
/**
  * 函    数：串口发送一个字符串
  * 参    数：String 要发送字符串的首地址
  * 返 回 值：无
  */
void Serial_SendString(uart_ctrl_t * const p_api_ctrl,char *String);
/**
  * 函    数：次方函数（内部使用）
  * 返 回 值：返回值等于X的Y次方
  */
uint32_t Serial_Pow(uint32_t X, uint32_t Y);
/**
  * 函    数：串口发送数字
  * 参    数：Number 要发送的数字，范围：0~4294967295
  * 参    数：Length 要发送数字的长度，范围：0~10
  * 返 回 值：无
  */
void Serial_SendNumber(uart_ctrl_t * const p_api_ctrl,uint32_t Number, uint8_t Length);
/**
  * 函    数：使用printf需要重定向的底层函数
  * 参    数：保持原始格式即可，无需变动
  * 返 回 值：保持原始格式即可，无需变动
  */
int my_fputc(uart_ctrl_t * const p_api_ctrl,int ch, FILE *f);
/**
  * 函    数：自己封装的prinf函数
  * 参    数：format 格式化字符串
  * 参    数：... 可变的参数列表
  * 返 回 值：无
  */
void Serial_Printf(uart_ctrl_t * const p_api_ctrl,char *format, ...);

#endif /* DEBUG_UART_DEBUG_UART_H_ */
