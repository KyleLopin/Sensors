/*******************************************************************************
 * File Name: sensor.h
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

#if !defined(_SENSOR_H)
#define _SENSOR_H
    
#include <project.h>
#include "stdbool.h"
    
    
/***************************************
*     Data Struct Definitions
***************************************/
    
typedef struct {
    uint8 address;
    uint8 buffer[10];
} SensorComm;
  
/***************************************
*        Function Prototypes
***************************************/   

void sensor_write8(SensorComm sensor_comm);
void sensor_write16(SensorComm sensor_comm);
void sensor_write_n(SensorComm sensor_comm, uint8 num_bytes);
uint8 sensor_read8(SensorComm _address, uint8 _register);  
uint16 sensor_read16(SensorComm _address, uint8 _register);
void sensor_read_n(SensorComm sensor_comm, uint8 _register, uint8 num_bytes)

#endif

/* [] END OF FILE */
