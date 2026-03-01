#include "app.h"
#include <devices.h>
#include <libs.h>
#include "errno.h"
#include "hal_data.h"
#include <stdio.h>

void DeviceTest(void)
{
//    UartDevicesRegister();
    TimerDevicesRegister();
    SPIDevicesRegister();
    IODevicesRegister();

    ADXL345Device *pADXL345 = ADXL345GetDevice();
    if(NULL == pADXL345)
    {
        xprintf("Failed to Find ADXL345 Device!\r\n");
        return;
    }
    pADXL345->Init(pADXL345);
    while(1)
    {
        if(pADXL345->Read(pADXL345) == ESUCCESS)
        {
//            xprintf("x:%.4fg\ty:%.4fg\tz:%.4fg\r\n", pADXL345->value.x, pADXL345->value.y, pADXL345->value.z);
            xprintf("add s0.id,0,%d\xff\xff\xff",(uint16_t)((pADXL345->value.x *50 / 4)+ 50));
            xprintf("add s2.id,0,%d\xff\xff\xff",(uint16_t)((pADXL345->value.y *50 / 4)+ 50));
            xprintf("add s1.id,0,%d\xff\xff\xff",(uint16_t)((pADXL345->value.z *50 / 4)+ 50));
        }
        mdelay(500);
    }
}
