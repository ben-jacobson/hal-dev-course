#include <Arduino.h>

class LCD_Display {
    public:
        LCD_Display();
        void print_text(const String text);     // these are called function prototypes

    private: 
        constexpr static byte BUS_WIDTH = 8;

        // Device control pins
        constexpr static byte REGISTER_SELECT_PIN = D12;
        constexpr static byte READ_WRITE_PIN = D11;
        constexpr static byte ENABLE_PIN = D10;

        // Device data bus pins
        byte DATA_BUS_PINS[BUS_WIDTH] = {D9, D8, D7, D6, D5, D4, D3, D2};

        // Device timings
        constexpr static byte CLEAR_DELAY_MS = 2; // clear display and return home instructions take 1.53ms to execute
        constexpr static byte INSTRUCTION_DELAY_MS = 1; // most instructions take 45us to execute, we'll force the MCU to wait

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

LCD_Display::LCD_Display() {        
    // Initialise the device control pins
    pinMode(REGISTER_SELECT_PIN, OUTPUT);
    pinMode(READ_WRITE_PIN, OUTPUT);
    pinMode(ENABLE_PIN, OUTPUT);

    // Initialise the data bus pins, setting the pin modes to output and setting the pins LOW by default
    for (byte i = 0; i < BUS_WIDTH; i++) {
        pinMode(DATA_BUS_PINS[i], OUTPUT);
        digitalWrite(DATA_BUS_PINS[i], LOW); 
    }

    // Initialise the display
    digitalWrite(REGISTER_SELECT_PIN, LOW);         // Set RS to instruction register. LOW = Instruction register selected, HIGH = Data register selected
    digitalWrite(READ_WRITE_PIN, LOW);              // LOW = Write mode, HIGH = Read Mode
    send_command(instr_fn_set);
    pulse_enable();

    send_command(instr_display_on_no_cursor_blink);
    pulse_enable();

    send_command(instr_entry_mode);
    pulse_enable();

    // Clear the display (This also resets cursor position)
    send_command(instr_clear_disp);
    pulse_enable();
    delay(CLEAR_DELAY_MS);                         // Sleep to allow the display to full initialize
}

void LCD_Display::print_text(const String text) {
    digitalWrite(REGISTER_SELECT_PIN, HIGH);            // LOW = Instruction register selected, HIGH = Data register selected

    for (byte i = 0; i < text.length(); i++) {
        send_command(text[i]);
        pulse_enable();
    }
}    

void LCD_Display::pulse_enable(void) {
    digitalWrite(ENABLE_PIN, LOW);        
    delay(INSTRUCTION_DELAY_MS);
    digitalWrite(ENABLE_PIN, HIGH);
}

void LCD_Display::send_command(byte command) {
    for (byte i = 0; i < BUS_WIDTH; i++) {
        digitalWrite(DATA_BUS_PINS[i], (command >> i) & 1);
    }
}

void setup() {
    LCD_Display my_lcd_screen;
    my_lcd_screen.print_text("Hello World!!");
}

void loop() {
    // Do nothing for now
}