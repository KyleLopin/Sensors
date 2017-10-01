/*******************************************************************************
 * File Name: tsl2591.c
 * Version 0.50
 *
 * Description:
 *  This file provides constants and parameter values for the isl29125 light sensor.
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

#if !defined(_TSL2561_H)
#define _TSL2561_H
    
#include <project.h>
#include "sensor.h"

/***************************************
*      Structures
***************************************/ 

typedef struct {
    uint8 working;
    uint8 integration_time;
    uint8 _gain;
} TSL2561;

TSL2561 tsl2561;

/***************************************
*    ISL29125 Device Constants
***************************************/ 

#define I2C_ADDRESS_GROUND          0X29
#define I2C_ADDRESS_FLOAT           0X39
#define I2C_ADDRESS_VDD             0X49

#define TSL

/***************************************
*    ISL29125 Register Assignment
***************************************/ 

#define TSL2561_REG_CONTROL         0x00
#define TSL2561_REG_TIMING          0x01
#define TSL2561_REG_THRESHOLD_LL    0x02
#define TSL2561_REG_THRESHOLD_LH    0x03
#define TSL2561_REG_THRESHOLD_HL    0x04
#define TSL2561_REG_THRESHOLD_HH    0x05
#define TSL2561_REG_INTERRUPT       0x06
#define TSL2561_REG_ID              0x0A
#define TSL2561_REG_DATA0_LOW       0x0C
#define TSL2561_REG_DATA0_HIGH      0x0D
#define TSL2561_REG_DATA1_LOW       0x0E
#define TSL2561_REG_DATA1_HIGH      0x0F

/***************************************
*        Function Prototypes
***************************************/     

void tsl2561_Init(void); 
void tsl2561_Start(void);

uint8 tsl2561_read_id(void);



    
#endif