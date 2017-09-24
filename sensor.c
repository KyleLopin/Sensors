/*******************************************************************************
 * File Name: sensor.c
 * Version 0.50
 *
 * Description:
 *  This file provides higher level I2C functions.
 *  A I2C master component with the name I2C is required
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

#include "sensor.h"

/***************************************
*      Static Function Prototypes
***************************************/  

static void sensor_read(uint8 address, uint8* buffer, uint8 _register, uint8 num_bytes);
static void sensor_write(uint8 address, uint8* buffer, uint8 num_bytes);

void sensor_write8(uint8 address, uint8* buffer) {   
    sensor_write(address, buffer, 2);
}

void sensor_write16(uint8 address, uint8* buffer) {   
    sensor_write(address, buffer, 3);
}

void sensor_write_n(uint8 address, uint8* buffer, uint8 num_bytes) {   
    sensor_write(address, buffer, num_bytes);
}

void sensor_write(uint8 address, uint8* buffer, uint8 num_bytes) {   
    uint8 status = 0;
    do {
        status = I2C_MasterWriteBuf(address, buffer, num_bytes, I2C_MODE_COMPLETE_XFER);
        for(;;) {
            if (0x00 != (I2C_MasterStatus() & I2C_MSTAT_WR_CMPLT)) {
                break;
            }
        }
    }
    while(status != I2C_MSTR_NO_ERROR);
}



uint8 sensor_read8(uint8 address, uint8* buffer, uint8 _register) {
    sensor_read(address, buffer, _register, 1);
    return buffer[0];
}

uint16 sensor_read16(uint8 address, uint8* buffer, uint8 _register) {
    sensor_read(address, buffer, _register, 2);
    return buffer[0] | (buffer[1] << 8);
}

void sensor_read_n(uint8 address, uint8* buffer, uint8 _register, uint8 num_bytes) {
    sensor_read(address, buffer, _register, num_bytes);
}

static void sensor_read(uint8 address, uint8* buffer, uint8 _register, uint8 num_bytes) {
    uint8 temp;
    do {
        temp = I2C_MasterWriteBuf(address, &_register, 1, I2C_MODE_NO_STOP);
        for(;;) {
            if (0x00 != (I2C_MasterStatus() & I2C_MSTAT_WR_CMPLT)) {
                break;
            }
        }
    }
    while (temp != I2C_MSTR_NO_ERROR);
    I2C_MasterClearWriteBuf();

    do {
        temp = I2C_MasterReadBuf(address, buffer, num_bytes, I2C_MODE_REPEAT_START);
        for(;;) {
            if (0x00 != (I2C_MasterStatus() & I2C_MSTAT_RD_CMPLT)) {
                break;
            }
        }
    }
    while (temp != I2C_MSTR_NO_ERROR);
    I2C_MasterClearReadBuf();
}

/* [] END OF FILE */
