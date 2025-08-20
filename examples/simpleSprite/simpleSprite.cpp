#include <Arduino.h>
#include "LGFX_Waveshare_ESP32S3_LCD_4.3.h"

// Function declarations
void createSprite();

// Global sprite object
LGFX_Sprite sprite(&lcd);

void setup() {
    Serial.begin(115200);
    delay(1000);
    
    // Initialize display
    if (!StartDisplay()) {
        Serial.println("Display initialization failed!");
        while(1) delay(1000);
    }
    
    // Clear screen
    lcd.fillScreen(TFT_BLACK);
    
    // Create and display sprite
    createSprite();
}

void loop() {
    // Nothing to do in loop
    delay(1000);
}

void createSprite() {
    
    sprite.createSprite(300, 200); // Create sprite with 300x200 pixels
    sprite.fillSprite(TFT_YELLOW); // Fill sprite with yellow background
    sprite.setTextColor(TFT_BLUE);  // Set text properties
    sprite.setTextSize(4);
    sprite.drawString("Hello World", 10, 10); // Draw "Hello World" text
    sprite.pushSprite(50, 50); // Push sprite to display at position (50, 50)
    
    Serial.println("Sprite created and displayed!");
}
