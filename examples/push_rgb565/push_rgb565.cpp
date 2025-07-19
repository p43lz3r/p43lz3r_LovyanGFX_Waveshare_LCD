#include <Arduino.h>
#include "LGFX_Waveshare_ESP32S3_LCD_4.3B.h"
//#include "LGFX_Waveshare_ESP32S3_LCD_7.h"
#include "images.h"

// Function declarations
void displaySprite();

uint16_t rgb565(uint8_t r, uint8_t g, uint8_t b);
uint16_t rgb565_lovyan(uint8_t r, uint8_t g, uint8_t b);

// Global variables
LGFX_Sprite sprite(&lcd);  // Create sprite linked to main lcd

void setup() {
    Serial.begin(115200);
    delay(1000);
    
    // Initialize display using your hardware configuration
    if (!initDisplay()) {
        Serial.println("Display initialization failed!");
        while(1) delay(1000);
    }
    
    // Clear screen to black
    lcd.fillScreen(TFT_BLACK);

    // Display the sprite with your image
    displaySprite();
    
    Serial.println("Setup complete!");
}

void loop() {
    // Nothing to do in loop for this example
    delay(1000);
}


// Display the sprite at coordinates (50, 50)
void displaySprite() {
    Serial.println("Displaying sprite...");
    
    // Create sprite with 100x100 dimensions
    //sprite.createSprite(400, 400);
    
    // Method 1: Push your PROGMEM image array to sprite
    //sprite.pushImage(0, 0, 400, 400, mrL_img);
    
    // Push sprite to LCD at position (50, 50)
    //sprite.pushSprite(10, 10);
    
    // Alternative Method 2: You can also draw directly to LCD
    lcd.pushImage(5, 5, 400, 400, mrL_img);

    Serial.println("Sprite displayed at (50, 50)");
    
    // Add some text to verify display is working
    //lcd.setTextColor(TFT_WHITE, TFT_BLACK);
    //lcd.setTextSize(2);
    //lcd.setCursor(200, 50);
    //lcd.print("100x100 Sprite");
    //lcd.setCursor(200, 80);
    //lcd.print("at (50,50)");
    
    // Display image info
    //lcd.setCursor(200, 120);
    //lcd.print("Your mrL_img");
    //lcd.setCursor(200, 150);
    //lcd.print("from PROGMEM");
}
