#include "HD44780_LCD_Parallel_STM32.h"

HD44780_LCD_Parallel_STM32::HD44780_LCD_Parallel_STM32( GPIO_TypeDef* register_select_port, uint16_t register_select_pin, 
                                                        GPIO_TypeDef* read_write_port,      uint16_t read_write_pin, 
                                                        GPIO_TypeDef* enable_port,          uint16_t enable_pin,
                                                        GPIO_TypeDef* data_bus_port)
:   HD44780_LCD_Driver(), // we want to call the parent class constructor for this as it will initialize all of our variables for us
    _register_select_port(register_select_port), _read_write_port(read_write_port), _enable_port(enable_port),      // member initializer list, setting the const values for the various pins and ports used to manipulate the device
    _register_select_pin(register_select_pin), _read_write_pin(read_write_pin), _enable_pin(enable_pin),
    _data_bus_port(data_bus_port)
{
    // don't think there's anything else to do after that?!
}

void HD44780_LCD_Parallel_STM32::send_cmd(void) {
    _data_bus_port->ODR = 0xFFFF;   
    _data_bus_port->ODR = 0x0000;   
    _data_bus_port->ODR = 0xAAAA;   
}

void HD44780_LCD_Parallel_STM32::select_instruction_register(void) {
    HAL_GPIO_WritePin(_register_select_port, _register_select_pin, (GPIO_PinState)INSTRUCTION_REGISTER); 
}

void HD44780_LCD_Parallel_STM32::select_data_register(void) {
    HAL_GPIO_WritePin(_register_select_port, _register_select_pin, (GPIO_PinState)DATA_REGISTER); 
}

void HD44780_LCD_Parallel_STM32::pulse_enable(void) {
    // pulse the enable pin high, the low and delay according to the datasheet
    HAL_GPIO_WritePin(_enable_port, _enable_pin, GPIO_PIN_SET); 
    HAL_Delay(INSTRUCTION_DELAY_MS);
    HAL_GPIO_WritePin(_enable_port, _enable_pin, GPIO_PIN_RESET); 
    // we can leave the device in the low state to keep it enabled.
}