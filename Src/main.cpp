#include <Arduino.h>

void setup() {
    // put your setup code here, to run once:
    Serial.begin(9600);
    Serial.println("Program Started");
}

void loop() {
    // put your main code here, to run repeatedly:
    Serial.println("Hello!");
    delay(1000);
}