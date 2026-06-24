/*
 * RTC.h
 *
 *  Created on: 2026年3月27日
 *      Author: lingk
 */

#ifndef RTC_RTC_H_
#define RTC_RTC_H_

#include "hal_data.h"

#define RTC_YEAR_SET 2026       //年
#define RTC_MON_SET 3           //月
#define RTC_MDAY_SET 27          //日
/*通过蔡勒公式计算星期，1~6代表周一到周六，0代表周日*/
#define RTC_WDAY_SET (RTC_YEAR_SET-2000 \
                  + ((RTC_YEAR_SET-2000)/4) \
                  - 35 + (26*(RTC_MON_SET+1))/10 \
                  + RTC_MDAY_SET -1 )%7

/**********时间宏定义**********/
#define RTC_HOUR_SET 15          //时
#define RTC_MIN_SET 50           //分
#define RTC_SEC_SET 0           //秒

void RTC_Init(void);

#endif /* RTC_RTC_H_ */
