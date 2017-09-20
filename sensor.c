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

static void sensor_read(SensorComm sensor_comm, uint8 _register, uint8 num_bytes);
static void sensor_write(SensorComm sensor_comm, uint8 num_bytes);

void sensor_write8(SensorComm sensor_comm) {   
    sensor_write(sensor_comm, 2);
}

void sensor_write16(SensorComm sensor_comm) {   
    sensor_write(sensor_comm, 3);
}

void sensor_write_n(SensorComm sensor_comm, uint8 num_bytes) {   
    sensor_write(sensor_comm, num_bytes);
}

void sensor_write(SensorComm sensor_comm, uint8 num_bytes) {   
    uint8 status = 0;
    do {
        status = I2C_MasterWriteBuf(sensor_comm.address, sensor_comm.buffer, num_bytes, I2C_MODE_COMPLETE_XFER);
        for(;;) {
            if (0x00 != (I2C_MasterStatus() & I2C_MSTAT_WR_CMPLT)) {
                break;
            }
        }
    }
    while(status != I2C_MSTR_NO_ERROR);
}



uint8 sensor_read8(SensorComm sensor_comm, uint8 _register) {
    sensor_read(sensor_comm, _register, 1);
    return sensor_comm.buffer[0];
}

uint16 sensor_read16(SensorComm sensor_comm, uint8 _register) {
    sensor_read(sensor_comm, _register, 2);
    return sensor_comm.buffer[0] | (sensor_comm.buffer[1] << 8);
}

void sensor_read_n(SensorComm sensor_comm, uint8 _register, uint8 num_bytes) {
    sensor_read(sensor_comm, _register, num_bytes);
}

static void sensor_read(SensorComm sensor_comm, uint8 _register, uint8 num_bytes) {
    uint8 temp;
    do {
        temp = I2C_MasterWriteBuf(sensor_comm.address, &_register, 1, I2C_MODE_NO_STOP);
        for(;;) {
            if (0x00 != (I2C_MasterStatus() & I2C_MSTAT_WR_CMPLT)) {
                break;
            }
        }
    }
    while (temp != I2C_MSTR_NO_ERROR);
    I2C_MasterClearWriteBuf();

    do {
        temp = I2C_MasterReadBuf(sensor_comm.address, sensor_comm.buffer, num_bytes, I2C_MODE_REPEAT_START);
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
