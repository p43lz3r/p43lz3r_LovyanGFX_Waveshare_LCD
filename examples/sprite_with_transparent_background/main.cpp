#include <Arduino.h>
#include "LGFX_Waveshare_ESP32S3_LCD_4.3B.h"
#include "images.h"
#include <OpenFontRender.h>
#include "myFonts.h" // NerdMiner font header - contains DigitalNumbers, NotoSans_Bold, etc.

// Function declarations
void displayBackground();
void displayTemperatureSprite(float temperature);
void displayTemperatureNerdMinerStyle(float temperature);
void displayTemperatureAt(float temperature, int x, int y);
uint16_t rgb565(uint8_t r, uint8_t g, uint8_t b);
uint16_t rgb565_lovyan(uint8_t r, uint8_t g, uint8_t b);

// Global variables
LGFX_Sprite tempSprite(&lcd); // Sprite for temperature display
OpenFontRender render;        // Font renderer (like NerdMiner)

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

    // Initialize OpenFontRender exactly like NerdMiner does
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

    // Set up render properties like NerdMiner
    render.setLineSpaceRatio(0.9); // Same as NerdMiner

    // Create temperature sprite
    tempSprite.createSprite(180, 70);

    // Display initial setup
    displayBackground();
    // displayTemperatureSprite(23.5);

    Serial.println("Setup complete with NerdMiner fonts!");
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
        displayTemperatureNerdMinerStyle(temperature); // Use sci-fi style matching background

        lastUpdate = millis();
        Serial.printf("Temperature updated: %.1f°C\n", temperature);
    }

    delay(100);
}

// Display the background image
void displayBackground()
{
    lcd.pushImage(0, 0, 750, 430, mrL_img); // Updated for 750x430 background
}

// Display temperature using NerdMiner DigitalNumbers font
void displayTemperatureSprite(float temperature)
{
    Serial.printf("Displaying temperature: %.1f°C with NerdMiner font\n", temperature);

    char tempStr[12];
    snprintf(tempStr, sizeof(tempStr), "%.1f°C", temperature);

    // Clear sprite
    tempSprite.fillSprite(TFT_BLACK);

    // Link sprite to renderer (like NerdMiner does)
    render.setDrawer(tempSprite);

    // Use DigitalNumbers font for temperature (like NerdMiner uses for numbers)
    render.setFontSize(32);
    render.setFontColor(TFT_WHITE, TFT_BLACK);

    // Use NerdMiner's rdrawString method for right-aligned text
    render.rdrawString(tempStr, tempSprite.width() - 10, tempSprite.height() / 2, TFT_WHITE);

    // Add border
    tempSprite.drawRect(0, 0, tempSprite.width(), tempSprite.height(), TFT_WHITE);

    // Position and display (adjusted for 750x430 screen)
    int spriteX = 750 - tempSprite.width() - 20; // Top-right for 750px width
    int spriteY = 20;
    tempSprite.pushSprite(spriteX, spriteY);

    Serial.println("Temperature displayed with NerdMiner DigitalNumbers font");
}

// Display temperature in sci-fi style to match your blue glowing background
void displayTemperatureNerdMinerStyle(float temperature)
{
    char tempStr[12];
    snprintf(tempStr, sizeof(tempStr), "%.1f", temperature); // Just the number
    char unitStr[] = "°C";

    // Create larger sprite for sci-fi style
    LGFX_Sprite nmSprite(&lcd);
    nmSprite.createSprite(220, 90);
    nmSprite.pushImage(-400, -200, 750, 430, mrL_img);

    // Semi-transparent dark background to blend with sci-fi theme
    // nmSprite.fillSprite(TFT_TRANSPARENT); // Very dark blue to match background

    // Link sprite to renderer
    render.setDrawer(nmSprite);

    // Large temperature number in glowing cyan to match border
    render.setFontSize(30);                         // Larger for sci-fi impact
    render.setFontColor(TFT_CYAN);                  // Bright cyan like the glowing borders
    render.rdrawString(tempStr, 160, 50, TFT_CYAN); // Right-aligned

    // Smaller unit text in bright blue
    // render.setFontSize(20);
    // render.rdrawString(unitStr, 210, 35, 0x07FF); // Bright blue

    // Add "TEMPERATURE" label in sci-fi style
    render.setFontSize(12);
    render.rdrawString("TEMP.", 70, 15, 0x3DDF); // Medium blue

    // Sci-fi style glowing border to match background
    // nmSprite.drawRect(0, 0, nmSprite.width(), nmSprite.height(), TFT_CYAN);
    // nmSprite.drawRect(1, 1, nmSprite.width()-2, nmSprite.height()-2, 0x07FF);
    // nmSprite.drawRect(2, 2, nmSprite.width()-4, nmSprite.height()-4, 0x3DDF);

    // Position in the center-right area where it won't interfere with border design
    // int x = 750 - nmSprite.width() - 100;  // A bit more margin from edge
    // int y = 215 - nmSprite.height()/2;    // Vertically centered

    int x = 400;
    int y = 200;

    nmSprite.pushSprite(x, y);
    nmSprite.deleteSprite();

    Serial.println("Temperature displayed in sci-fi style matching background");
}

// Display temperature at custom position using NerdMiner methods
void displayTemperatureAt(float temperature, int x, int y)
{
    char tempStr[12];
    snprintf(tempStr, sizeof(tempStr), "%.1f°C", temperature);

    // Create sprite like NerdMiner does
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

    // Link to renderer like NerdMiner
    render.setDrawer(customSprite);
    render.setLineSpaceRatio(0.9);

    // Color-coded temperature like NerdMiner uses different colors
    uint16_t tempColor;
    if (temperature < 15)
    {
        tempColor = TFT_CYAN; // Cold
    }
    else if (temperature < 25)
    {
        tempColor = 0xDEDB; // Normal (same as NerdMiner secondary color)
    }
    else
    {
        tempColor = TFT_YELLOW; // Warm
    }

    render.setFontSize(24);
    render.setFontColor(tempColor, TFT_BLACK);

    // Center the text like NerdMiner does
    render.cdrawString(tempStr, customSprite.width() / 2, customSprite.height() / 2, tempColor);

    // Add border
    customSprite.drawRect(0, 0, customSprite.width(), customSprite.height(), tempColor);

    // Push to screen and cleanup like NerdMiner
    customSprite.pushSprite(x, y);
    customSprite.deleteSprite();

    Serial.printf("Custom temperature displayed at (%d, %d)\n", x, y);
}

// Function to mimic NerdMiner's createBackgroundSprite pattern
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

// Advanced function: Multiple temperature displays like NerdMiner's multiple screens
void displayMultipleTemperatures(float temp1, float temp2, float temp3)
{
    // Display 3 temperatures in NerdMiner style layout

    // Temperature 1 - Top right (current) - positioned for 750x430 screen
    if (createTemperatureSprite(120, 50))
    {
        tempSprite.fillSprite(TFT_BLACK);
        render.setFontSize(20);
        render.setFontColor(TFT_WHITE, TFT_BLACK);

        char temp1Str[10];
        snprintf(temp1Str, sizeof(temp1Str), "%.1f°C", temp1);
        render.cdrawString(temp1Str, 60, 25, TFT_WHITE);

        tempSprite.drawRect(0, 0, 120, 50, TFT_WHITE);
        tempSprite.pushSprite(750 - 140, 20); // Adjusted for 750px width
        tempSprite.deleteSprite();
    }

    // Temperature 2 - Middle right (average)
    if (createTemperatureSprite(120, 50))
    {
        tempSprite.fillSprite(TFT_BLACK);
        render.setFontSize(18);
        render.setFontColor(0xDEDB, TFT_BLACK);

        char temp2Str[10];
        snprintf(temp2Str, sizeof(temp2Str), "%.1f°C", temp2);
        render.cdrawString(temp2Str, 60, 25, 0xDEDB);

        tempSprite.drawRect(0, 0, 120, 50, 0xDEDB);
        tempSprite.pushSprite(750 - 140, 80); // Adjusted for 750px width
        tempSprite.deleteSprite();
    }

    // Temperature 3 - Bottom right (max)
    if (createTemperatureSprite(120, 50))
    {
        tempSprite.fillSprite(TFT_BLACK);
        render.setFontSize(16);
        render.setFontColor(TFT_YELLOW, TFT_BLACK);

        char temp3Str[10];
        snprintf(temp3Str, sizeof(temp3Str), "%.1f°C", temp3);
        render.cdrawString(temp3Str, 60, 25, TFT_YELLOW);

        tempSprite.drawRect(0, 0, 120, 50, TFT_YELLOW);
        tempSprite.pushSprite(750 - 140, 140); // Adjusted for 750px width
        tempSprite.deleteSprite();
    }

    Serial.println("Multiple temperatures displayed NerdMiner style");
}
