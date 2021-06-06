#include <Arduino.h>

// Device control pins
constexpr byte REGISTER_SELECT_PIN = D12;
constexpr byte READ_WRITE_PIN = D11;
constexpr byte ENABLE_PIN = D10;

// Device data bus pins
constexpr byte DATA_BUS_PINS[8] = {D9, D8, D7, D6, D5, D4, D3, D2};

// Device timings
constexpr byte CLEAR_DELAY_MS = 2; // clear display and return home instructions take 1.53ms to execute
constexpr byte INSTRUCTION_DELAY_MS = 1; // most instructions take 45us to execute, we'll force the MCU to wait

void setup() {

    // Initialise the device control pins
    pinMode(REGISTER_SELECT_PIN, OUTPUT);
    pinMode(READ_WRITE_PIN, OUTPUT);
    pinMode(ENABLE_PIN, OUTPUT);

    // Initialise the data bus, setting the pin modes to output and setting the pins LOW by default
    for (byte i = 0; i < 8; i++) {
        pinMode(DATA_BUS_PINS[i], OUTPUT);
        digitalWrite(DATA_BUS_PINS[i], LOW); 
    }

    // Initialise the display
    digitalWrite(REGISTER_SELECT_PIN, LOW);         // Set RS to instruction register. LOW = Instruction register selected, HIGH = Data register selected
    digitalWrite(READ_WRITE_PIN, LOW);              // LOW = Write mode, HIGH = Read Mode
 
    digitalWrite(DATA_BUS_PINS[0], LOW);            // 0, send out the function set command: 0b00111000 on data bus
    digitalWrite(DATA_BUS_PINS[1], LOW);            // 0
    digitalWrite(DATA_BUS_PINS[2], LOW);            // 0
    digitalWrite(DATA_BUS_PINS[3], HIGH);           // 1
    digitalWrite(DATA_BUS_PINS[4], HIGH);           // 1
    digitalWrite(DATA_BUS_PINS[5], HIGH);           // 1
    digitalWrite(DATA_BUS_PINS[6], LOW);            // 0        
    digitalWrite(DATA_BUS_PINS[7], LOW);            // 0            
    digitalWrite(ENABLE_PIN, LOW);                  // Then pulse enable
    delay(INSTRUCTION_DELAY_MS);
    digitalWrite(ENABLE_PIN, HIGH);

    digitalWrite(DATA_BUS_PINS[0], LOW);            // 0, send out the display on, no cursor blink command: 0b00001100 on data bus
    digitalWrite(DATA_BUS_PINS[1], LOW);            // 0
    digitalWrite(DATA_BUS_PINS[2], HIGH);           // 1
    digitalWrite(DATA_BUS_PINS[3], HIGH);           // 1
    digitalWrite(DATA_BUS_PINS[4], LOW);            // 0
    digitalWrite(DATA_BUS_PINS[5], LOW);            // 0
    digitalWrite(DATA_BUS_PINS[6], LOW);            // 0        
    digitalWrite(DATA_BUS_PINS[7], LOW);            // 0          
    digitalWrite(ENABLE_PIN, LOW);                  // Then pulse enable
    delay(INSTRUCTION_DELAY_MS);
    digitalWrite(ENABLE_PIN, HIGH); 

    digitalWrite(DATA_BUS_PINS[0], LOW);            // 0, set cursor move direction with the entry mode command: 0b00000110 on data bus
    digitalWrite(DATA_BUS_PINS[1], HIGH);           // 1
    digitalWrite(DATA_BUS_PINS[2], HIGH);           // 1
    digitalWrite(DATA_BUS_PINS[3], LOW);            // 0
    digitalWrite(DATA_BUS_PINS[4], LOW);            // 0
    digitalWrite(DATA_BUS_PINS[5], LOW);            // 0
    digitalWrite(DATA_BUS_PINS[6], LOW);            // 0        
    digitalWrite(DATA_BUS_PINS[7], LOW);            // 0     
    digitalWrite(ENABLE_PIN, LOW);                  // Then pulse enable
    delay(INSTRUCTION_DELAY_MS);
    digitalWrite(ENABLE_PIN, HIGH);   

    // Clear the display (This also resets cursor position)
    digitalWrite(DATA_BUS_PINS[0], HIGH);           // 1, send out the entry mode command: 0b00000001 on data bus
    digitalWrite(DATA_BUS_PINS[1], LOW);            // 0
    digitalWrite(DATA_BUS_PINS[2], LOW);            // 0
    digitalWrite(DATA_BUS_PINS[3], LOW);            // 0
    digitalWrite(DATA_BUS_PINS[4], LOW);            // 0
    digitalWrite(DATA_BUS_PINS[5], LOW);            // 0
    digitalWrite(DATA_BUS_PINS[6], LOW);            // 0        
    digitalWrite(DATA_BUS_PINS[7], LOW);            // 0     
    digitalWrite(ENABLE_PIN, LOW);                  // Then pulse enable
    delay(INSTRUCTION_DELAY_MS);
    digitalWrite(ENABLE_PIN, HIGH);   
    delay(CLEAR_DELAY_MS);                         // Sleep to allow the display to full initialize

    // Print Hello world to the display    
    digitalWrite(REGISTER_SELECT_PIN, HIGH);         // LOW = Instruction register selected, HIGH = Data register selected

    digitalWrite(DATA_BUS_PINS[0], LOW);            // 0, Print H: 0b01001000 on data bus
    digitalWrite(DATA_BUS_PINS[1], LOW);            // 0
    digitalWrite(DATA_BUS_PINS[2], LOW);            // 0
    digitalWrite(DATA_BUS_PINS[3], HIGH);           // 1
    digitalWrite(DATA_BUS_PINS[4], LOW);            // 0
    digitalWrite(DATA_BUS_PINS[5], LOW);            // 0
    digitalWrite(DATA_BUS_PINS[6], HIGH);           // 1        
    digitalWrite(DATA_BUS_PINS[7], LOW);            // 0     
    digitalWrite(ENABLE_PIN, LOW);                  // Then pulse enable
    delay(INSTRUCTION_DELAY_MS);
    digitalWrite(ENABLE_PIN, HIGH);           

    digitalWrite(DATA_BUS_PINS[0], HIGH);               // 1, Print e: 0b01100101 on data bus
    digitalWrite(DATA_BUS_PINS[1], LOW);                // 0
    digitalWrite(DATA_BUS_PINS[2], HIGH);               // 1
    digitalWrite(DATA_BUS_PINS[3], LOW);                // 0
    digitalWrite(DATA_BUS_PINS[4], LOW);                // 0
    digitalWrite(DATA_BUS_PINS[5], HIGH);               // 1
    digitalWrite(DATA_BUS_PINS[6], HIGH);               // 1        
    digitalWrite(DATA_BUS_PINS[7], LOW);                // 0     
    digitalWrite(ENABLE_PIN, LOW);                      // Then pulse enable
    delay(INSTRUCTION_DELAY_MS);
    digitalWrite(ENABLE_PIN, HIGH);      

    digitalWrite(DATA_BUS_PINS[0], LOW);                // 0, Print l: 0b01101100 on data bus
    digitalWrite(DATA_BUS_PINS[1], LOW);                // 0
    digitalWrite(DATA_BUS_PINS[2], HIGH);               // 1
    digitalWrite(DATA_BUS_PINS[3], HIGH);               // 1
    digitalWrite(DATA_BUS_PINS[4], LOW);                // 0
    digitalWrite(DATA_BUS_PINS[5], HIGH);               // 1
    digitalWrite(DATA_BUS_PINS[6], HIGH);               // 1        
    digitalWrite(DATA_BUS_PINS[7], LOW);                // 0     
    digitalWrite(ENABLE_PIN, LOW);                      // Then pulse enable
    delay(INSTRUCTION_DELAY_MS);
    digitalWrite(ENABLE_PIN, HIGH);        

    digitalWrite(DATA_BUS_PINS[0], LOW);                // 0, Print l: 0b01101100 on data bus
    digitalWrite(DATA_BUS_PINS[1], LOW);                // 0
    digitalWrite(DATA_BUS_PINS[2], HIGH);               // 1
    digitalWrite(DATA_BUS_PINS[3], HIGH);               // 1
    digitalWrite(DATA_BUS_PINS[4], LOW);                // 0
    digitalWrite(DATA_BUS_PINS[5], HIGH);               // 1
    digitalWrite(DATA_BUS_PINS[6], HIGH);               // 1        
    digitalWrite(DATA_BUS_PINS[7], LOW);                // 0     
    digitalWrite(ENABLE_PIN, LOW);                      // Then pulse enable
    delay(INSTRUCTION_DELAY_MS);
    digitalWrite(ENABLE_PIN, HIGH);            

    digitalWrite(DATA_BUS_PINS[0], HIGH);               // 1, Print o: 0b01101111 on data bus
    digitalWrite(DATA_BUS_PINS[1], HIGH);               // 1
    digitalWrite(DATA_BUS_PINS[2], HIGH);               // 1
    digitalWrite(DATA_BUS_PINS[3], HIGH);               // 1
    digitalWrite(DATA_BUS_PINS[4], LOW);                // 0
    digitalWrite(DATA_BUS_PINS[5], HIGH);               // 1
    digitalWrite(DATA_BUS_PINS[6], HIGH);               // 1        
    digitalWrite(DATA_BUS_PINS[7], LOW) ;               // 0     
    digitalWrite(ENABLE_PIN, LOW);                      // Then pulse enable
    delay(INSTRUCTION_DELAY_MS);
    digitalWrite(ENABLE_PIN, HIGH);                

    // you get the point from here!

}

void loop() {
    // Do nothing for now
}