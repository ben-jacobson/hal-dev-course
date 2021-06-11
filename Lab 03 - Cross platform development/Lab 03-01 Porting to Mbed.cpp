#include <mbed.h>
#include "platform/Stream.h"

class LCD_Display: public Stream {
    public:
        static constexpr uint8_t _BUS_WIDTH = 8;

        LCD_Display(PinName register_sel_pin, PinName read_write_pin, PinName enable_pin, BusOut *ptr_to_data_bus);

    private: 
        // Device control pins
        DigitalOut _register_sel_pin, _read_write_pin, _enable_pin;
		BusOut *_ptr_to_data_bus; // requires us to define our bus out in our main code, then point to it. 

        // Device data bus pins
        static uint8_t _data_bus_pins[_BUS_WIDTH];     // declared static because other devices can share the same data bus, no need to have separate data busses for multiple devices. 

        enum LCD_Instructions: uint8_t {
            instr_clear_disp                    = 0b00000001, // 0x01 clears the display entirely. 
            instr_return_home                   = 0b00000010, // 0x02 returns the cursor to the home position
            instr_display_on_no_cursor_blink    = 0b00001100, // 0x0F Display ON, Cursor Off, Cursor Not Blinking.
            instr_entry_mode                    = 0b00000110, // 0x06 Entry Mode, Increment cursor position, No display shift. change to B00000100 to disable automatic cursor increment
            instr_fn_set                        = 0b00111000, // 0x38 Function set, 8 bit mode, 2 lines, 5×8 font.
        };

        void pulse_enable(void);        
        void send_command(uint8_t command);		

        // Stream implementation - provides printf() interface to write to display
        int _putc(int value);
        int _getc() { return -1; };
};

uint8_t LCD_Display::_data_bus_pins[LCD_Display::_BUS_WIDTH] = {0, 0, 0, 0, 0, 0, 0, 0}; // we are forced to initialize this static array, but can change it at any time during run time. 

LCD_Display::LCD_Display(PinName register_sel_pin, PinName read_write_pin, PinName enable_pin, BusOut *ptr_to_data_bus)
    :  _register_sel_pin(register_sel_pin), _read_write_pin(read_write_pin), _enable_pin(enable_pin), _ptr_to_data_bus(ptr_to_data_bus) 
{        
    // Pin modes no longer need to be initialized as this is covered by the DigitalOut constructor, same for data bus
    
    // Initialise the display
    _register_sel_pin = 0;  // Set RS to instruction register. LOW = Instruction register selected, HIGH = Data register selected
    _read_write_pin = 0;     // LOW = Write mode, HIGH = Read Mode       
    send_command(instr_fn_set);
    pulse_enable();

    send_command(instr_display_on_no_cursor_blink);
    pulse_enable();

    send_command(instr_entry_mode);
    pulse_enable();

    // Clear the display (This also resets cursor position)
    send_command(instr_clear_disp);
    pulse_enable();                     
    ThisThread::sleep_for(2ms);             // Sleep to allow the display to full initialize

    // now that device is set up, we can set our register select to data, just note that if you want to change config mid execution, you will have to pull it low again
    _register_sel_pin = 1;            // 0 = Instruction register selected, 1 = Data register selected
}

void LCD_Display::pulse_enable(void) {
    _enable_pin = 0;        
    ThisThread::sleep_for(1ms);
    _enable_pin = 1;
}

void LCD_Display::send_command(uint8_t command) {
    _ptr_to_data_bus->write(command);
}

int LCD_Display::_putc(int value) {     
    send_command(value); 
    pulse_enable();
    return 0;
}

int main() {
	BusOut DATA_BUS(D9, D8, D7, D6, D5, D4, D3, D2);
    LCD_Display my_lcd_screen(D12, D11, D10, &DATA_BUS);
    my_lcd_screen.printf("Hello World!!");

	while(true) {
		// put your main code here, to run repeatedly:
	}
}