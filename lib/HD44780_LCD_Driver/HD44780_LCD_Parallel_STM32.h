#ifndef HD44780_LCD_Parallel_STM32_h
#define HD44780_LCD_Parallel_STM32_h

#include "HD44780_LCD_Driver.h"

/*
    Bare metal STM32 implementation of the LCD display
    Uses a parallel bus. 
    Could easily be adapted for I2C or SPI
*/
class HD44780_LCD_Parallel_STM32: public HD44780_LCD_Driver {
    public:
        HD44780_LCD_Parallel_STM32();
        void send_cmd(void);    // overwriting the send_cmd with our own implementation
};

#endif