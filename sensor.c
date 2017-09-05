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

static void sensor_read(uint8 _address, uint8 _register, uint8* read_buffer, uint8 num_bytes);

void sensor_write8(uint8 _address, uint8* write_buffer) {   
    uint8 status = 0;
    do {
        status = I2C_MasterWriteBuf(_address, write_buffer, 2, I2C_MODE_COMPLETE_XFER);
        for(;;) {
            if (0x00 != (I2C_MasterStatus() & I2C_MSTAT_WR_CMPLT)) {
                break;
            }
        }
    }
    while(status != I2C_MSTR_NO_ERROR);
}

uint8 sensor_read8(uint8 _address, uint8 _register, uint8* read_buffer) {
    sensor_read(_address, _register, read_buffer, 1);
    return read_buffer[0];
}

uint16 sensor_read16(uint8 _address, uint8 _register, uint8* read_buffer) {
    sensor_read(_address, _register, read_buffer, 2);
    return read_buffer[0] | (read_buffer[1] << 8);
}

static void sensor_read(uint8 _address, uint8 _register, uint8* read_buffer, uint8 num_bytes) {
    uint8 temp;
    do {
        temp = I2C_MasterWriteBuf(_address, &_register, 1, I2C_MODE_NO_STOP);
        for(;;) {
            if (0x00 != (I2C_MasterStatus() & I2C_MSTAT_WR_CMPLT)) {
                break;
            }
        }
    }
    while (temp != I2C_MSTR_NO_ERROR);
    I2C_MasterClearWriteBuf();

    do {
        temp = I2C_MasterReadBuf(_address, read_buffer, num_bytes, I2C_MODE_REPEAT_START);
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
