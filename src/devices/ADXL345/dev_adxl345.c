#include "dev_adxl345.h"
#include <devices.h>
#include <errno.h>
#include <libs.h>
#include "hal_data.h"
#include <stdio.h>
#include <stdlib.h>

/*! Reads the value of a register. */
static uint8_t adxl345_get_register_value(uint8_t register_address);
/*! Writes data into a register. */
static void adxl345_set_register_value(uint8_t register_address, uint8_t register_value);
/*! Burst reads from a ADXL345 Internal Register. */
static int adxl345_get_burst_register_value(unsigned char addr);
/*! Read all axes. */
static void adxl345_get_all_axes(void);
/*! Places the device into standby/measure mode. */
static void adxl345_set_power_mode(uint8_t pwr_mode);
/*! Reads the raw output data of each axis. */
static void adxl345_get_xyz(int16_t *x, int16_t *y, int16_t *z);
/*! Reads the raw output data of each axis and converts it to g. */
static void adxl345_get_g_xyz(float *x, float *y, float *z);
/*! Enables/disables the tap detection. */
static void adxl345_set_tap_detection(uint8_t tap_type,
                                      uint8_t tap_axes,
                                      uint8_t tap_dur,
                                      uint8_t tap_latent,
                                      uint8_t tap_window,
                                      uint8_t tap_thresh,
                                      uint8_t tap_int);
/*! Enables/disables the activity detection. */
static void adxl345_set_activity_detection(uint8_t act_on_off,
                                           uint8_t act_axes,
                                           uint8_t act_ac_dc,
                                           uint8_t act_thresh,
                                           uint8_t act_int);
/*! Enables/disables the inactivity detection. */
static void adxl345_set_inactivity_detection(uint8_t inact_on_off,
                                             uint8_t inact_axes,
                                             uint8_t inact_ac_dc,
                                             uint8_t inact_thresh,
                                             uint8_t inact_time,
                                             uint8_t inact_int);
/*! Enables/disables the free-fall detection. */
static void adxl345_set_free_fall_detection(uint8_t ff_on_off,
                                            uint8_t ff_thresh,
                                            uint8_t ff_time,
                                            uint8_t ff_int);
/*! Sets an offset value for each axis (Offset Calibration). */
static void adxl345_set_offset(uint8_t x_offset,uint8_t y_offset,uint8_t z_offset);
/*! Selects the measurement range. */
static void adxl345_set_range_resolution(uint8_t g_range,uint8_t full_res);

static int ADXLDevInit(struct ADXL345Dev *ptdev);
static int ADXLDevRead(struct ADXL345Dev *ptdev);
static int ADXLDevStart(struct ADXL345Dev *ptdev);
static int ADXLDevStop(struct ADXL345Dev *ptdev);

static IODevice  *pINT1IO = NULL;
static SPIDevice *pADXLSPI = NULL;
static ADXL345Device gADXL345 = {
    .name = "ADXL345",
    .selected_range = 2,
    .full_resolution_set = 0,
    .Init   = ADXLDevInit,
    .Read   = ADXLDevRead,
    .Start  = ADXLDevStart,
    .Stop   = ADXLDevStop
};

struct ADXL345Dev *ADXL345GetDevice(void)
{
    return &gADXL345;
}

static int ADXLDevInit(struct ADXL345Dev *ptdev)
{
    if(NULL == ptdev)       return -EINVAL;
    
    pINT1IO = IODeviceFind("ADXL345 INT1");
    if(NULL == pINT1IO)    return -ENXIO;

    pADXLSPI = SPIDeviceFind("ADXL345 SPI");
    if(NULL == pADXLSPI)    return -ENXIO;
    if(ESUCCESS != pADXLSPI->Init(pADXLSPI))    return -EIO;

    if (adxl345_get_register_value(ADXL345_DEVID) != ADXL345_ID)
    {
        xprintf("Failed to read ADXL345's ID!\r\n");
        return -EIO;
    }
    
    adxl345_set_power_mode(0x01);   /* measure(1)/standby(0) mode.*/
    adxl345_set_range_resolution(ADXL345_RANGE_PM_8G,  /* Range option. +- 16g */
                                 ADXL345_FULL_RES);     /*Enables full resolution*/
    adxl345_set_tap_detection(ADXL345_SINGLE_TAP | ADXL345_DOUBLE_TAP,    /* Tap type (none, single, double) */
                              ADXL345_TAP_X_EN | ADXL345_TAP_Y_EN | ADXL345_TAP_Z_EN, /* tap_axes */
                              0x0D,   /* tap_dur */
                              0x50,   /* tap_latent */
                              0xF0,   /* tap_window */
                              0x20,   /* Tap threshold */
                              0x00);    /* Interrupts pin.0x0 - interrupts on INT1 pin */
    adxl345_set_activity_detection(0x01,    /* enables(1)/disable(0) the activity detection. */
                                   ADXL345_ACT_X_EN | ADXL345_ACT_Y_EN | ADXL345_ACT_Z_EN,  /* Axes which participate in detecting activity. */
                                   0x00,    /* dc(0)/ac(ADXL345_ACT_ACDC)-coupled operation. */
                                   0x08,    /* Threshold value for detecting activity */
                                   0x00);   /* Interrupts pin.0-int1 */
    adxl345_set_free_fall_detection(0x01,   /* disables(0)/enable(1) the free-fall detection. */
                                    0x08,   /* Threshold value for free-fall detection. The scale factor */
                                    0x0A,   /* Time value for free-fall detection */
                                    0x00);  /* Interrupts pin. */
	adxl345_get_all_axes();

    return ESUCCESS;
}

static int ADXLDevRead(struct ADXL345Dev *ptdev)
{
    if(NULL == ptdev)       return -EINVAL;
    if(NULL == pADXLSPI)    return -EIO;

    adxl345_get_g_xyz(&ptdev->value.x, &ptdev->value.y, &ptdev->value.z);
    return ESUCCESS;
}

static int ADXLDevStart(struct ADXL345Dev *ptdev)
{
    if(NULL == ptdev)       return -EINVAL;
    adxl345_set_register_value(ADXL345_POWER_CTL, ADXL345_PCTL_MEASURE);
    return ESUCCESS;
}

static int ADXLDevStop(struct ADXL345Dev *ptdev)
{
    if(NULL == ptdev)       return -EINVAL;
    adxl345_set_register_value(ADXL345_POWER_CTL, ADXL345_PCTL_SLEEP);
    return ESUCCESS;
}

/***************************************************************************//**
 * @brief Reads the value of a register.
 *
 * @param register_address - Address of the register.
 *
 * @return register_value  - Value of the register.
*******************************************************************************/
static uint8_t adxl345_get_register_value(uint8_t register_address)
{
    if(NULL == pADXLSPI)    return 0;

    uint8_t data_buffer[2] = {0, 0};
    uint8_t rxbuffer[2] = {0, 0};
    uint8_t register_value = 0;
    data_buffer[0] = ADXL345_SPI_READ | register_address;
    data_buffer[1] = 0;
    if(ESUCCESS != pADXLSPI->WriteRead(pADXLSPI, data_buffer, rxbuffer, 2))  return 0;
    register_value = rxbuffer[1];
    return register_value;
}

/***************************************************************************//**
 * @brief Writes data into a register.
 *
 * @param register_address - Address of the register.
 * @param register_value   - Data value to write.
 *
 * @return None.
*******************************************************************************/
static void adxl345_set_register_value(uint8_t register_address, uint8_t register_value)
{
    if(NULL == pADXLSPI)    return;

    uint8_t data_buffer[2] = {0, 0};
    uint8_t rxbuffer[2] = {0, 0};
    data_buffer[0] = ADXL345_SPI_WRITE | register_address;
    data_buffer[1] = register_value;
    pADXLSPI->WriteRead(pADXLSPI, data_buffer, rxbuffer, 2);
}

/******************************************************************************
* @brief Burst reads from a ADXL345 Internal Register.
*
* @param addr 	- First Register address.
*
* @return rxData - Data read from ADXL345.
******************************************************************************/
static int adxl345_get_burst_register_value(unsigned char addr)
{
    if(NULL == pADXLSPI)    return -EINVAL;
	unsigned char txBuffer[3] 	= {0x00, 0x00, 0x00};
	unsigned char rxBuffer[3] 	= {0x00, 0x00, 0x00};
	int  rxData 	 	= 0;

	txBuffer[0] = (1 << 7) | (1 << 6) | (addr & 0x3F);
	txBuffer[1] = 0x00;

    pADXLSPI->WriteRead(pADXLSPI, txBuffer, rxBuffer, 3);
	rxData = ((rxBuffer[2] & 0xFF) << 8) | (rxBuffer[1] & 0xFF);

	return(rxData);
}

/******************************************************************************
* @brief Read all axes.
*
* @param None.
*
* @return None.
******************************************************************************/
static void adxl345_get_all_axes(void)
{
	adxl345_get_burst_register_value(ADXL345_DATAX0);
	adxl345_get_burst_register_value(ADXL345_DATAY0);
	adxl345_get_burst_register_value(ADXL345_DATAZ0);
}

/***************************************************************************//**
 * @brief Places the device into standby/measure mode.
 *
 * @param pwr_mode - Power mode.
 *                   Example: 0x0 - standby mode.
 *                            0x1 - measure mode.
 *
 * @return None.
*******************************************************************************/
static void adxl345_set_power_mode(uint8_t pwr_mode)
{
    uint8_t old_power_ctl = 0;
    uint8_t new_power_ctl = 0;

    old_power_ctl = adxl345_get_register_value(ADXL345_POWER_CTL);
    new_power_ctl = old_power_ctl & ~ADXL345_PCTL_MEASURE;
    new_power_ctl = new_power_ctl | (pwr_mode * ADXL345_PCTL_MEASURE);
    adxl345_set_register_value(ADXL345_POWER_CTL, new_power_ctl);
}

/***************************************************************************//**
 * @brief Reads the raw output data of each axis.
 *
 * @param x   - X-axis's output data.
 * @param y   - Y-axis's output data.
 * @param z   - Z-axis's output data.
 *
 * @return None.
*******************************************************************************/
static void adxl345_get_xyz(int16_t *x, int16_t *y, int16_t *z)
{
    if(NULL == pADXLSPI)    return;

    uint8_t first_reg_address = ADXL345_DATAX0;
    uint8_t read_buffer[7]    = {0, 0, 0, 0, 0, 0, 0};
    
    read_buffer[0] = ADXL345_SPI_READ | ADXL345_SPI_MB | first_reg_address;
    pADXLSPI->WriteRead(pADXLSPI, read_buffer, read_buffer, 7);
    /* x = ((ADXL345_DATAX1) << 8) + ADXL345_DATAX0 */
    *x = (int16_t)((read_buffer[2] << 8) + read_buffer[1]);
    /* y = ((ADXL345_DATAY1) << 8) + ADXL345_DATAY0 */  
    *y = (int16_t)((read_buffer[4] << 8) + read_buffer[3]);
    /* z = ((ADXL345_DATAZ1) << 8) + ADXL345_DATAZ0 */  
    *z = (int16_t)((read_buffer[6] << 8) + read_buffer[5]);
}

/***************************************************************************//**
 * @brief Reads the raw output data of each axis and converts it to g.
 *
 * @param x   - X-axis's output data.
 * @param y   - Y-axis's output data.
 * @param z   - Z-axis's output data.
 *
 * @return None.
*******************************************************************************/
static void adxl345_get_g_xyz(float *x, float *y, float *z)
{
    int16_t x_data = 0;  // X-axis's output data.
    int16_t y_data = 0;  // Y-axis's output data.
    int16_t z_data = 0;  // Z-axis's output data.

    adxl345_get_xyz(&x_data, &y_data, &z_data);
    *x = (float)(gADXL345.full_resolution_set  ? (x_data * ADXL345_SCALE_FACTOR) :
                (x_data * ADXL345_SCALE_FACTOR * (gADXL345.selected_range >> 1)));
    *y = (float)(gADXL345.full_resolution_set  ? (y_data * ADXL345_SCALE_FACTOR) :
                (y_data * ADXL345_SCALE_FACTOR * (gADXL345.selected_range >> 1)));
    *z = (float)(gADXL345.full_resolution_set  ? (z_data * ADXL345_SCALE_FACTOR) :
                (z_data * ADXL345_SCALE_FACTOR * (gADXL345.selected_range >> 1)));
}

/***************************************************************************//**
 * @brief Enables/disables the tap detection.
 *
 * @param tap_type   - Tap type (none, single, double).
 *                     Example: 0x0 - disables tap detection.
 *              ADXL345_SINGLE_TAP - enables single tap
 *                                                   detection.
 *              ADXL345_DOUBLE_TAP - enables double tap
 *                                                   detection.
 * @param tap_axes   - Axes which participate in tap detection.
 *                     Example: 0x0 - disables axes participation.
 *              ADXL345_TAP_X_EN - enables x-axis participation.
 *              ADXL345_TAP_Y_EN - enables y-axis participation.
 *              ADXL345_TAP_Z_EN - enables z-axis participation.
 * @param tap_dur    - Tap duration. The scale factor is 625us is/LSB.
 * @param tap_latent - Tap latency. The scale factor is 1.25 ms/LSB.
 * @param tap_window - Tap window. The scale factor is 1.25 ms/LSB.
 * @param tap_thresh - Tap threshold. The scale factor is 62.5 mg/LSB.
 * @param tap_int    - Interrupts pin.
 *                     Example: 0x0 - interrupts on INT1 pin.
 *              ADXL345_SINGLE_TAP - single tap interrupts on
 *                           INT2 pin.
 *              ADXL345_DOUBLE_TAP - double tap interrupts on
 *                           INT2 pin.
 *
 * @return None.adxl345_set_tap_detection(ADXL345_SINGLE_TAP | ADXL345_DOUBLE_TAP, ADXL345_TAP_X_EN | ADXL345_TAP_Y_EN | ADXL345_TAP_Z_EN, 0x0D, 0x50)
*******************************************************************************/
static void adxl345_set_tap_detection(uint8_t tap_type,
                                      uint8_t tap_axes,
                                      uint8_t tap_dur,
                                      uint8_t tap_latent,
                                      uint8_t tap_window,
                                      uint8_t tap_thresh,
                                      uint8_t tap_int)
{
    uint8_t old_tap_axes = 0;
    uint8_t new_tap_axes = 0;
    uint8_t old_int_map = 0;
    uint8_t new_int_map = 0;
    uint8_t old_int_enable = 0;
    uint8_t new_int_enable = 0;

    old_tap_axes = adxl345_get_register_value(ADXL345_TAP_AXES);
    new_tap_axes = old_tap_axes & ~(ADXL345_TAP_X_EN |ADXL345_TAP_Y_EN |ADXL345_TAP_Z_EN);
    new_tap_axes = new_tap_axes | tap_axes;
    adxl345_set_register_value(ADXL345_TAP_AXES, new_tap_axes);
    adxl345_set_register_value(ADXL345_DUR, tap_dur);
    adxl345_set_register_value(ADXL345_LATENT, tap_latent);
    adxl345_set_register_value(ADXL345_WINDOW, tap_window);
    adxl345_set_register_value(ADXL345_THRESH_TAP, tap_thresh);
    old_int_map = adxl345_get_register_value(ADXL345_INT_MAP);
    new_int_map = old_int_map & (~(ADXL345_SINGLE_TAP | ADXL345_DOUBLE_TAP));
    new_int_map = new_int_map | tap_int;
    adxl345_set_register_value(ADXL345_INT_MAP, new_int_map);
    old_int_enable = adxl345_get_register_value(ADXL345_INT_ENABLE);
    new_int_enable = old_int_enable & (~(ADXL345_SINGLE_TAP | ADXL345_DOUBLE_TAP));
    new_int_enable = new_int_enable | tap_type;
    adxl345_set_register_value(ADXL345_INT_ENABLE, new_int_enable);
}

/***************************************************************************//**
 * @brief Enables/disables the activity detection.
 *
 * @param act_on_off - Enables/disables the activity detection.
 *                    Example: 0x0 - disables the activity detection.
 *                             0x1 - enables the activity detection.
 * @param act_axes   - Axes which participate in detecting activity.
 *                    Example: 0x0 - disables axes participation.
 *                             ADXL345_ACT_X_EN - enables x-axis participation.
 *                             ADXL345_ACT_Y_EN - enables y-axis participation.
 *                             ADXL345_ACT_Z_EN - enables z-axis participation.
 * @param act_ac_dc  - Selects dc-coupled or ac-coupled operation.
 *                    Example: 0x0 - dc-coupled operation.
 *                             ADXL345_ACT_ACDC - ac-coupled operation.
 * @param act_thresh - Threshold value for detecting activity. The scale factor
                      is 62.5 mg/LSB.
 * @param act_int    - Interrupts pin.
 *                    Example: 0x0 - activity interrupts on INT1 pin.
 *                             ADXL345_ACTIVITY - activity interrupts on INT2
 *                                                pin.
 *
 * @return None.
*******************************************************************************/
static void adxl345_set_activity_detection(uint8_t act_on_off,
                                           uint8_t act_axes,
                                           uint8_t act_ac_dc,
                                           uint8_t act_thresh,
                                           uint8_t act_int)
{
    uint8_t old_act_inact_ctl = 0;
    uint8_t new_act_inact_ctl = 0;
    uint8_t old_int_map = 0;
    uint8_t new_int_map = 0;
    uint8_t old_int_enable = 0;
    uint8_t new_int_enable = 0;

    old_act_inact_ctl = adxl345_get_register_value(ADXL345_INT_ENABLE);
    new_act_inact_ctl = old_act_inact_ctl & ~(ADXL345_ACT_ACDC | ADXL345_ACT_X_EN | ADXL345_ACT_Y_EN | ADXL345_ACT_Z_EN);
    new_act_inact_ctl = new_act_inact_ctl | (act_ac_dc | act_axes);
    adxl345_set_register_value(ADXL345_ACT_INACT_CTL, new_act_inact_ctl);
    adxl345_set_register_value(ADXL345_THRESH_ACT, act_thresh);
    old_int_map = adxl345_get_register_value(ADXL345_INT_MAP);
    new_int_map = old_int_map & ~(ADXL345_ACTIVITY);
    new_int_map = new_int_map | act_int;
    adxl345_set_register_value(ADXL345_INT_MAP, new_int_map);
    old_int_enable = adxl345_get_register_value(ADXL345_INT_ENABLE);
    new_int_enable = old_int_enable & ~(ADXL345_ACTIVITY);
    new_int_enable = new_int_enable | (ADXL345_ACTIVITY * act_on_off);
    adxl345_set_register_value(ADXL345_INT_ENABLE, new_int_enable);
}

/***************************************************************************//**
 * @brief Enables/disables the inactivity detection.
 *
 * @param inact_on_off - Enables/disables the inactivity detection.
 *                       Example: 0x0 - disables the inactivity detection.
 *                                0x1 - enables the inactivity detection.
 * @param inact_axes   - Axes which participate in detecting inactivity.
 *                       Example: 0x0 - disables axes participation.
 *                                ADXL345_INACT_X_EN - enables x-axis.
 *                                ADXL345_INACT_Y_EN - enables y-axis.
 *                                ADXL345_INACT_Z_EN - enables z-axis.
 * @param inact_ac_dc  - Selects dc-coupled or ac-coupled operation.
 *                       Example: 0x0 - dc-coupled operation.
 *                                ADXL345_INACT_ACDC - ac-coupled operation.
 * @param inact_thresh - Threshold value for detecting inactivity. The scale
                         factor is 62.5 mg/LSB.
 * @param inact_time   - Inactivity time. The scale factor is 1 sec/LSB.
 * @param inact_int    - Interrupts pin.
 *               Example: 0x0 - inactivity interrupts on INT1 pin.
 *                ADXL345_INACTIVITY - inactivity interrupts on
 *                             INT2 pin.
 *
 * @return None.
*******************************************************************************/
static void adxl345_set_inactivity_detection(uint8_t inact_on_off,
                                             uint8_t inact_axes,
                                             uint8_t inact_ac_dc,
                                             uint8_t inact_thresh,
                                             uint8_t inact_time,
                                             uint8_t inact_int)
{
    uint8_t old_act_inact_ctl = 0;
    uint8_t new_act_inact_ctl = 0;
    uint8_t old_int_map = 0;
    uint8_t new_int_map = 0;
    uint8_t old_int_enable = 0;
    uint8_t new_int_enable = 0;

    old_act_inact_ctl = adxl345_get_register_value(ADXL345_INT_ENABLE);
    new_act_inact_ctl = old_act_inact_ctl & ~(ADXL345_INACT_ACDC | ADXL345_INACT_X_EN | ADXL345_INACT_Y_EN | ADXL345_INACT_Z_EN);
    new_act_inact_ctl = new_act_inact_ctl | (inact_ac_dc | inact_axes);
    adxl345_set_register_value(ADXL345_ACT_INACT_CTL, new_act_inact_ctl);
    adxl345_set_register_value(ADXL345_THRESH_INACT, inact_thresh);
    adxl345_set_register_value(ADXL345_TIME_INACT, inact_time);
    old_int_map = adxl345_get_register_value(ADXL345_INT_MAP);
    new_int_map = old_int_map & ~(ADXL345_INACTIVITY);
    new_int_map = new_int_map | inact_int;
    adxl345_set_register_value(ADXL345_INT_MAP, new_int_map);
    old_int_enable = adxl345_get_register_value( ADXL345_INT_ENABLE);
    new_int_enable = old_int_enable & ~(ADXL345_INACTIVITY);
    new_int_enable = new_int_enable | (ADXL345_INACTIVITY * inact_on_off);
    adxl345_set_register_value(ADXL345_INT_ENABLE, new_int_enable);
}

/***************************************************************************//**
 * @brief Enables/disables the free-fall detection.
 *
 * @param ff_on_off - Enables/disables the free-fall detection.
 *                    Example: 0x0 - disables the free-fall detection.
 *                             0x1 - enables the free-fall detection.
 * @param ff_thresh - Threshold value for free-fall detection. The scale factor
 *                    is 62.5 mg/LSB.
 * @param ff_time   - Time value for free-fall detection. The scale factor is
 *                    5 ms/LSB.
 * @param ff_int    - Interrupts pin.
 *            Example: 0x0 - free-fall interrupts on INT1 pin.
 *                 ADXL345_FREE_FALL - free-fall interrupts on
 *                                                 INT2 pin.
 *
 * @return None.
*******************************************************************************/
static void adxl345_set_free_fall_detection(uint8_t ff_on_off,
                                            uint8_t ff_thresh,
                                            uint8_t ff_time,
                                            uint8_t ff_int)
{
    uint8_t old_int_map = 0;
    uint8_t new_int_map = 0;
    uint8_t old_int_enable = 0;
    uint8_t new_int_enable = 0;

    adxl345_set_register_value(ADXL345_THRESH_FF, ff_thresh);
    adxl345_set_register_value(ADXL345_TIME_FF, ff_time);
    old_int_map = adxl345_get_register_value(ADXL345_INT_MAP);
    new_int_map = old_int_map & ~(ADXL345_FREE_FALL);
    new_int_map = new_int_map | ff_int;
    adxl345_set_register_value(ADXL345_INT_MAP, new_int_map);
    old_int_enable = adxl345_get_register_value(ADXL345_INT_ENABLE);
    new_int_enable = old_int_enable & ~ADXL345_FREE_FALL;
    new_int_enable = new_int_enable | (ADXL345_FREE_FALL * ff_on_off);
    adxl345_set_register_value(ADXL345_INT_ENABLE, new_int_enable);
}

/***************************************************************************//**
 * @brief Sets an offset value for each axis (Offset Calibration).
 *
 * @param x_offset - X-axis's offset.
 * @param y_offset - Y-axis's offset.
 * @param z_offset - Z-axis's offset.
 *
 * @return None.
*******************************************************************************/
static void adxl345_set_offset(uint8_t x_offset,uint8_t y_offset,uint8_t z_offset)
{
    adxl345_set_register_value(ADXL345_OFSX, x_offset);
    adxl345_set_register_value(ADXL345_OFSY, y_offset);
    adxl345_set_register_value(ADXL345_OFSZ, z_offset);
}

/***************************************************************************//**
 * @brief Selects the measurement range.
 *
 * @param g_range  - Range option.
 *                   Example: ADXL345_RANGE_PM_2G  - +-2 g
 *                            ADXL345_RANGE_PM_4G  - +-4 g
 *                            ADXL345_RANGE_PM_8G  - +-8 g
 *                            ADXL345_RANGE_PM_16G - +-16 g
 * @param full_res - Full resolution option.
 *                   Example: 0x0 - Disables full resolution.
 *                            ADXL345_FULL_RES - Enables full resolution.
 *
 * @return None.
*******************************************************************************/
static void adxl345_set_range_resolution(uint8_t g_range,uint8_t full_res)
{
    uint8_t old_data_format = 0;
    uint8_t new_data_format = 0;

    old_data_format = adxl345_get_register_value(ADXL345_DATA_FORMAT);
    new_data_format = old_data_format & (~(ADXL345_RANGE(0x3) | ADXL345_FULL_RES));
    new_data_format =  new_data_format | ADXL345_RANGE(g_range) | full_res;
    adxl345_set_register_value(ADXL345_DATA_FORMAT, new_data_format);
    gADXL345.selected_range = (uint8_t)(1 << (g_range + 1));
    gADXL345.full_resolution_set = full_res ? 1 : 0;
}
