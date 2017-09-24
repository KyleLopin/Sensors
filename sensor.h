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
*        Function Prototypes
***************************************/   

void sensor_write8(uint8 address, uint8* buffer);
void sensor_write16(uint8 address, uint8* buffer);
void sensor_write_n(uint8 address, uint8* buffer, uint8 num_bytes);
uint8 sensor_read8(uint8 address, uint8* buffer, uint8 _register);  
uint16 sensor_read16(uint8 address, uint8* buffer, uint8 _register);
void sensor_read_n(uint8 address, uint8* buffer, uint8 _register, uint8 num_bytes);

#endif

/* [] END OF FILE */
