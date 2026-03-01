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
/* User Extern --------------------------------------- */
/* User Function ------------------------------------- */
void debug_uart7_init(void);
void hmi_uart9_init(void);
void hmi_uart9_callback(uart_callback_args_t *p_args);
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
