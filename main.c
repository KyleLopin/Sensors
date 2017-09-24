/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/
#include "project.h"
#include "stdio.h"

// local files
#include "isl29125.h"

uint8 count = 0;
char LCD_str[40];

uint8 red;
uint8 green;
uint8 blue;

int main(void)
{
    CyGlobalIntEnable; /* Enable global interrupts. */

    LCD_Start();
    LCD_ClearDisplay();
    I2C_Start();
    LCD_Position(0, 0);
    LCD_PrintString("Sensor");
    
    isl29125_init();

    for(;;) {
        CyDelay(1000);
        LCD_ClearDisplay();
        uint8 ID = isl29125_read_id();
        LCD_Position(0, 0);
        sprintf(LCD_str, "id: 0x%02X", ID);
        LCD_PrintString(LCD_str);
        
        red = isl29125_read_red();
        green = isl29125_read_green();
        blue = isl29125_read_blue();
        LCD_Position(1,0);
        sprintf(LCD_str, "r:%i,g:%i,b:%i ", red, green, blue);
        LCD_PrintString(LCD_str); 
    }
}

/* [] END OF FILE */
