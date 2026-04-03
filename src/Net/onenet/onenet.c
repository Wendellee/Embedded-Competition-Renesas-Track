/**
    ************************************************************
    * 文件名：    onenet.c
    * 说明：         与onenet平台的数据交互接口层 (瑞萨 FSP 适配版)
    ************************************************************
**/

// 瑞萨 FSP 底层头文件 (替换了原有的 main.h 和 usart.h)
#include <Net/device/ESP8266.h>
#include "hal_data.h"
// === 修改 1：替换头文件 ===
#include "onenet.h"
#include "NET/MQTT/mqttkit.h"
#include "../src/debug_uart/debug_uart.h"

//算法
#include "base64.h"
#include "hmac_sha1.h"

//硬件驱动 (保留你的外部设备驱动)
//#include "delay.h"
//#include "led.h"

//C库
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "NET/CJSON/cJSON.h"

// =====================================================================
#define PROID           "mKSc01Bedy"
#define ACCESS_KEY      "OFVXUlR3c3Z4RDBjTm5zTXM5ZWI1cTNQZjJJWk1CUW0="
#define DEVICE_NAME     "W800"
// =====================================================================

char devid[16];
char key[48];

// === 修改 2：替换外部接收缓存区变量名 ===
extern unsigned char w800_buf[512];

/*
************************************************************
* 函数名称：   OTA_UrlEncode
*
* 函数功能：   sign需要进行URL编码
*
* 入口参数：   sign：加密结果
*
* 返回参数：   0-成功    其他-失败
*
* 说明：     +           %2B
* 空格      %20
* /           %2F
* ?           %3F
* %           %25
* #           %23
* &           %26
* =           %3D
************************************************************
*/
static unsigned char OTA_UrlEncode(char *sign)
{

    char sign_t[40];
    unsigned char i = 0, j = 0;
    unsigned char sign_len = strlen(sign);

    if(sign == (void *)0 || sign_len < 28)
        return 1;

    for(; i < sign_len; i++)
    {
        sign_t[i] = sign[i];
        sign[i] = 0;
    }
    sign_t[i] = 0;

    for(i = 0, j = 0; i < sign_len; i++)
    {
        switch(sign_t[i])
        {
            case '+':
                strcat(sign + j, "%2B");j += 3;
            break;

            case ' ':
                strcat(sign + j, "%20");j += 3;
            break;

            case '/':
                strcat(sign + j, "%2F");j += 3;
            break;

            case '?':
                strcat(sign + j, "%3F");j += 3;
            break;

            case '%':
                strcat(sign + j, "%25");j += 3;
            break;

            case '#':
                strcat(sign + j, "%23");j += 3;
            break;

            case '&':
                strcat(sign + j, "%26");j += 3;
            break;

            case '=':
                strcat(sign + j, "%3D");j += 3;
            break;

            default:
                sign[j] = sign_t[i];j++;
            break;
        }
    }

    sign[j] = 0;

    return 0;

}

/*
************************************************************
* 函数名称：   OTA_Authorization
*
* 函数功能：   计算Authorization
*
* 入口参数：   ver：参数组版本号，日期格式，目前仅支持格式"2018-10-31"
* res：产品id
* et：过期时间，UTC秒值
* access_key：访问密钥
* dev_name：设备名
* authorization_buf：缓存token的指针
* authorization_buf_len：缓存区长度(字节)
*
* 返回参数：   0-成功    其他-失败
*
* 说明：     当前仅支持sha1
************************************************************
*/
#define METHOD      "sha1"
static unsigned char OneNET_Authorization(char *ver, char *res, unsigned int et, char *access_key, char *dev_name,
                                            char *authorization_buf, unsigned short authorization_buf_len, _Bool flag)
{

    size_t olen = 0;

    char sign_buf[64];                              //保存签名的Base64编码结果 和 URL编码结果
    char hmac_sha1_buf[64];                         //保存签名
    char access_key_base64[64];                     //保存access_key的Base64编码结合
    char string_for_signature[72];                  //保存string_for_signature，这个是加密的key

//----------------------------------------------------参数合法性--------------------------------------------------------------------
    if(ver == (void *)0 || res == (void *)0 || et < 1564562581 || access_key == (void *)0
        || authorization_buf == (void *)0 || authorization_buf_len < 120)
        return 1;

//----------------------------------------------------将access_key进行Base64解码----------------------------------------------------
    memset(access_key_base64, 0, sizeof(access_key_base64));
    BASE64_Decode((unsigned char *)access_key_base64, sizeof(access_key_base64), &olen, (unsigned char *)access_key, strlen(access_key));
//  printf("access_key_base64: %s\r\n", access_key_base64);

//----------------------------------------------------计算string_for_signature-----------------------------------------------------
    memset(string_for_signature, 0, sizeof(string_for_signature));
    if(flag)
        snprintf(string_for_signature, sizeof(string_for_signature), "%d\n%s\nproducts/%s\n%s", et, METHOD, res, ver);
    else
        snprintf(string_for_signature, sizeof(string_for_signature), "%d\n%s\nproducts/%s/devices/%s\n%s", et, METHOD, res, dev_name, ver);
//  printf("string_for_signature: %s\r\n", string_for_signature);

//----------------------------------------------------加密-------------------------------------------------------------------------
    memset(hmac_sha1_buf, 0, sizeof(hmac_sha1_buf));

    hmac_sha1((unsigned char *)access_key_base64, strlen(access_key_base64),
                (unsigned char *)string_for_signature, strlen(string_for_signature),
                (unsigned char *)hmac_sha1_buf);

//  printf("hmac_sha1_buf: %s\r\n", hmac_sha1_buf);

//----------------------------------------------------将加密结果进行Base64编码------------------------------------------------------
    olen = 0;
    memset(sign_buf, 0, sizeof(sign_buf));
    BASE64_Encode((unsigned char *)sign_buf, sizeof(sign_buf), &olen, (unsigned char *)hmac_sha1_buf, strlen(hmac_sha1_buf));

//----------------------------------------------------将Base64编码结果进行URL编码---------------------------------------------------
    OTA_UrlEncode(sign_buf);
//  printf("sign_buf: %s\r\n", sign_buf);

//----------------------------------------------------计算Token--------------------------------------------------------------------
    if(flag)
        snprintf(authorization_buf, authorization_buf_len, "version=%s&res=products%%2F%s&et=%d&method=%s&sign=%s", ver, res, et, METHOD, sign_buf);
    else
        snprintf(authorization_buf, authorization_buf_len, "version=%s&res=products%%2F%s%%2Fdevices%%2F%s&et=%d&method=%s&sign=%s", ver, res, dev_name, et, METHOD, sign_buf);
//  printf("Token: %s\r\n", authorization_buf);

    return 0;

}

//==========================================================
//  函数名称：   OneNET_RegisterDevice
//
//  函数功能：   在产品中注册一个设备
//
//  入口参数：   access_key：访问密钥
//              pro_id：产品ID
//              serial：唯一设备号
//              devid：保存返回的devid
//              key：保存返回的key
//
//  返回参数：   0-成功        1-失败
//
//  说明：
//==========================================================
_Bool OneNET_RegisterDevice(void)
{
    _Bool result = 1;
    unsigned short send_len = 11 + strlen(DEVICE_NAME);
    char *send_ptr = NULL, *data_ptr = NULL;

    char authorization_buf[144]; // 保存加密计算后的 token

    send_ptr = malloc(send_len + 240);
    if(send_ptr == NULL)
        return result;

    Serial_Printf(g_debug_uart4.p_ctrl,"OneNET: Creating TCP Socket to 183.230.40.33:80...\r\n");

    // W800 创建 TCP 客户端指令：AT+SKCT=<protocol>,<cs>,<host>,<remote_port>,<local_port>
    // 0:TCP, 0:Client, 183.230.40.33: OneNET API地址, 80: HTTP端口, 1000: 本地随意分配的端口
    while(W800_SendCmd("AT+SKCT=0,0,\"183.230.40.33\",80,1000\r\n", "OK"))
    {
        R_BSP_SoftwareDelay(500, BSP_DELAY_UNITS_MILLISECONDS);
    }
    Serial_Printf(g_debug_uart4.p_ctrl,"OneNET: TCP Socket Created Successfully.\r\n");

    // 计算鉴权 Token
    OneNET_Authorization("2018-10-31", PROID, 1956499200, ACCESS_KEY, NULL,
                            authorization_buf, sizeof(authorization_buf), 1);

    // 拼接 HTTP POST 请求报文
    snprintf(send_ptr, 280 + send_len, "POST /mqtt/v1/devices/reg HTTP/1.1\r\n"
                    "Authorization:%s\r\n"
                    "Host:ota.heclouds.com\r\n"
                    "Content-Type:application/json\r\n"
                    "Content-Length:%d\r\n\r\n"
                    "{\"name\":\"%s\"}",
                    authorization_buf, 11 + strlen(DEVICE_NAME), DEVICE_NAME);

    Serial_Printf(g_debug_uart4.p_ctrl,"OneNET: Sending HTTP POST Registration Request...\r\n");

    // 调用 W800 专属发送函数 (内部已封装 AT+SKSND=1,len 指令)
    W800_SendData((unsigned char *)send_ptr, strlen(send_ptr));

    // 等待平台响应 (HTTP 响应中会包含 JSON 格式的 device_id 等信息)
    data_ptr = (char *)W800_GetIPD(1000);

    if(data_ptr)
    {
        data_ptr = strstr(data_ptr, "device_id");
    }

    if(data_ptr)
    {
        char name[16];
        int pid = 0;

        // 使用 sscanf 提取返回的设备凭证
        if(sscanf(data_ptr, "device_id\" : \"%[^\"]\",\r\n\"name\" : \"%[^\"]\",\r\n\r\n\"pid\" : %d,\r\n\"key\" : \"%[^\"]\"", devid, name, &pid, key) == 4)
        {
            Serial_Printf(g_debug_uart4.p_ctrl,"OneNET: Register OK! device: %s, name: %s, pid: %d, key: %s\r\n", devid, name, pid, key);
            result = 0;
        }
        else
        {
            Serial_Printf(g_debug_uart4.p_ctrl,"OneNET: JSON Parse Failed.\r\n");
        }
    }
    else
    {
        Serial_Printf(g_debug_uart4.p_ctrl,"OneNET: Registration HTTP Response Timeout.\r\n");
    }

    // 释放内存
    free(send_ptr);

    Serial_Printf(g_debug_uart4.p_ctrl,"OneNET: Closing HTTP TCP Socket...\r\n");
    // W800 关闭 Socket 指令：AT+SKCLS=<socket> (刚才创建的 socket 通道通常为 1)
    W800_SendCmd("AT+SKCLS=1\r\n", "OK");

    return result;
}

//==========================================================
//  函数名称：   OneNet_DevLink
//
//  函数功能：   与onenet创建连接
//
//  入口参数：   无
//
//  返回参数：   1-成功    0-失败
//
//  说明：     与onenet平台建立连接
//==========================================================
_Bool OneNet_DevLink(void)
{

    MQTT_PACKET_STRUCTURE mqttPacket = {NULL, 0, 0, 0};                 //协议包

    unsigned char *dataPtr;

    char authorization_buf[160];

    _Bool status = 1;

    OneNET_Authorization("2018-10-31", PROID, 1956499200, ACCESS_KEY, DEVICE_NAME,
                                authorization_buf, sizeof(authorization_buf), 0);

    Serial_Printf(g_debug_uart4.p_ctrl,"OneNET_DevLink\r\n"
                            "NAME: %s,  PROID: %s,  KEY:%s\r\n"
                        , DEVICE_NAME, PROID, authorization_buf);

    if(MQTT_PacketConnect(PROID, authorization_buf, DEVICE_NAME, 256, 1, MQTT_QOS_LEVEL0, NULL, NULL, 0, &mqttPacket) == 0)
    {
        // === 修改 7：替换发送函数 ===
        W800_SendData(mqttPacket._data, mqttPacket._len);            //上传平台

        // === 修改 8：替换接收函数 ===
        dataPtr = W800_GetIPD(250);                                  //等待平台响应
        if(dataPtr != NULL)
        {
            if(MQTT_UnPacketRecv(dataPtr) == MQTT_PKT_CONNACK)
            {
                switch(MQTT_UnPacketConnectAck(dataPtr))
                {
                    case 0:Serial_Printf(g_debug_uart4.p_ctrl,"Tips:    连接成功\r\n");status = 0;break;

                    case 1:Serial_Printf(g_debug_uart4.p_ctrl,"WARN:    连接失败：协议错误\r\n");break;
                    case 2:Serial_Printf(g_debug_uart4.p_ctrl,"WARN:    连接失败：非法的clientid\r\n");break;
                    case 3:Serial_Printf(g_debug_uart4.p_ctrl,"WARN:    连接失败：服务器失败\r\n");break;
                    case 4:Serial_Printf(g_debug_uart4.p_ctrl,"WARN:    连接失败：用户名或密码错误\r\n");break;
                    case 5:Serial_Printf(g_debug_uart4.p_ctrl,"WARN:    连接失败：非法链接(比如token非法)\r\n");break;

                    default:Serial_Printf(g_debug_uart4.p_ctrl,"ERR:    连接失败：未知错误\r\n");break;
                }
            }
        }

        MQTT_DeleteBuffer(&mqttPacket);                             //删包
    }
    else
        Serial_Printf(g_debug_uart4.p_ctrl,"WARN:   MQTT_PacketConnect Failed\r\n");

    return status;

}
/*
************************************************************
* 函数名称：   OneNet_FillBuf
* 函数功能：   将 ERR 数列打包为 OneNet 物模型 JSON 格式
************************************************************
*/
extern const char *ERR_Strings[];
extern uint8_t current_index;
extern const char *Reason_Strings[];
extern const char *Advice_Strings[];


unsigned char OneNet_FillBuf(char *buf)
{
    // 彻底清空缓存区
    memset(buf, 0, 256);

    sprintf(buf, "{\"id\":123,\"dp\":{\"ERR\":[{\"v\":\"%s\"}],\"Advice\":[{\"v\":\"%s\"}],\"Reason\":[{\"v\":\"%s\"}]}}",
                    ERR_Strings[current_index],Advice_Strings[current_index] , Reason_Strings[current_index]);
    return strlen(buf);
}
//==========================================================
//  函数名称：   OneNet_SendData
//
//  函数功能：   上传数据到平台
//
//  入口参数：   type：发送数据的格式
//
//  返回参数：   无
//
//  说明：
//==========================================================
void OneNet_SendData(void)
{

    MQTT_PACKET_STRUCTURE mqttPacket = {NULL, 0, 0, 0};                                             //协议包

    char buf[256];

    short body_len = 0, i = 0;

    //printf("Tips:   OneNet_SendData-MQTT\r\n");

    memset(buf, 0, sizeof(buf));

    body_len = OneNet_FillBuf(buf);                                                                 //获取当前需要发送的数据流的总长度
    //printf("Im here\r\n");
    if(body_len)
    {
        Serial_Printf(g_debug_uart4.p_ctrl,"Raw JSON: %s\r\n", buf);
        if(MQTT_PacketSaveData(PROID, DEVICE_NAME, body_len, NULL, &mqttPacket) == 0)               //封包
        {
            for(; i < body_len; i++)
                mqttPacket._data[mqttPacket._len++] = buf[i];

            // === 修改 9：替换发送函数 ===
            W800_SendData(mqttPacket._data, mqttPacket._len);                                    //上传数据到平台
            //printf("Send %d Bytes\r\n", mqttPacket._len);

            MQTT_DeleteBuffer(&mqttPacket);                                                         //删包
        }
        else
            Serial_Printf(g_debug_uart4.p_ctrl,"WARN:   EDP_NewBuffer Failed\r\n");
    }

}


//==========================================================
//  函数名称：   OneNET_Publish
//
//  函数功能：   发布消息
//
//  入口参数：   topic：发布的主题
//              msg：消息内容
//
//  返回参数：   无
//
//  说明：
//==========================================================
void OneNET_Publish(const char *topic, const char *msg)
{

    MQTT_PACKET_STRUCTURE mqtt_packet = {NULL, 0, 0, 0};                        //协议包

    Serial_Printf(g_debug_uart4.p_ctrl,"Publish Topic: %s, Msg: %s\r\n", topic, msg);

    if(MQTT_PacketPublish(MQTT_PUBLISH_ID, topic, msg, strlen(msg), MQTT_QOS_LEVEL0, 0, 1, &mqtt_packet) == 0)
    {
        // === 修改 10：替换发送函数 ===
        W800_SendData(mqtt_packet._data, mqtt_packet._len);                  //向平台发送订阅请求

        MQTT_DeleteBuffer(&mqtt_packet);                                        //删包
    }

}

//==========================================================
//  函数名称：   OneNET_Subscribe
//
//  函数功能：   订阅
//
//  入口参数：   无
//
//  返回参数：   无
//
//  说明：
//==========================================================
void OneNET_Subscribe(void)
{

    MQTT_PACKET_STRUCTURE mqtt_packet = {NULL, 0, 0, 0};                        //协议包

    char topic_buf[58];
    const char *topic = topic_buf;

    snprintf(topic_buf, sizeof(topic_buf), "$sys/%s/%s/thing/property/set", PROID, DEVICE_NAME);

    Serial_Printf(g_debug_uart4.p_ctrl,"Subscribe Topic: %s\r\n", topic_buf);

    if(MQTT_PacketSubscribe(MQTT_SUBSCRIBE_ID, MQTT_QOS_LEVEL0, &topic, 1, &mqtt_packet) == 0)
    {
        // === 修改 11：替换发送函数 ===
        W800_SendData(mqtt_packet._data, mqtt_packet._len);                  //向平台发送订阅请求

        MQTT_DeleteBuffer(&mqtt_packet);                                        //删包
    }

}

//==========================================================
//  函数名称：   OneNet_RevPro
//
//  函数功能：   平台返回数据检测
//
//  入口参数：   dataPtr：平台返回的数据
//
//  返回参数：   无
//
//  说明：
//==========================================================
void OneNet_RevPro(unsigned char *cmd)
{
    char *req_payload = NULL;
        char *cmdid_topic = NULL;

        char reply_topic[48];
        char reply_msg[48];

        unsigned short topic_len = 0;
        unsigned short req_len = 0;

        unsigned char qos = 0;
        static unsigned short pkt_id = 0;

        unsigned char type = 0;
        short result = 0;

        cJSON *raw_json, *params_json; // 删除了 led_json

        type = MQTT_UnPacketRecv(cmd);
        switch(type)
        {
            case MQTT_PKT_PUBLISH:                                                              //接收的Publish消息

                result = MQTT_UnPacketPublish(cmd, &cmdid_topic, &topic_len, &req_payload, &req_len, &qos, &pkt_id);
                if(result == 0)
                {
                    Serial_Printf(g_debug_uart4.p_ctrl,"topic: %s, topic_len: %d, payload: %s, payload_len: %d\r\n",
                                                                        cmdid_topic, topic_len, req_payload, req_len);

                    raw_json = cJSON_Parse(req_payload);
                    params_json = cJSON_GetObjectItem(raw_json,"params");

                    // === LED 解析与执行逻辑已全部安全移除 ===

                    sprintf(reply_topic, "$sys/%s/%s/thing/property/set_reply", PROID, DEVICE_NAME);
                    sprintf(reply_msg, "{\"id\": \"%s\",\"code\": 200,\"msg\": \"success\"}", cJSON_GetObjectItem(raw_json,"id")->valuestring);

                    cJSON_Delete(raw_json);

                    OneNET_Publish(reply_topic,reply_msg);
                }
                break;

            case MQTT_PKT_PUBACK:                                                       //发送Publish消息，平台回复的Ack
                if(MQTT_UnPacketPublishAck(cmd) == 0)
                    Serial_Printf(g_debug_uart4.p_ctrl,"Tips:   MQTT Publish Send OK\r\n");
                break;

            case MQTT_PKT_SUBACK:                                                               //发送Subscribe消息的Ack
                if(MQTT_UnPacketSubscribe(cmd) == 0)
                    Serial_Printf(g_debug_uart4.p_ctrl,"Tips:   MQTT Subscribe OK\r\n");
                else
                    Serial_Printf(g_debug_uart4.p_ctrl,"Tips:   MQTT Subscribe Err\r\n");
                break;

            default:
                result = -1;
                break;
        }

        // === 修改 12：替换缓存清空函数 ===
        W800_Clear();                                    //清空缓存

        if(result == -1)
            return;

        if(type == MQTT_PKT_CMD || type == MQTT_PKT_PUBLISH)
        {
            MQTT_FreeBuffer(cmdid_topic);
            MQTT_FreeBuffer(req_payload);
        }

}


