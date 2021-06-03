#ifndef HD44780_LCD_Driver_h
#define HD44780_LCD_Driver_h

#include <stdint.h>

/*
    Interface class, containing cross platform C++ code. Business logic is completely separate from implementation. 

    Instantiating this object by itself will cause a compliation error, you must create your own child class and inherit from this, 
    then overwrite the send_cmd function with your own implementation based on your chosen platform HAL or RTOS

    How to use:
        1. Create a child class
        2. Write your own implementation to overwrite all virtual functions, such as send_cmd()
*/
class HD44780_LCD_Driver {
    public:
        HD44780_LCD_Driver();
        virtual void send_cmd() = 0; // this is now an abstract class
        void init_display(void);
        void new_line(void);
        void set_cursor_position(void);
        
    private:
        static constexpr uint8_t DISPLAY_WIDTH = 16;        // we do not want these member variables consuming any RAM. Using constexpr will force the compiler to sub in the values needed at compile time
        static constexpr uint8_t DISPLAY_HEIGHT = 2;

        static constexpr uint8_t INSTRUCTION_REGISTER = 0;
        static constexpr uint8_t DATA_REGISTER = 1;                  

        enum LCD_Instructions: uint8_t {
            instr_clear_disp                    = 0b00000001,       // 0x01 clears the display entirely. 
            instr_return_home                   = 0b00000010,       // 0x02 returns the cursor to the home position
            instr_display_on                    = 0b00001111,       // 0x0F Display ON, Cursor On, Cursor Blinking.
            instr_display_on_no_cursor_blink    = 0b00001100,       // 0x0F Display ON, Cursor Off, Cursor Not Blinking.
            instr_entry_mode                    = 0b00000110,       // 0x06 Entry Mode, Increment cursor position, No display shift. change to B00000100 to disable automatic cursor increment
            instr_fn_set                        = 0b00111000,       // 0x38 Function set, 8 bit mode, 2 lines, 5×8 font.
            instr_move_cursor_left              = 0b00010000,       // 0x10 move cursor left
            instr_move_cursor_right             = 0b00010100        // 0x14 move cursor right        
        };
        uint8_t cursor_x, cursor_y;
        char screen_buffer[DISPLAY_WIDTH * DISPLAY_HEIGHT];

        virtual void select_instruction_register(void); 
        virtual void select_data_register(void);   
        virtual void pulse_enable(void);        
};

#endif


