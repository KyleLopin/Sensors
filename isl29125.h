/*******************************************************************************
 * File Name: isl29125.h
 * Version 0.50
 *
 * Description:
 *  This file provides constants and parameter values for the isl29125 light sensor.

 *
********************************************************************************
 *
 * Copyright by Kyle Lopin, Naresuan University, 2017
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF Naresuan University.
 *
 * ========================================
*/
#if !defined(_ISL29125_H)
#define _ISL29125_H
    
#include <project.h>
#include "sensor.h"
#include "stdbool.h"

    
/***************************************
*      Structures
***************************************/ 

typedef struct {
    uint8       working;  // Status of the device
    uint8       color_mode;
    uint8       intensity_range;
    uint8       adc_resolution;
    uint8       isr_setting;
    uint8       ir_offset;
    uint8       ir_setting;
    uint8       interrupt_color; 
    uint8       config_reg1;
    uint8       config_reg2;
    uint8       config_reg3;
} ISL29125;

ISL29125 isl29125;
    
    
#define ISL29125_I2C_ADDRESS        0x44  
#define DEVICE_RESET_CODE           0x46
#define DEVICE_ID                   0x7D

/***************************************
*    ISL29125 Register assignment
***************************************/ 
    
#define DEVICE_ID_REG               0x00
#define CONFIG_REG_1                0x01
#define CONFIG_REG_2                0x02
#define CONFIG_REG_3                0x03
#define THRESHOLD_REG_LL            0x04
#define THRESHOLD_REG_LH            0x05
#define THRESHOLD_REG_HL            0x06
#define THRESHOLD_REG_HH            0x07
#define STATUS_REG                  0x08 
#define GREEN_REG_L                 0x09 
#define GREEN_REG_H                 0x0A
#define RED_REG_L                   0x0B
#define RED_REG_H                   0x0C
#define BLUE_REG_L                  0x0D
#define BLUE_REG_H                  0x0E  
 
    
/***************************************
*       Configuration settings
***************************************/ 
    
// Configuration base settings
#define CONFIG_DEFAULT              0x00
    
// CONFIGURATION 1 REGISTER OPTIONS
// Pick color mode
#define CONFIG1_POWERDOWN           0x00
#define CONFIG1_GREEN_MODE          0x01
#define CONFIG1_RED_MODE            0x02
#define CONFIG1_BLUE_MODE           0x03
#define CONFIG1_STANDBY             0x04
#define CONFIG1_RGB_MODE            0x05
#define CONFIG1_RG_MODE             0x06
#define CONFIG1_GB_MODE             0x07
// Light intensity range
#define CONFIG1_375LUX              0x00
#define CONFIG1_10KLUX              0x08
// ADC accuracy
#define CONFIG1_ADC_16BIT           0x00
#define CONFIG1_ADC_12BIT           0x10
// interrupt pin setting
#define CONFIG1_NO_SYNC             0x00
#define CONFIG1_SYNC_TO_INT         0x20
    
// CONFIGURATION 2 REGISTER OPTIONS
// Range for IR filter
#define CONFIG2_IR_OFFSET_OFF       0x00  
#define CONFIG2_IR_OFFSET_ON        0x80   
// Sets amount of IR filtering, can use these presets or any value between 0x00 and 0x3F
// Consult datasheet for detailed IR filtering calibration
#define CONFIG2_IR_ADJUST_LOW       0x00
#define CONFIG2_IR_ADJUST_MID       0x20
#define CONFIG2_IR_ADJUST_HIGH      0x3F
    
// CONFIGURATION 3 REGISTER OPTIONS
// Chose what color fires an interupt
#define CONFIG3_NO_INT              0x00
#define CONFIG3_G_INT               0x01
#define CONFIG3_R_INT               0x02
#define CONFIG3_B_INT               0x03

// How many times a sensor sample must hit a threshold before triggering an interrupt
// More consecutive samples means more times between interrupts, but less triggers from short transients
#define CONFIG3_INT_1_TIME          0x00
#define CONFIG3_INT_2_TIME          0x04
#define CONFIG3_INT_4_TIME          0x08
#define CONFIG3_INT_8_TIME          0x0C
// Choose to have interrupt pin go low when a conversion is finished
#define CONFIG3_DISABLE_INT_ON_CONVERSION   0x00
#define CONFIG3_ENABLE_INT_ON_CONVERSION    0x10
    
    
/***************************************
*        Function Prototypes
***************************************/     

void isl29125_init(void);    
void isl29125_start(void);
void isl29125_sleep(void);
void isl29125_stop(void);
uint8 isl29125_read_id(void);

void set_upper_threshold(uint16 level);
void set_lower_threshold(uint16 level);

void set_adc_resolution(uint8 resolution);
void get_adc_resolution(uint8 resolution);

uint16 isl29125_read_red(void);
uint16 isl29125_read_green(void);
uint16 isl29125_read_blue(void);

#endif
/* [] END OF FILE */
