/*
 * SD.h
 *
 *  Created on: 2026年3月26日
 *      Author: lingk
 */

#ifndef SD_SD_H_
#define SD_SD_H_
#include "hal_data.h"
#include <stdio.h>

extern  __IO bool g_card_inserted;
extern  sdmmc_device_t my_sdmmc_device;
extern  __IO uint32_t g_transfer_complete;
extern __IO uint32_t g_card_erase_complete;

void SDCard_Init(void);
void SDCard_Operation(void);

#endif /* SD_SD_H_ */
