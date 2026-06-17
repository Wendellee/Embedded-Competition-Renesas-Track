/* generated common header file - do not edit */
#ifndef COMMON_DATA_H_
#define COMMON_DATA_H_
#include <stdint.h>
#include "bsp_api.h"
#include "rm_rai_data_collector.h"
#include "r_sci_uart.h"
#include "r_uart_api.h"
#include "../ra/fsp/src/rm_comms_lock/rm_comms_lock.h"
#include "rm_comms_uart.h"
#include "rm_comms_api.h"
#include "rm_rai_data_shipper.h"
#include "r_ioport.h"
#include "bsp_pin_cfg.h"
FSP_HEADER
/* RAI Data Collector Interface */
extern const rai_data_collector_instance_t g_rai_data_collector0;

/** Access the RAI Data Collector instance using these structures
 when calling API functions directly (::p_api is not used). */
extern rai_data_collector_instance_ctrl_t g_rai_data_collector0_ctrl;

extern const rai_data_collector_cfg_t g_rai_data_collector0_cfg;

#ifndef rai_data_collector0_callback
void rai_data_collector0_callback(const rai_data_collector_callback_args_t *p_args);
#endif

#ifndef rai_data_collector0_error_callback
void rai_data_collector0_error_callback(const rai_data_collector_error_callback_args_t *p_args);
#endif

fsp_err_t g_rai_data_collector0_init();
/** UART on SCI Instance. */
extern const uart_instance_t g_uart7;

/** Access the UART instance using these structures when calling API functions directly (::p_api is not used). */
extern sci_uart_instance_ctrl_t g_uart7_ctrl;
extern const uart_cfg_t g_uart7_cfg;
extern const sci_uart_extended_cfg_t g_uart7_cfg_extend;

#ifndef NULL
void NULL(uart_callback_args_t *p_args);
#endif
/* UART Communication Device */
extern const rm_comms_instance_t g_comms_uart0;
extern rm_comms_uart_instance_ctrl_t g_comms_uart0_ctrl;
extern const rm_comms_cfg_t g_comms_uart0_cfg;

#ifndef NULL
void NULL(rm_comms_callback_args_t *p_args);
#endif
/* Data Shipper Interface */
extern const rai_data_shipper_instance_t g_rai_data_shipper0;

/** Access the Data Shipper instance using these structures
 when calling API functions directly (::p_api is not used). */
extern rai_data_shipper_instance_ctrl_t g_rai_data_shipper0_ctrl;

extern const rai_data_shipper_cfg_t g_rai_data_shipper0_cfg;

#ifndef rai_data_shipper0_callback
void rai_data_shipper0_callback(rai_data_shipper_callback_args_t *p_args);
#endif
#define IOPORT_CFG_NAME g_bsp_pin_cfg
#define IOPORT_CFG_OPEN R_IOPORT_Open
#define IOPORT_CFG_CTRL g_ioport_ctrl

/* IOPORT Instance */
extern const ioport_instance_t g_ioport;

/* IOPORT control structure. */
extern ioport_instance_ctrl_t g_ioport_ctrl;
void g_common_init(void);
FSP_FOOTER
#endif /* COMMON_DATA_H_ */
