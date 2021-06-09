#include <Arduino.h>

constexpr byte BUS_WIDTH = 8;

class LCD_Display {
    public:
        LCD_Display(byte register_select_pin, byte read_write_pin, byte enable_pin, const byte data_bus_pins[]); //, byte data_bus_pins[BUS_WIDTH]);
        void print_text(const String text);     // these are called function prototypes

    private: 
        // Device control pins
        const byte _REGISTER_SELECT_PIN;
        const byte _READ_WRITE_PIN;
        const byte _ENABLE_PIN;

        // Device data bus pins
        static byte _data_bus_pins[BUS_WIDTH];     // declared static because other devices can share the same data bus, no need to have separate data busses for multiple devices. 

        // Device timings
        constexpr static byte _CLEAR_DELAY_MS = 2; // clear display and return home instructions take 1.53ms to execute
        constexpr static byte _INSTRUCTION_DELAY_MS = 1; // most instructions take 45us to execute, we'll force the MCU to wait

        enum LCD_Instructions: byte {
            instr_clear_disp                    = 0b00000001, // 0x01 clears the display entirely. 
            instr_return_home                   = 0b00000010, // 0x02 returns the cursor to the home position
            instr_display_on_no_cursor_blink    = 0b00001100, // 0x0F Display ON, Cursor Off, Cursor Not Blinking.
            instr_entry_mode                    = 0b00000110, // 0x06 Entry Mode, Increment cursor position, No display shift. change to B00000100 to disable automatic cursor increment
            instr_fn_set                        = 0b00111000, // 0x38 Function set, 8 bit mode, 2 lines, 5×8 font.
        };

        void pulse_enable(void);        
        void send_command(byte command);
};

byte LCD_Display::_data_bus_pins[BUS_WIDTH] = {0, 0, 0, 0, 0, 0, 0, 0}; // we are forced to initialize this static array, but can change it at any time during run time. 

LCD_Display::LCD_Display(byte register_select_pin, byte read_write_pin, byte enable_pin, const byte data_bus_pins[])
    :  _REGISTER_SELECT_PIN(register_select_pin), _READ_WRITE_PIN(read_write_pin), _ENABLE_PIN(enable_pin)
{        
    // Initialise the device control pins
    pinMode(_REGISTER_SELECT_PIN, OUTPUT);
    pinMode(_READ_WRITE_PIN, OUTPUT);
    pinMode(_ENABLE_PIN, OUTPUT);

    // Initialise the data bus pins, setting the pin modes to output and setting the pins LOW by default
    for (byte i = 0; i < BUS_WIDTH; i++) {
        LCD_Display::_data_bus_pins[i] = data_bus_pins[i];
        pinMode(_data_bus_pins[i], OUTPUT);
        digitalWrite(_data_bus_pins[i], LOW); 
    }

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

void LCD_Display::print_text(const String text) {
    digitalWrite(_REGISTER_SELECT_PIN, HIGH);            // LOW = Instruction register selected, HIGH = Data register selected

    for (byte i = 0; i < text.length(); i++) {
        send_command(text[i]);
        pulse_enable();
    }
}    

void LCD_Display::pulse_enable(void) {
    digitalWrite(_ENABLE_PIN, LOW);        
    delay(_INSTRUCTION_DELAY_MS);
    digitalWrite(_ENABLE_PIN, HIGH);
}

void LCD_Display::send_command(byte command) {
    for (byte i = 0; i < BUS_WIDTH; i++) {
        digitalWrite(LCD_Display::_data_bus_pins[i], (command >> i) & 1);
    }
}

void setup() {
    constexpr byte REGISTER_SELECT_PIN = D12;
    constexpr byte READ_WRITE_PIN = D11;
    constexpr byte ENABLE_PIN = D10;
    constexpr byte DATA_BUS[8] = {D9, D8, D7, D6, D5, D4, D3, D2};

    LCD_Display my_lcd_screen(REGISTER_SELECT_PIN, READ_WRITE_PIN, ENABLE_PIN, DATA_BUS);
    my_lcd_screen.print_text("Hello World!!");
}

void loop() {
    // Do nothing for now
}