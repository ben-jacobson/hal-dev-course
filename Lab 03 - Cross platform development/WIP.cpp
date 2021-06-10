#include <mbed.h>
#include "platform/Stream.h"

constexpr uint8_t BUS_WIDTH = 8;

class LCD_Display: public Stream {
    public:
        LCD_Display(PinName register_sel_pin, PinName read_write_pin, PinName enable_pin, BusOut *ptr_to_data_bus);

    private: 
        // Device control pins
        DigitalOut _register_sel_pin, _read_write_pin, _enable_pin;
		BusOut *_ptr_to_data_bus; // requires us to define our bus out in our main code, then point to it. 

        // Device data bus pins
        static uint8_t _data_bus_pins[BUS_WIDTH];     // declared static because other devices can share the same data bus, no need to have separate data busses for multiple devices. 

        // Device timings
        constexpr static uint8_t _CLEAR_DELAY_MS = 2; // clear display and return home instructions take 1.53ms to execute
        constexpr static uint8_t _INSTRUCTION_DELAY_MS = 1; // most instructions take 45us to execute, we'll force the MCU to wait

        enum LCD_Instructions: uint8_t {
            instr_clear_disp                    = 0b00000001, // 0x01 clears the display entirely. 
            instr_return_home                   = 0b00000010, // 0x02 returns the cursor to the home position
            instr_display_on_no_cursor_blink    = 0b00001100, // 0x0F Display ON, Cursor Off, Cursor Not Blinking.
            instr_entry_mode                    = 0b00000110, // 0x06 Entry Mode, Increment cursor position, No display shift. change to B00000100 to disable automatic cursor increment
            instr_fn_set                        = 0b00111000, // 0x38 Function set, 8 bit mode, 2 lines, 5×8 font.
        };

        void pulse_enable(void);        
        void send_command(uint8_t command);		
		inline int _putc(int value) { send_command((uint8_t)value); };
};

uint8_t LCD_Display::_data_bus_pins[BUS_WIDTH] = {0, 0, 0, 0, 0, 0, 0, 0}; // we are forced to initialize this static array, but can change it at any time during run time. 

LCD_Display::LCD_Display(PinName register_sel_pin, PinName read_write_pin, PinName enable_pin, BusOut *ptr_to_data_bus)
    :  _register_sel_pin(register_sel_pin), _read_write_pin(read_write_pin), _enable_pin(enable_pin), _ptr_to_data_bus(ptr_to_data_bus) 
{        
    // Pin modes no longer need to be initialized as this is covered by the DigitalOut constructor, same for data bus

    // Initialise the display
    digitalWrite(_REGISTER_SELECT_PIN, LOW);         // Set RS to instruction register. LOW = Instruction register selected, HIGH = Data register selected
    digitalWrite(_READ_WRITE_PIN, LOW);              // LOW = Write mode, HIGH = Read Mode
    send_command(instr_fn_set);
    pulse_enable();

    send_command(instr_display_on_no_cursor_blink);
    pulse_enable();

    send_command(instr_entry_mode);
    pulse_enable();

    // Clear the display (This also resets cursor position)
    send_command(instr_clear_disp);
    pulse_enable();
    delay(_CLEAR_DELAY_MS);                         // Sleep to allow the display to full initialize
}



void LCD_Display::pulse_enable(void) {
    digitalWrite(_ENABLE_PIN, LOW);        
    delay(_INSTRUCTION_DELAY_MS);
    digitalWrite(_ENABLE_PIN, HIGH);
}

void LCD_Display::send_command(uint8_t command) {
    _register_sel_pin = 1;            // 0 = Instruction register selected, 1 = Data register selected
	&_ptr_to_data_bus.write(command);
}

int main() {
    constexpr uint8_t REGISTER_SELECT_PIN = D12;
    constexpr uint8_t READ_WRITE_PIN = D11;
    constexpr uint8_t ENABLE_PIN = D10;
    //constexpr uint8_t DATA_BUS[8] = {D9, D8, D7, D6, D5, D4, D3, D2};
	BusOut DATA_BUS(D9, D8, D7, D6, D5, D4, D3, D2);

    LCD_Display my_lcd_screen(REGISTER_SELECT_PIN, READ_WRITE_PIN, ENABLE_PIN, &DATA_BUS);
    my_lcd_screen.prinft("Hello World!!");

	while(1) {
		// put your main code here, to run repeatedly:
	}
}