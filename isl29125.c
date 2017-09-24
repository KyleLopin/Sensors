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

uint8 read_buffer[8];
uint8 write_buffer[2];


/***************************************
*      Static Function Prototypes
***************************************/  

static inline bool isl29125_reset(void);
static inline bool isl29125_config(uint8 config1, uint8 config2, uint8 config3);

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
*
* Return:
*  bool: true if the device was initialized, or false if not
*
*******************************************************************************/

void isl29125_init(void) {
    uint8 data;
    
    data = isl29125_read8(DEVICE_ID_REG);
    if (data != DEVICE_ID) {
        isl29125.working = false;
    }
    if (!isl29125_reset()) {
        isl29125.working = false;
    }
    
    if (!isl29125_config(CONFIG1_RGB_MODE | CONFIG1_10KLUX, CONFIG2_IR_ADJUST_HIGH, CONFIG_DEFAULT)) {
        isl29125.working = false;
    }
}


void isl29125_start(void) {
    isl29125_write8(CONFIG_REG_1, CONFIG1_RGB_MODE | CONFIG1_10KLUX);
}

void isl29125_sleep(void) {
    isl29125_write8(CONFIG_REG_1, CONFIG1_STANDBY | CONFIG1_10KLUX);
}

void isl29125_stop(void) {
    isl29125_write8(CONFIG_REG_1, CONFIG1_POWERDOWN | CONFIG1_10KLUX);
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
    isl29125_write8(DEVICE_ID_REG, DEVICE_RESET_CODE);
    
    // Check reset
    data = isl29125_read8(CONFIG_REG_1);
    data |= isl29125_read8(CONFIG_REG_2);
    data |= isl29125_read8(CONFIG_REG_3);
    data |= isl29125_read8(STATUS_REG);
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
    isl29125_write8(CONFIG_REG_1, config1);
    isl29125_write8(CONFIG_REG_2, config2);
    isl29125_write8(CONFIG_REG_3, config3);
    // confirm the registers are correct
    data = isl29125_read8(CONFIG_REG_1);
    if (data != config1) {
        return false;
    }
    data = isl29125_read8(CONFIG_REG_2);
    if (data != config2) {
        return false;
    }
    data = isl29125_read8(CONFIG_REG_3);
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
    return isl29125_read8(DEVICE_ID_REG); 
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
    return isl29125_read16(RED_REG_L);
  
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
    return isl29125_read16(GREEN_REG_L);
  
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
    return isl29125_read16(BLUE_REG_L);
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