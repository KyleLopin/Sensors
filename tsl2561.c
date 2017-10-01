/*******************************************************************************
 * File Name: tsl2591.c
 * Version 0.50
 *
 * Description:
 *  This file provides the source code of APIs for the tsl2591 light sensor.
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

#include "tsl2561.h"

/***************************************
*      I2C communication buffer
***************************************/  

uint8 static read_buffer[8];
uint8 static write_buffer[2];
#define TSL2561_I2C_ADDRESS         I2C_ADDRESS_FLOAT

/***************************************
*      Static Function Prototypes
***************************************/  


static inline void tsl2561_write8(uint8 _register, uint8 data);

static inline uint8 tsl2561_read8(uint8 _register);
//static inline uint16 tsl2561_read16(uint8 _register);


/******************************************************************************
* Function Name: tsl2561_Init
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

void tsl2561_Init(void) {
    uint8 id = tsl2561_read8(TSL2561_REG_ID);
    if (id != 0x0A) {
        tsl2561.working = false;
    }
    
    
    
    
    
    
}




/******************************************************************************
* Function Name: tsl2561_read_id
*******************************************************************************
*
* Summary:
*  Read the ID code of the tsl2561 lux sensor, should be 0x0A???
*
* Return:
*  uint8: byte read from the device's ID register
*
*******************************************************************************/

uint8 tsl2561_read_id(void) {
    return tsl2561_read8(TSL2561_REG_ID); 
}

/*****************************************************************************
* Function Name: tsl25691_read8
*******************************************************************************
*
* Summary:
*  Read a byte of data from the specified register.  Uses the sensors.c library
*
* Parameters:
*  uint8 _register: register of tsl2591 to read from
*  uint8 data: value to put into the _register of the tsl2591
*
* Return:
*  uint8: value of the register.
*
*******************************************************************************/

static uint8 tsl2561_read8(uint8 _register) {
    return sensor_read8(TSL2561_I2C_ADDRESS, read_buffer, _register);
}

/*****************************************************************************
* Function Name: tsl2561_write8
*******************************************************************************
*
* Summary:
*  Write a byte of data to the specified register.  Uses the sensors.c library
*
* Parameters:
*  uint8 _register: register of tsl2591 to write to
*  uint8 data: value to put into the _register of the tsl2591
*
*******************************************************************************/

static void tsl2561_write8(uint8 _register, uint8 data) {
    write_buffer[0] = _register;
    write_buffer[1] = data;
    sensor_write8(TSL2561_I2C_ADDRESS, write_buffer);
}
