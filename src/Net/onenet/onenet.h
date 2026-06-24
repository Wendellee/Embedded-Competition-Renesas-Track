/*
 * onenet.h
 *
 *  Created on: 2026年3月12日
 *      Author: lingk
 */

#ifndef NET_ONENET_ONENET_H_
#define NET_ONENET_ONENET_H_

_Bool OneNET_RegisterDevice(void);

_Bool OneNet_DevLink(void);

void OneNet_SendData(void);

void OneNET_Subscribe(void);

void OneNet_RevPro(unsigned char *cmd);

unsigned char OneNet_FillBuf(char *buf);

#endif /* NET_ONENET_ONENET_H_ */
