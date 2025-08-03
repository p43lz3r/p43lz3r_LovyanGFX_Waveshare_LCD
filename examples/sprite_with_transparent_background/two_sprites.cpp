#include <Arduino.h>
#include "LGFX_Waveshare_ESP32S3_LCD_4.3B.h"
#include "images.h"
#include <OpenFontRender.h>
#include "myFonts.h"

// Function declarations
void displayBackground();
void displayTemperatureSprite(float temperature);
void displayTemperatureNerdMinerStyle(float temperature);
void displayTemperatureAt(float temperature, int x, int y);
uint16_t rgb565(uint8_t r, uint8_t g, uint8_t b);
uint16_t rgb565_lovyan(uint8_t r, uint8_t g, uint8_t b);

// Global variables
LGFX_Sprite tempSprite(&lcd); // Sprite for temperature display
OpenFontRender render;        // Font renderer

//void displayTwoTemperatureGauges(float temp1, float temp2);
void displayTemperatureNerdMinerStyle(float temperature, int x, int y, const char* label, uint16_t color);

void setup()
{
    Serial.begin(115200);
    delay(1000);

    // Initialize display using your hardware configuration
    if (!initDisplay())
    {
        Serial.println("Display initialization failed!");
        while (1)
            delay(1000);
    }

    // Clear screen to black
    lcd.fillScreen(TFT_BLACK);

    // Initialize OpenFontRender
    // They use DigitalNumbers font for numeric displays
    if (render.loadFont(DigitalNumbers, sizeof(DigitalNumbers)))
    {
        Serial.println("Error loading DigitalNumbers font!");
        // Try alternative font if DigitalNumbers fails
        if (render.loadFont(NotoSans_Bold, sizeof(NotoSans_Bold)))
        {
            Serial.println("Error loading NotoSans_Bold font too!");
            while (1)
                delay(1000); // Stop if both fonts fail
        }
        else
        {
            Serial.println("NotoSans_Bold font loaded successfully!");
        }
    }
    else
    {
        Serial.println("DigitalNumbers font loaded successfully!");
    }

    // Set up render properties
    render.setLineSpaceRatio(0.9);

    // Create temperature sprite
    tempSprite.createSprite(180, 70);

    // Display initial setup
    displayBackground();
    // displayTemperatureSprite(23.5);

    Serial.println("Setup complete!");
}

void loop()
{
    // Simulate temperature readings
    static float temperature = 20.0;
    static unsigned long lastUpdate = 0;
    static bool increasing = true;

    if (millis() - lastUpdate > 3000)
    {
        if (increasing)
        {
            temperature += random(5, 15) / 10.0;
            if (temperature > 35.0)
                increasing = false;
        }
        else
        {
            temperature -= random(5, 15) / 10.0;
            if (temperature < 15.0)
                increasing = true;
        }

        // displayBackground();
        //displayTemperatureNerdMinerStyle(temperature); // Use sci-fi style matching background
        displayTemperatureNerdMinerStyle(temperature, 100, 200, "CURRENT", TFT_CYAN);
        displayTemperatureNerdMinerStyle(temperature + 10, 350, 200, "MAXIMUM", TFT_YELLOW);

        
        lastUpdate = millis();
        Serial.printf("Temperature updated: %.1f°C\n", temperature);
    }

    delay(100);
}

// Display the background image
void displayBackground()
{
    lcd.pushImage(0, 0, 750, 430, background_img); // Updated for 750x430 background
}

// Display temperature
void displayTemperatureSprite(float temperature)
{
    Serial.printf("Displaying temperature: %.1f°C with NerdMiner font\n", temperature);

    char tempStr[12];
    snprintf(tempStr, sizeof(tempStr), "%.1f°C", temperature);

    // Clear sprite
    tempSprite.fillSprite(TFT_BLACK);

    // Link sprite to renderer
    render.setDrawer(tempSprite);

    // Use DigitalNumbers font for temperature
    render.setFontSize(32);
    render.setFontColor(TFT_WHITE, TFT_BLACK);

    // Use rdrawString method for right-aligned text
    render.rdrawString(tempStr, tempSprite.width() - 10, tempSprite.height() / 2, TFT_WHITE);

    // Add border
    tempSprite.drawRect(0, 0, tempSprite.width(), tempSprite.height(), TFT_WHITE);

    // Position and display (adjusted for 750x430 screen)
    int spriteX = 750 - tempSprite.width() - 20; // Top-right for 750px width
    int spriteY = 20;
    tempSprite.pushSprite(spriteX, spriteY);

    Serial.println("Temperature displayed with DigitalNumbers font");
}

// Display temperature in sci-fi style to match your blue glowing background
void displayTemperatureNerdMinerStyle(float temperature, int x, int y, const char* label, uint16_t color)
{
    char tempStr[12];
    snprintf(tempStr, sizeof(tempStr), "%.1f", temperature); // Just the number

    // Create sprite for gauge (adjusted size for 2 gauges side by side)
    LGFX_Sprite nmSprite(&lcd);
    nmSprite.createSprite(200, 90); // Slightly smaller than original (was 220x90)
    
    // Option 4: Crop background image at correct offset for this gauge position
    // The key is to offset the background by -x, -y so the correct portion shows through
    nmSprite.pushImage(-x, -y, 750, 430, background_img);
    
    // Link sprite to renderer
    render.setDrawer(nmSprite);

    // Large temperature number in specified color
    render.setFontSize(30); // Slightly smaller to fit better
    render.setFontColor(color);
    render.rdrawString(tempStr, 160, 20, color); // Right-aligned number

    // Unit text in same color but smaller  
    //render.setFontSize(16);
    //render.rdrawString("°C", 190, 35, color);

    // Label text
    render.setFontSize(10);
    render.rdrawString(label, 60, 65, 0x3DDF); // Medium blue like original

    // Push sprite to specified position and cleanup
    nmSprite.pushSprite(x, y);
    nmSprite.deleteSprite();

    Serial.printf("Temperature gauge displayed: %.1f°C at (%d, %d) with label '%s'\n", temperature, x, y, label);
}

// Display temperature at custom position
void displayTemperatureAt(float temperature, int x, int y)
{
    char tempStr[12];
    snprintf(tempStr, sizeof(tempStr), "%.1f°C", temperature);

    // Create sprite
    LGFX_Sprite customSprite(&lcd);
    bool spriteCreated = customSprite.createSprite(150, 60);
    if (!spriteCreated)
    {
        Serial.println("#### Temperature Sprite Error ####");
        return;
    }

    customSprite.setColorDepth(16);
    customSprite.setSwapBytes(true);
    customSprite.fillSprite(TFT_BLACK);

    // Link to renderer
    render.setDrawer(customSprite);
    render.setLineSpaceRatio(0.9);

    // Color-coded temperature
    uint16_t tempColor;
    if (temperature < 15)
    {
        tempColor = TFT_CYAN; // Cold
    }
    else if (temperature < 25)
    {
        tempColor = 0xDEDB; // Normal
    }
    else
    {
        tempColor = TFT_YELLOW; // Warm
    }

    render.setFontSize(24);
    render.setFontColor(tempColor, TFT_BLACK);

    // Center the text
    render.cdrawString(tempStr, customSprite.width() / 2, customSprite.height() / 2, tempColor);

    // Add border
    customSprite.drawRect(0, 0, customSprite.width(), customSprite.height(), tempColor);

    // Push to screen and cleanup
    customSprite.pushSprite(x, y);
    customSprite.deleteSprite();

    Serial.printf("Custom temperature displayed at (%d, %d)\n", x, y);
}

// Function createBackgroundSprite pattern
bool createTemperatureSprite(int16_t wdt, int16_t hgt)
{
    bool success = tempSprite.createSprite(wdt, hgt);

    if (success)
    {
        tempSprite.setColorDepth(16);
        tempSprite.setSwapBytes(true);
        render.setDrawer(tempSprite);
        render.setLineSpaceRatio(0.9);
        return true;
    }
    else
    {
        Serial.println("#### Temperature Sprite Error ####");
        Serial.printf("Size w:%d h:%d \n", wdt, hgt);
        return false;
    }
}
