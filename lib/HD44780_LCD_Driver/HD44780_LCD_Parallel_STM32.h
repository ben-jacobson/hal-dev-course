#ifndef HD44780_LCD_Parallel_STM32_h
#define HD44780_LCD_Parallel_STM32_h

#include "HD44780_LCD_Driver.h"

class HD44780_LCD_Parallel_STM32: public HD44780_LCD_Driver {
    public:
        HD44780_LCD_Parallel_STM32();
        void send_cmd(void);
};

#endif