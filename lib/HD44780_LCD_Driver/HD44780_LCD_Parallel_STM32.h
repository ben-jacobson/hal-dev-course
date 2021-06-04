#ifndef HD44780_LCD_Parallel_STM32_h
#define HD44780_LCD_Parallel_STM32_h

// Note, these next includes must be changed over for whatever platform you are using, e.g STM32F411xx.h for the F411
#include <stm32f303x8.h>        
#include <stm32f3xx_hal_gpio.h>

#include "HD44780_LCD_Driver.h"

/*
    Bare metal STM32 implementation of the LCD display
    Uses a parallel bus. 
    Could easily be adapted for I2C or SPI
*/
class HD44780_LCD_Parallel_STM32: public HD44780_LCD_Driver {
    public:
        HD44780_LCD_Parallel_STM32( GPIO_TypeDef* register_select_port, uint16_t register_select_pin, 
                                    GPIO_TypeDef* read_write_port,      uint16_t read_write_pin, 
                                    GPIO_TypeDef* enable_port,          uint16_t enable_pin,
                                    GPIO_TypeDef* data_bus_port);
        void send_cmd(void);    // overwriting the send_cmd with our own implementation

    private:  
        GPIO_TypeDef *_register_select_port, *_read_write_port, *_enable_port; // it seems like these should be const, but by using the HAL_GPIO_WRitePin, we would need to cast away the constness and modify the data. 
        uint16_t _register_select_pin, _read_write_pin, _enable_pin;
        GPIO_TypeDef *_data_bus_port;

        void select_instruction_register(void); 
        void select_data_register(void);   
        void pulse_enable(void);       
};

#endif