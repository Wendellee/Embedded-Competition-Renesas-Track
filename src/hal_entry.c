#include "hal_data.h"
#include "../src/led/app_led.h"
#include "../src/key/app_key.h"
#include "../src/debug_uart/debug_uart.h"
#include "../src/Net/onenet/onenet.h"
#include "../src/Net/device/ESP8266.h"
#include "../src/FatFs/ff15/ff.h"
#include "../src/SD/SD.h"
#include "../src/RTC/RTC.h"
#include <string.h>
#include <stdint.h>
#include <stdio.h>
#include <stdarg.h>
#include "r_sdmmc_api.h"
#include "stdio.h"
#include "../src/applications/app.h"
#include "../src/common_utils.h"
#include "../src/rai/Motor_Monitor_NN_model.h"
#include <arm_math.h>

#include "../src/devices/devices.h"
#include "../src/libs/libs.h"
#include "../src/include/errno.h"

// 在头文件包含部分添加：
#include "rm_rai_data_collector.h"  // RAI数据采集器头文件
#include "rm_rai_data_shipper.h"    // RAI数据发送器头文件

FSP_CPP_HEADER
void R_BSP_WarmStart(bsp_warm_start_event_t event);
FSP_CPP_FOOTER

#define DATA_COLLECTION_EN       (0)         //收集数据时打开

#define MAIN_EN                (1)         //主程序
#define UART_TEST_EN           (0)         //UART测试程序
#define W800_EN                (1)         //W800测试程序
#define FFT_EN                 (0)         //FFT测试程序
#define SD_CARD_EN             (1)         //SD卡测试程序

#define ACCELERATION_EN 1                    //是否开启加速度传感器数据读取

#define  NUM_BUFFS 2
#define WINDOW_SIZE (512)
#define BUFF_LEN (WINDOW_SIZE)

#define SMOOTH_LEN 5

static int count = 0;
float data0_x[BUFF_LEN];
float data0_y[BUFF_LEN];
float data0_z[BUFF_LEN];
float data1_x[BUFF_LEN];
float data1_y[BUFF_LEN];
float data1_z[BUFF_LEN];

// RTC相关 ---------------------------------------------------------

extern rtc_time_t get_time;
// SD卡相关 ---------------------------------------------------------
MKFS_PARM f_opt = {
    .fmt = FM_FAT32,//格式选项
    .n_fat = 0,     //FATs大小
    .align = 0,     //数据区域对齐（扇区）
    .n_root = 0,    //根目录条目数
    .au_size = 0,   //群集大小（字节）
};
FATFS fs;                         /* FatFs文件系统对象 */
FIL fnew;                         /* 文件对象 */
UINT fnum;                        /* 文件成功读写数量 */
FRESULT res_sd;                   /* 文件操作结果 */
// BYTE ReadBuffer[1024];        /* 读缓冲区 */
// BYTE WriteBuffer[1024];
BYTE work[FF_MAX_SS]; /* Work area (larger is better for processing time) */
char file_name[100];
//FFT --------------------------------------------------------------
uint16_t fft_input_buf[512] = {0};
float32_t fft_output_buf[512] = {0};
float32_t fft_output_pow;
// 故障信息----------------------------------------------------------
uint8_t Progress_precent = 0;
uint8_t rai_monitor_abnormal = 0;
char fault_type[50];
char fault_reason[50];
char fault_advice[50];
uint8_t model_select = 0; //0:风扇叶片 1:皮带轮 2:联轴器
//W800 --------------------------------------------------------------
extern unsigned char w800_buf[512];
uint8_t ERR_Len = 5;

// 1. 挖三个全局 RAM 缓存坑，分别接住传感器传来的三个动态文本
char Err_Buf[50]    = "Waiting_Err...";
char Reason_Buf[50] = "Waiting_Reason...";
char Advice_Buf[50] = "Waiting_Advice...";

// 传感器数列 (在 onenet.c 中通过 extern 引用)
uint8_t current_index = 0;//索引
const char *ERR_Strings[1] = {Err_Buf};
const char *Reason_Strings[10] = {Reason_Buf};
const char *Advice_Strings[10] = {Advice_Buf};
// RAI数据采集器相关全局变量------------------------------------------
static uint16_t g_events = 0;
static uint32_t g_err_count = 0;
static bool g_dc_callback = false;
static rai_data_collector_callback_args_t g_callback_args;
rai_data_collector_frame_buffer_t g_frame_buf[8];
#if DATA_COLLECTION_EN==0
#include "Motor_Monitor_NN_model.h"
#define APPLY_SOFTMAX  1
static float scores[Motor_Monitor_NN_NUM_CLASSES];  /* confidence scores */
static float inputData[Motor_Monitor_NN_SAMPLES_PER_CHANNEL * Motor_Monitor_NN_NUM_CHANNELS];
#endif
// 1. RAI数据采集器回调函数------------------------------------------
void rai_data_collector_error_callback(const rai_data_collector_error_callback_args_t *p_args);

void rai_data_collector0_callback(const rai_data_collector_callback_args_t *p_args)
{
    g_callback_args.frame_buf_len = p_args->frame_buf_len;
    g_callback_args.frames = p_args->frames;
    g_callback_args.instance_id = p_args->instance_id;
    for(uint8_t i = 0; i < p_args->frames; i++)
    {
        g_frame_buf[i] = p_args->p_frame_buf[i];
    }
    g_callback_args.p_frame_buf = g_frame_buf;

    g_dc_callback = true;
}

void rai_data_collector0_error_callback(const rai_data_collector_error_callback_args_t *p_args)
{
    g_events |= p_args->event;
}

void rai_data_collector_error_callback(const rai_data_collector_error_callback_args_t *p_args)
{
    g_events |= p_args->event;
}

void rai_data_shipper0_callback(rai_data_shipper_callback_args_t *p_args)
{
    if(p_args->result != (uint16_t)FSP_SUCCESS)
    {
        g_err_count++;
    }
    g_rai_data_collector0.p_api->bufferRelease(&g_rai_data_collector0_ctrl);
}

// 2. 定义一个函数，用于将故障信息发送到本地 PC
void Send_To_Local_PC(const char *err, const char *reason, const char *advice);
// 3. 定义一个函数，用于计算512点实数序列的幅频响应
void arm_rfft_q15_calculate(uint16_t fftSize,uint16_t *Input,float32_t *Output);
// 4. 定义一个函数，用于将故障信息写入SD卡
void SDCard_write(char *error,char *reason,char *advice);
/*******************************************************************************************************************//**
 * main() is generated by the RA Configuration editor and is used to generate threads if an RTOS is used.  This function
 * is called by main() when no RTOS is used.+
 **********************************************************************************************************************/
void hal_entry(void)
{
    /* TODO: add your own code here */

    debug_uart7_init();
    hmi_uart9_init();
    debug_uart4_init();
    W800_uart6_init();
    RTC_Init();
    SDCard_Init();      // SD卡初始化
    
//    Serial_Printf(g_debug_uart4.p_ctrl,"--- RA6M5 System Start ---\r\n");
//    Serial_Printf(g_uart9.p_ctrl,"This is RA6M5's here!\r\n");
    #if W800_EN
    Serial_Printf(g_debug_uart4.p_ctrl,"W800 Initializing...\r\n");
    W800_Init(); 
    Serial_Printf(g_debug_uart4.p_ctrl,"W800 Initialized.\r\n");
    // Serial_Printf(g_debug_uart4.p_ctrl,"Connecting to OneNET TCP Server...\r\n");
    // while(W800_SendCmd_WIFI("AT+SKCT=0,0,\"183.230.40.96\",1883,1001\r\n", "OK")){
    //     R_BSP_SoftwareDelay(500,BSP_DELAY_UNITS_MILLISECONDS);}
    // Serial_Printf(g_debug_uart4.p_ctrl,"TCP Connected!\r\n");
    // while(OneNet_DevLink()){
    //     Serial_Printf(g_debug_uart4.p_ctrl,"OneNet MQTT Authenticating...\r\n");
    //     R_BSP_SoftwareDelay(1000,BSP_DELAY_UNITS_MILLISECONDS);}
    // Serial_Printf(g_debug_uart4.p_ctrl,"OneNet MQTT Login Successfully!\r\n");
    #endif //end of W800_EN

    #if FFT_EN //FFT 初始化
    arm_rfft_instance_q15 S;
    arm_rfft_init_q15(&S, 512, 0, 1);
    #endif //end of FFT_EN
    
    #if SD_CARD_EN
    /*----------------------- 格式化测试 ---------------------------*/
    /* 尝试挂载外部FLASH FAT文件系统 */
    res_sd = f_mount(&fs, "1:", 1);
    /* 如果没有文件系统就格式化SD卡创建文件系统 */
    if(res_sd == FR_NO_FILESYSTEM)
    {
        // Serial_Printf(g_debug_uart4.p_ctrl,"》SD卡还没有文件系统，即将进行格式化...\r\n");
        /* 格式化 */
        res_sd = f_mkfs("1:", NULL, work, sizeof(work));
        if(res_sd == FR_OK)
        {
            // Serial_Printf(g_debug_uart4.p_ctrl,"》SD卡已成功格式化文件系统。\r\n");
            /* 格式化后，先取消挂载 */
            res_sd = f_mount(NULL, "1:", 1);
            /* 重新挂载   */
            res_sd = f_mount(&fs, "1:", 1);
        }
        else
        {
            //LED1_ON;  //红灯亮
            // Serial_Printf(g_debug_uart4.p_ctrl,"《《格式化失败。》》\r\n");
            // while(1);
        }
    }
    else if(res_sd != FR_OK)
    {
        //LED1_ON;      //红灯亮
        // Serial_Printf(g_debug_uart4.p_ctrl,"！！SD卡挂载文件系统失败。(%d)\r\n", res_sd);
        // Serial_Printf(g_debug_uart4.p_ctrl,"！！可能原因：SD卡初始化不成功。\r\n");
        // while(1);
    }
    else
    {
        Serial_Printf(g_debug_uart4.p_ctrl,">文件系统挂载成功，可以进行读写测试。\r\n");
    }
    
    SDCard_write("Motor_no_results", "Motor_no_results", "Motor_no_results");
    #endif //end of SD_CARD_EN
    
    TimerDevicesRegister();
    SPIDevicesRegister();
    IODevicesRegister();
    
    // 初始化ADXL345传感器
    ADXL345Device *pADXL345_0 = ADXL345GetDevice_0();
    ADXL345Device *pADXL345_1 = ADXL345GetDevice_1();
    SPIDevice *pADXLSPI_0 = SPIDeviceFind("ADXL345_0 SPI");
    SPIDevice *pADXLSPI_1 = SPIDeviceFind("ADXL345_1 SPI");
    
    if(NULL == pADXL345_0)
    {
        // Serial_Printf(g_debug_uart4.p_ctrl,"Failed to Find ADXL345_0 Device!\r\n");
        return;
    }
    pADXL345_0->Init(pADXL345_0);
    Serial_Printf(g_debug_uart4.p_ctrl,"ADXL345_0 Initialized.\r\n");
    if(NULL == pADXL345_1)
    {
        // Serial_Printf(g_debug_uart4.p_ctrl,"Failed to Find ADXL345_1 Device!\r\n");
        return;
    }
    pADXL345_1->Init(pADXL345_1);
    Serial_Printf(g_debug_uart4.p_ctrl,"ADXL345_1 Initialized.\r\n");
    // 初始化RAI数据采集器
    #if DATA_COLLECTION_EN
    RM_RAI_DATA_COLLECTOR_Open(&g_rai_data_collector0_ctrl, &g_rai_data_collector0_cfg);
    RM_RAI_DATA_SHIPPER_Open(&g_rai_data_shipper0_ctrl, &g_rai_data_shipper0_cfg);

    RM_RAI_DATA_COLLECTOR_BufferReset(&g_rai_data_collector0_ctrl);

    RM_RAI_DATA_COLLECTOR_ChannelBufferGet(&g_rai_data_collector0_ctrl, 0, (void*)data0_x);
    RM_RAI_DATA_COLLECTOR_ChannelBufferGet(&g_rai_data_collector0_ctrl, 1, (void*)data0_y);
    RM_RAI_DATA_COLLECTOR_ChannelBufferGet(&g_rai_data_collector0_ctrl, 2, (void*)data0_z);
    RM_RAI_DATA_COLLECTOR_ChannelBufferGet(&g_rai_data_collector0_ctrl, 3, (void*)data1_x);
    RM_RAI_DATA_COLLECTOR_ChannelBufferGet(&g_rai_data_collector0_ctrl, 4, (void*)data1_y);
    RM_RAI_DATA_COLLECTOR_ChannelBufferGet(&g_rai_data_collector0_ctrl, 5, (void*)data1_z);
    #endif  //end of DATA_COLLECTION_EN
    #if MAIN_EN
    Serial_Printf(g_debug_uart4.p_ctrl,"--- System Infinte Loop Start ---\r\n");
    while(1)
    {
        // Serial_Printf(g_debug_uart4.p_ctrl,"In Loop\r\n");
        //串口接收数据
        
        // sprintf(file_name,"motor_fault_info_%d-%d-%d-%d:%d:%d.txt", get_time.tm_year + 1900, get_time.tm_mon + 1, get_time.tm_mday,get_time.tm_hour, get_time.tm_min, get_time.tm_sec);
        
        if(hmi_Serial_GetRxFlag() == 1)
        {
            if(hmi_Serial_RxPacket[2] == 0x66)//打开电机
            {
                g_ioport.p_api->pinWrite(g_ioport.p_ctrl,BSP_IO_PORT_03_PIN_12,BSP_IO_LEVEL_HIGH);
            }
            else if(hmi_Serial_RxPacket[2] == 0x99)//关闭电机
            {
                g_ioport.p_api->pinWrite(g_ioport.p_ctrl,BSP_IO_PORT_03_PIN_12,BSP_IO_LEVEL_LOW);
            }
        }
        #if ACCELERATION_EN
        if(((pADXL345_0->Read(pADXLSPI_0,pADXL345_0) == ESUCCESS) && (pADXL345_1->Read(pADXLSPI_1,pADXL345_1) == ESUCCESS)))
        {
//            Serial_Printf(g_debug_uart4.p_ctrl,"Get Data\r\n");
            //eprintf("x:%.4fg\ty:%.4fg\tz:%.4fg\r\n", pADXL345_0->value.x, pADXL345_0->value.y, pADXL345_0->value.z);
            Serial_Printf(g_uart9.p_ctrl,"add x_wave.id,0,%d\xff\xff\xff",(uint16_t)((pADXL345_0->value.x *50 / 4)+ 50));
            Serial_Printf(g_uart9.p_ctrl,"add x_wave.id,1,%d\xff\xff\xff",(uint16_t)((pADXL345_1->value.x *50 / 4)+ 50));
            Serial_Printf(g_uart9.p_ctrl,"add y_wave.id,0,%d\xff\xff\xff",(uint16_t)((pADXL345_0->value.y *50 / 4)+ 50));
            Serial_Printf(g_uart9.p_ctrl,"add y_wave.id,1,%d\xff\xff\xff",(uint16_t)((pADXL345_1->value.y *50 / 4)+ 50));
            Serial_Printf(g_uart9.p_ctrl,"add z_wave.id,0,%d\xff\xff\xff",(uint16_t)((pADXL345_0->value.z *50 / 4)+ 50));
            Serial_Printf(g_uart9.p_ctrl,"add z_wave.id,1,%d\xff\xff\xff",(uint16_t)((pADXL345_1->value.z *50 / 4)+ 50));
//            Serial_Printf(g_debug_uart4.p_ctrl,"%d\r\n",(uint16_t)((pADXL345->value.x * 250)+ 1000));
            if(count < BUFF_LEN)
            {

                fft_input_buf[count] = (uint16_t)((pADXL345_1->value.z * 250)+ 1000);
                data0_x[count] = pADXL345_0->value.x;
                data0_y[count] = pADXL345_0->value.y;
                data0_z[count] = pADXL345_0->value.z;
                data1_x[count] = pADXL345_1->value.x;
                data1_y[count] = pADXL345_1->value.y;
                data1_z[count] = pADXL345_1->value.z;

                #if DATA_COLLECTION_EN==0
                inputData[count] = pADXL345_0->value.x;
                inputData[count+WINDOW_SIZE] = pADXL345_0->value.y;
                inputData[count+2*WINDOW_SIZE] = pADXL345_0->value.z;
                inputData[count+3*WINDOW_SIZE] = pADXL345_1->value.x;
                inputData[count+4*WINDOW_SIZE] = pADXL345_1->value.y;
                inputData[count+5*WINDOW_SIZE] = pADXL345_1->value.z;
                #endif      //end of ACCELERATION_EN

                count++;
            }
            else
            {
                // count = 0;
                //FFT 处理
//                 fft_output_pow = 0;
// //                arm_rfft_q15_calculate(512,fft_input_buf,fft_output_buf);
//                 for(int i = 0; i < 256; i++)
//                 {
//                     fft_output_pow += fft_output_buf[i]/1000;
//                     // Serial_Printf(g_debug_uart4.p_ctrl,"%d,%.3f,%.3f,%d\r\n",fft_input_buf[i],fft_output_buf[i],fft_output_pow,i);
                    
//                 }
                // Serial_Printf(g_debug_uart4.p_ctrl,"%.3f\r\n",fft_output_pow);
                //模型采集数据
                #if DATA_COLLECTION_EN     //Start collect data
                RM_RAI_DATA_COLLECTOR_ChannelWrite(&g_rai_data_collector0_ctrl, 0, data0_x, BUFF_LEN);
                RM_RAI_DATA_COLLECTOR_ChannelWrite(&g_rai_data_collector0_ctrl, 1, data0_y, BUFF_LEN);
                RM_RAI_DATA_COLLECTOR_ChannelWrite(&g_rai_data_collector0_ctrl, 2, data0_z, BUFF_LEN);
                RM_RAI_DATA_COLLECTOR_ChannelWrite(&g_rai_data_collector0_ctrl, 3, data1_x, BUFF_LEN);
                RM_RAI_DATA_COLLECTOR_ChannelWrite(&g_rai_data_collector0_ctrl, 4, data1_y, BUFF_LEN);
                RM_RAI_DATA_COLLECTOR_ChannelWrite(&g_rai_data_collector0_ctrl, 5, data1_z, BUFF_LEN);  

                rai_data_shipper_write_params_t arg;
                arg.diagnostic_data_len = 0;
                arg.events = g_events;
                arg.p_diagnostic_data = NULL;
                arg.p_sensor_data = &g_callback_args;
                g_dc_callback = false;

                RM_RAI_DATA_SHIPPER_Write(&g_rai_data_shipper0_ctrl, &arg);
                g_dc_callback = false;
                #else       //end of DATA_COLLECTION_EN
                // 模型预测
                /* Model prediction */
                Motor_Monitor_NN_AICLASS AIC = Motor_Monitor_NN_predict(inputData);
                
                /* Convert class score to confidence score (CS) */
                RealityAI_get_class_scores(scores, Motor_Monitor_NN_NUM_CLASSES, APPLY_SOFTMAX, get_Motor_Monitor_NN_model());
                /* Decode classification event using class names */
                switch(AIC){
                case(Motor_Monitor_NN_no_results):
                    Serial_Printf(g_debug_uart4.p_ctrl,"Predicted Class: no_results\r\n");
                    strcpy(fault_type, "Motor_no_results");
                    strcpy(fault_reason, "No results");
                    strcpy(fault_advice, "Check the Motor Start");
                    break;
                case(Motor_Monitor_NN_motor_base_fault_level_high):
                   
                        Serial_Printf(g_debug_uart4.p_ctrl,"Predicted Class: motor_base_fault_level_high, Score: [%.3f]\r\n", scores[(int)AIC-1]);
                        strcpy(fault_type, "Motor_base_fault_level_high");
                        strcpy(fault_reason, "Three screws on the motor base are loose");
                        strcpy(fault_advice, "Check the motor base screws");
                  
                    break;
                case(Motor_Monitor_NN_motor_base_fault_level_low):
                    
                        Serial_Printf(g_debug_uart4.p_ctrl,"Predicted Class: motor_base_fault_level_low, Score: [%.3f]\r\n", scores[(int)AIC-1]);
                        strcpy(fault_type, "Motor_base_fault_level_low");
                        strcpy(fault_reason, "Two screws on the motor base are loose");
                        strcpy(fault_advice, "Check the motor base screws");
                    
                    break;
                case(Motor_Monitor_NN_motor_base_fault_level_medium):
                    
                        Serial_Printf(g_debug_uart4.p_ctrl,"Predicted Class: motor_base_fault_level_medium, Score: [%.3f]\r\n", scores[(int)AIC-1]);
                        strcpy(fault_type, "Motor_base_fault_level_medium");
                        strcpy(fault_reason, "One screw on the motor base are loose");
                        strcpy(fault_advice, "Check the motor base screws");
                    
                    break;
                case(Motor_Monitor_NN_motor_runnnig_fan):
                    
                        Serial_Printf(g_debug_uart4.p_ctrl,"Predicted Class: motor_runnnig_fan, Score: [%.3f]\r\n", scores[(int)AIC-1]);
                        // strcpy(fault_type, "Motor_runnnig_fan");
                        // strcpy(fault_reason, "Motor runnnig fan");
                        // strcpy(fault_advice, "Check the motor runnnig fan");
                   
                        
                        // strcpy(fault_type, "stop");
                        // strcpy(fault_reason, "Motor runnnig fan");
                        // strcpy(fault_advice, "Check the motor runnnig fan");
                    
                    break;
                case(Motor_Monitor_NN_motor_runnnig_fan_unbalance):
                    Serial_Printf(g_debug_uart4.p_ctrl,"Predicted Class: motor_runnnig_fan_unbalance, Score: [%.3f]\r\n", scores[(int)AIC-1]);
                    strcpy(fault_type, "Motor_runnnig_fan_unbalance");
                    strcpy(fault_reason, "Fan imbalance");
                    strcpy(fault_advice, "Check the motor fan imbalance");
                    break;
                case(Motor_Monitor_NN_motor_stop):
                    Serial_Printf(g_debug_uart4.p_ctrl,"Predicted Class: motor_stop, Score: [%.3f]\r\n", scores[(int)AIC-1]);
                    // strcpy(fault_type, "Motor_stop");
                    // strcpy(fault_reason, "Motor power off");
                    // strcpy(fault_advice, "Check the Motor Start");
                    break;
                };
                //count = 0;
                #endif   //end of DATA_COLLECTION_EN
                Send_To_Local_PC(fault_type, fault_reason, fault_advice);
                // R_RTC_CalendarTimeGet (rtc.p_ctrl, &get_time);
                // SDCard_write(fault_type, fault_reason, fault_advice);
                W800_Clear();
                
                count = 0;
            };
        }
        mdelay(5);

        
        #endif //end of ACCELERATION_EN
    }   //end of while(1)      
    #endif //end of MAIN_EN



    #if UART_TEST_EN
    Serial_Printf(g_debug_uart4.p_ctrl,"--- RA6M5 System Start ---\r\n");
    Serial_Printf(g_uart9.p_ctrl,"This is RA6M5's here!\r\n");
    while(1)
    {
        if((pADXL345_0->Read(pADXLSPI_0,pADXL345_0) == ESUCCESS) && (pADXL345_1->Read(pADXLSPI_1,pADXL345_1) == ESUCCESS))
        {
            Serial_Printf(g_debug_uart4.p_ctrl,"%.4f\r,%.4f\r,%.4f\r,%.4f\r,%.4f\r,%.4f\r\n", pADXL345_0->value.x, pADXL345_0->value.y, pADXL345_0->value.z, pADXL345_1->value.x, pADXL345_1->value.y, pADXL345_1->value.z);
            
        }
        mdelay(5);
    }
    #endif //end of UART_TEST_EN

#if BSP_TZ_SECURE_BUILD
    /* Enter non-secure code */
    R_BSP_NonSecureEnter();
#endif
}   //end of main()

/*******************************************************************************************************************//**
 * This function is called at various points during the startup process.  This implementation uses the event that is
 * called right before main() to set up the pins.
 *
 * @param[in]  event    Where at in the start up process the code is currently at
 **********************************************************************************************************************/
void R_BSP_WarmStart(bsp_warm_start_event_t event)
{
    if (BSP_WARM_START_RESET == event)
    {
#if BSP_FEATURE_FLASH_LP_VERSION != 0

        /* Enable reading from data flash. */
        R_FACI_LP->DFLCTL = 1U;

        /* Would normally have to wait tDSTOP(6us) for data flash recovery. Placing the enable here, before clock and
         * C runtime initialization, should negate the need for a delay since the initialization will typically take more than 6us. */
#endif
    }

    if (BSP_WARM_START_POST_C == event)
    {
        /* C runtime environment and system clocks are setup. */

        /* Configure pins. */
        R_IOPORT_Open (&IOPORT_CFG_CTRL, &IOPORT_CFG_NAME);

#if BSP_CFG_SDRAM_ENABLED

        /* Setup SDRAM and initialize it. Must configure pins first. */
        R_BSP_SdramInit(true);
#endif
    }
}


void Send_To_Local_PC(const char *err, const char *reason, const char *advice)
{
    char send_buf[512];
    char json_buf[256];
    char cmd_buf[100];

    // ⚠️重要：请将这里替换为你电脑的实际局域网 IP 地址
    const char *pc_ip = "10.28.122.50";
    int pc_port = 5000;

    // 1. 拼接 JSON 数据
    sprintf(json_buf, "{\"advice\":\"%s\",\"err\":\"%s\",\"reason\":\"%s\"}", advice, err, reason);

    // 2. W800 创建 TCP 连接到电脑 (AT+SKCT=0,0,"IP",端口,本地端口)
    sprintf(cmd_buf, "AT+SKCT=0,0,\"%s\",%d,1000\r\n", pc_ip, pc_port);
    Serial_Printf(g_debug_uart4.p_ctrl,"Connecting to Local PC (%s:%d)...\r\n", pc_ip, pc_port);

    if(W800_SendCmd(cmd_buf, "OK"))
    {
         Serial_Printf(g_debug_uart4.p_ctrl,"TCP Connect Failed!\r\n");
         return; // 连接失败则退出
    }

    // 3. 构造标准的 HTTP POST 报文
    sprintf(send_buf, "POST /api/upload HTTP/1.1\r\n"
                      "Host: %s:%d\r\n"
                      "Content-Type: application/json\r\n"
                      "Content-Length: %d\r\n\r\n"
                      "%s", pc_ip, pc_port, strlen(json_buf), json_buf);

    // 4. 调用底层的发送函数把数据推过去
    Serial_Printf(g_debug_uart4.p_ctrl,"Sending Data to PC...\r\n");
    W800_SendData((unsigned char *)send_buf, strlen(send_buf));

    // 5. 短暂延时后关闭通道 (通道号默认为1)
    R_BSP_SoftwareDelay(100, BSP_DELAY_UNITS_MILLISECONDS);
    W800_SendCmd("AT+SKCLS=1\r\n", "OK");
    Serial_Printf(g_debug_uart4.p_ctrl,"Data Sent and Socket Closed.\r\n");
}
void arm_rfft_q15_calculate(uint16_t fftSize,uint16_t *Input,float32_t *Output)
{
    uint16_t i;
    float32_t testOutput_f32[fftSize];
    int16_t testInput[fftSize];
    int16_t testOutput_q15[fftSize];
    arm_rfft_instance_q15 S;
    arm_rfft_init_q15(&S, fftSize, 0, 1);
    for(i=0; i<fftSize; i++)
    {
        testInput[i] = Input[i];
    }
    arm_rfft_q15(&S, testInput, testOutput_q15);
    for(i = 0; i < fftSize; i++)
    {
        testOutput_f32[i] = (float32_t)testOutput_q15[i];
    }
    arm_cmplx_mag_f32(testOutput_f32, Output, fftSize);
}

void SDCard_write(char *error,char *reason,char *advice)
{
    // 生成简化的文件名
    // char filename[100];
    // sprintf(filename,"fault_%02d%02d%02d_%02d%02d%02d.txt", 
    //         get_time->tm_mon + 1, get_time->tm_mday,
    //         get_time->tm_hour, get_time->tm_min, get_time->tm_sec);
    
    BYTE WriteBuffer[512];
    sprintf((char *)WriteBuffer,"设备型号:Y2632-4\r\n 故障描述:%s\r\n 故障原因:%s\r\n 故障建议:%s\r\n",
            error, reason, advice);
    
    res_sd = f_open(&fnew, "1:FatFs_test.txt", FA_CREATE_ALWAYS | FA_WRITE);  //FatFs读写测试文件 FatFs_files
                  if ( res_sd == FR_OK )
                  {
                      printf("》打开/创建FatFs读写测试文件.txt文件成功，向文件写入数据。\r\n");
                      res_sd = f_write(&fnew, WriteBuffer, sizeof(WriteBuffer), &fnum);
                      if(res_sd == FR_OK)
                      {
                          printf("》文件写入成功，写入字节数据：%d\n", fnum);
                          printf("》向文件写入的数据为：\r\n%s\r\n", WriteBuffer);
                      }
                      else
                      {
                          //LED1_ON;  //红灯亮
                          printf("！！文件写入失败：(%d)\n", res_sd);
                      }
                      f_close(&fnew);
                  }
}
#if BSP_TZ_SECURE_BUILD

FSP_CPP_HEADER
BSP_CMSE_NONSECURE_ENTRY void template_nonsecure_callable ();

/* Trustzone Secure Projects require at least one nonsecure callable function in order to build (Remove this if it is not required to build). */
BSP_CMSE_NONSECURE_ENTRY void template_nonsecure_callable ()
{

}
FSP_CPP_FOOTER

#endif
