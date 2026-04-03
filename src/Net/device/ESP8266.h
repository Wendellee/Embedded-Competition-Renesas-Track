/*
 * W800.h
 *
 * Created on: 2026年3月11日
 * Author: lingk
 */

#ifndef NET_DEVICE_ESP8266_H_
#define NET_DEVICE_ESP8266_H_
#include "hal_data.h" // 引入瑞萨 FSP 库头文件
#include "../src/libs/libs.h"
#define REV_OK      0   //接收完成标志
#define REV_WAIT    1   //接收未完成标志

void W800_Init(void);
void W800_Clear(void);
_Bool W800_SendCmd(char *cmd, char *res);
_Bool W800_SendCmd_WIFI(char *cmd, char *res);
void W800_SendData(unsigned char *data, unsigned short len);
unsigned char *W800_GetIPD(unsigned short timeOut);
_Bool W800_WaitRecive(void);


int _write(int fd, char *pBuffer, int size);
int _close(int fd);
int _lseek(int fd, int ptr, int dir);
int _read(int fd, char *ptr, int len);
int _fstat(int fd, void *st);
int _isatty(int fd);


#endif /* NET_DEVICE_ESP8266_H_ */
