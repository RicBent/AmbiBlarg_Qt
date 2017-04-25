#include "FastLED.h"

#define DATA_PIN 6
#define MAX_LEDS 250

#define SERIAL_RATE 115200

#define u16 unsigned short

// We waste memory here, but we don't need it for anything else
CRGB leds[MAX_LEDS];

// Magic word used for pairing
char magic[] = { 'A', 'B', 'l', 'a', 'r', 'g', '\n' };


// Functions
void waitForMagicWord(char* magic);
void clearStrip(bool updateStrip = false);


void setup()
{
    FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, MAX_LEDS);
    
    Serial.begin(SERIAL_RATE);
    
    // Send magic word
    for (int i = 0; i < sizeof(magic); i++)
        Serial.write(magic[i]);
        
    clearStrip(true);
}

void loop()
{
    waitForMagicWord(magic);
    
    // Get LED Count
    while(Serial.available() < 2) {}
    u16 ledCount = Serial.read();
    ledCount |= (Serial.read() << 8);
    
    // Clear all LEDs
    clearStrip();
    
    // Get and set LED colors    
    for (u16 i = 0; i < ledCount; i++)
    {        
        while(Serial.available() < 3) {}

        leds[i].r = Serial.read();
        leds[i].g = Serial.read();
        leds[i].b = Serial.read();
    }
    
    // Update LEDs
    FastLED.show();
}


void waitForMagicWord(char* magic)
{ 
    for (int i = 0;; i++)
    {        
        if (magic[i] == '\n')
            return;
        
        waitLoop:
        while (!Serial.available()) {}
        
        if (magic[i] == Serial.read())
            continue;
        
        i = 0;
        goto waitLoop;
    }
}

void clearStrip(bool updateStrip)
{
    memset(leds, 0, MAX_LEDS * sizeof(CRGB));
    
    if (updateStrip)
        FastLED.show();
}

