#ifndef HD44780_LCD_Driver_h
#define HD44780_LCD_Driver_h

/*
    Interface class, containing cross platform C++ code. Business logic is completely separate from implementation. 

    Instantiating this object by itself will cause a compliation error, you must create your own child class and inherit from this, 
    then overwrite the send_cmd function with your own implementation based on your chosen platform HAL or RTOS

    How to use:
        1. Create a child class
        2. Write your own implementation of send_cmd to overwrite ours
*/
class HD44780_LCD_Driver {
    public:
        HD44780_LCD_Driver();
        virtual void send_cmd() = 0; // this is now an abstract class
};

#endif