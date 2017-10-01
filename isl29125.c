/*******************************************************************************
 * File Name: isl29125.c
 * Version 0.50
 *
 * Description:
 *  This file provides the source code of APIs for the isl29125 light sensor.
 *  The sensor needs a I2C master component with the name I2C
 *
 *******************************************************************************
 * Copyright by Kyle Lopin, Naresuan University, 2017
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF Naresuan University.
 *
 * ========================================
 */
#include "isl29125.h"

/***************************************
*      I2C communication buffers
***************************************/  

uint8 static read_buffer[8];
uint8 static write_buffer[2];

/***************************************
*      Static Function Prototypes
***************************************/  

static inline bool isl29125_reset(void);
static inline bool isl29125_config(uint8 config1, uint8 config2, uint8 config3);

static uint8 config_register1(void);
static uint8 config_register2(void);
static uint8 config_register3(void);

static inline void isl29125_write8(uint8 _register, uint8 data);

static inline uint8 isl29125_read8(uint8 _register);
static inline uint16 isl29125_read16(uint8 _register);


/******************************************************************************
* Function Name: isl29125_init
*******************************************************************************
*
* Summary:
*  Initialize a isl29125 red, green, blue light sensor.  
*  First check that the device returns the correct ID.  Reset the device
*  and then configure it to read in the rgb mode, at the high lux setting
*  and with the ir adjustment set to high.  
*  Save to the isl29125 structure all the operational settings
*
* Global Parameters:
*  ISL29125 isl29125: structure to save settings of the isl29125
*
*******************************************************************************/

void isl29125_init(void) {
    uint8 data;
    
    data = isl29125_read8(ISL29125_DEVICE_ID_REG);
    if (data != ISL29125_DEVICE_ID) {
        isl29125.working = false;
    }
    if (!isl29125_reset()) {
        isl29125.working = false;
    }
    
    if (!isl29125_config(ISL29125_CONFIG1_RGB_MODE | ISL29125_CONFIG1_10KLUX, 
        ISL29125_CONFIG2_IR_ADJUST_HIGH, ISL29125_CONFIG3_NO_INT)) {
        isl29125.working = false;
    }
    isl29125.color_mode = ISL29125_CONFIG1_RGB_MODE;
    isl29125.intensity_range = ISL29125_CONFIG1_10KLUX;
    isl29125.adc_resolution = ISL29125_CONFIG1_ADC_16BIT;
    isl29125.isr_setting = ISL29125_CONFIG1_NO_SYNC;
    isl29125.ir_offset = ISL29125_CONFIG2_IR_OFFSET_OFF;
    isl29125.ir_setting = ISL29125_CONFIG2_IR_ADJUST_HIGH;
    isl29125.interrupt_color = ISL29125_CONFIG3_NO_INT;
    isl29125.working = true;
}

/******************************************************************************
* Function Name: isl29125_start
*******************************************************************************
*
* Summary:
*  Start the isl29125 in rgb mode
*
* Global Parameters:
*  ISL29125 isl29125: structure to save settings of the isl29125
*
*******************************************************************************/

void isl29125_start(void) {
    isl29125.color_mode = ISL29125_CONFIG1_RGB_MODE;
    isl29125.config_reg1 |= ISL29125_CONFIG1_RGB_MODE;
    isl29125_write8(ISL29125_CONFIG_REG_1, isl29125.config_reg1);
}

/******************************************************************************
* Function Name: isl29125_sleep
*******************************************************************************
*
* Summary:
*  Put the isl29125 into the standby mode
*
* Global Parameters:
*  ISL29125 isl29125: structure to save settings of the isl29125
*
*******************************************************************************/

void isl29125_sleep(void) {
    isl29125.color_mode = ISL29125_CONFIG1_STANDBY;
    isl29125.config_reg1 |= ISL29125_CONFIG1_STANDBY;
    isl29125_write8(ISL29125_CONFIG_REG_1, isl29125.config_reg1);
}

/******************************************************************************
* Function Name: isl29125_stop
*******************************************************************************
*
* Summary:
*  Power down the isl29125
*
* Global Parameters:
*  ISL29125 isl29125: structure to save settings of the isl29125
*
*******************************************************************************/

void isl29125_stop(void) {
    isl29125.color_mode = ISL29125_CONFIG1_POWERDOWN;
    isl29125.config_reg1 |= ISL29125_CONFIG1_POWERDOWN;
    isl29125_write8(ISL29125_CONFIG_REG_1, isl29125.config_reg1);
}

/******************************************************************************
* Function Name: isl29125_init
*******************************************************************************
*
* Summary:
*  Reset the isl29125 rgb sensor.  Send the reset code and then read the 
*  configuration registers to confirm the are set to 0
*
* Return:
*  bool: true if the device was reset, or false if not
*
*******************************************************************************/

static bool isl29125_reset(void) {
    uint8 data = 0x00;
    
    // Reset the registers
    isl29125_write8(ISL29125_DEVICE_ID_REG, ISL29125_DEVICE_RESET_CODE);
    
    // Check reset
    data = isl29125_read8(ISL29125_CONFIG_REG_1);
    data |= isl29125_read8(ISL29125_CONFIG_REG_2);
    data |= isl29125_read8(ISL29125_CONFIG_REG_3);
    data |= isl29125_read8(ISL29125_STATUS_REG);
    if (data == 0x00) {
        return true;
    }
    return false;
}

/******************************************************************************
* Function Name: isl29125_config
*******************************************************************************
*
* Summary:
*  Configure the isl29125 rgb sensor.  Write to the 3 configuration registers
*  and then confirm they are set properly
*
* Parameters:
*  uint8 config1: value to put in congiuration register 1
*  uint8 config2: value to put in congiuration register 2
*  uint8 config3: value to put in congiuration register 3
*
* Return:
*  bool: true if the device was properly configured, or false if not
*
*******************************************************************************/

static bool isl29125_config(uint8 config1, uint8 config2, uint8 config3) {
    uint8 data;  
    // set up the registers
    isl29125_write8(ISL29125_CONFIG_REG_1, config1);
    isl29125_write8(ISL29125_CONFIG_REG_2, config2);
    isl29125_write8(ISL29125_CONFIG_REG_3, config3);
    // confirm the registers are correct
    data = isl29125_read8(ISL29125_CONFIG_REG_1);
    if (data != config1) {
        return false;
    }
    data = isl29125_read8(ISL29125_CONFIG_REG_2);
    if (data != config2) {
        return false;
    }
    data = isl29125_read8(ISL29125_CONFIG_REG_3);
    if (data != config3) {
        return false;
    }
    return true;
}

/******************************************************************************
* Function Name: isl29125_read_id
*******************************************************************************
*
* Summary:
*  Read the ID code of the isl29125 rgb sensor, should be 0x74
*
* Return:
*  uint8: byte read from the device's ID register
*
*******************************************************************************/

uint8 isl29125_read_id(void) {
    return isl29125_read8(ISL29125_DEVICE_ID_REG); 
}

/******************************************************************************
* Function Name: isl29125_read_red
*******************************************************************************
*
* Summary:
*  Read the red value of the isl29125 rgb sensor.  The device has to be already 
*  running. 
*
* Return:
*  uint16: value of the red registers.
*
*******************************************************************************/

uint16 isl29125_read_red(void) {
    return isl29125_read16(ISL29125_RED_REG_L);
}

/******************************************************************************
* Function Name: isl29125_read_green
*******************************************************************************
*
* Summary:
*  Read the green value of the isl29125 rgb sensor.  The device has to be already 
*  running. 
*
* Return:
*  uint16: value of the green registers.
*
*******************************************************************************/

uint16 isl29125_read_green(void) {
    return isl29125_read16(ISL29125_GREEN_REG_L);
}

/******************************************************************************
* Function Name: isl29125_read_blue
*******************************************************************************
*
* Summary:
*  Read the blue value of the isl29125 rgb sensor.  The device has to be already 
*  running. 
*
* Return:
*  uint16: value of the blue registers.
*
*******************************************************************************/

uint16 isl29125_read_blue(void) {
    return isl29125_read16(ISL29125_BLUE_REG_L);
}

/******************************************************************************
* Function Name: isl29125_config_register1
*******************************************************************************
*
* Summary:
*  Take the isl29125 setting and make the byte the first
*   configuration register should be set to.
*
* Return:
*  uint8: value to set the isl29125's first configuration register to
*
*******************************************************************************/

static uint8 config_register1(void){
    return (isl29125.color_mode | isl29125.intensity_range | 
            isl29125.adc_resolution | isl29125.isr_setting);
}

/******************************************************************************
* Function Name: isl29125_config_register2
*******************************************************************************
*
* Summary:
*  Take the isl29125 setting and make the byte the second
*   configuration register should be set to.
*
* Return:
*  uint8: value to set the isl29125's second configuration register to
*
*******************************************************************************/

static uint8 config_register2(void){
    return (isl29125.ir_setting | isl29125.ir_offset);
}

/******************************************************************************
* Function Name: isl29125_config_register3
*******************************************************************************
*
* Summary:
*  Take the isl29125 setting and make the byte the third
*   configuration register should be set to.
*
* Return:
*  uint8: value to set the isl29125's third configuration register to
*
*******************************************************************************/

static uint8 config_register3(void){
    return (isl29125.interrupt_color);
}

/*****************************************************************************
* Function Name: isl29125_write8
*******************************************************************************
*
* Summary:
*  Write a byte of data to the specified register.  Uses the sensors.c library
*
* Parameters:
*  uint8 _register: register of isl29125 to write to
*  uint8 data: value to put into the _register of the isl29125
*
*******************************************************************************/

static void isl29125_write8(uint8 _register, uint8 data) {
    write_buffer[0] = _register;
    write_buffer[1] = data;
    sensor_write8(ISL29125_I2C_ADDRESS, write_buffer);
}

/*****************************************************************************
* Function Name: isl29A125_read8
*******************************************************************************
*
* Summary:
*  Read a byte of data from the specified register.  Uses the sensors.c library
*
* Parameters:
*  uint8 _register: register of isl29125 to read from
*  uint8 data: value to put into the _register of the isl29125
*
* Return:
*  uint8: value of the register.
*
*******************************************************************************/

static uint8 isl29125_read8(uint8 _register) {
    return sensor_read8(ISL29125_I2C_ADDRESS, read_buffer, _register);
}

/*****************************************************************************
* Function Name: isl29125_read16
*******************************************************************************
*
* Summary:
*  Read 2 bytes of data starting from the specified register. Uses the sensors.c library
*
* Parameters:
*  uint8 _register: register of isl29125 to read from
*
* Return:
*  uint16: value of the registers.
*
*******************************************************************************/

static uint16 isl29125_read16(uint8 _register) {
    return sensor_read16(ISL29125_I2C_ADDRESS, read_buffer, _register);
}


/* [] END OF FILE */