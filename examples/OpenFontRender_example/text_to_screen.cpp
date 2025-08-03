#include <Arduino.h>
#include "LGFX_Waveshare_ESP32S3_LCD_4.3B.h"
#include <OpenFontRender.h>
#include "myFonts.h"

// Function declarations
void testDigitalNumbers();
void testNotoSansBold();
void testDSEG7Classic();
void displayWithDigitalNumbers(float temperature);
void displayWithNotoSans(float temperature);
void displayWithDSEG7(float temperature);

// Global font renderer
OpenFontRender render;

void setup()
{
    Serial.begin(115200);
    delay(1000);

    // Initialize display
    if (!initDisplay())
    {
        Serial.println("Display initialization failed!");
        while (1) delay(1000);
    }

    // Clear screen to black
    lcd.fillScreen(TFT_BLACK);

    // Set renderer to draw directly on screen
    render.setDrawer(lcd);
    render.setLineSpaceRatio(0.9);

    Serial.println("=== Font Testing Started ===");
    
    // Test all available fonts
    testDigitalNumbers();
    delay(2000);
    
    testNotoSansBold();
    delay(2000);
    
    Serial.println("Font testing complete!");
}

void loop()
{
    // Simple temperature simulation with different fonts
    static float temperature = 20.0;
    static unsigned long lastUpdate = 0;
    static int fontIndex = 0;
    
    if (millis() - lastUpdate > 4000) // Change every 4 seconds
    {
        temperature += random(-20, 20) / 10.0;
        if (temperature < 10.0) temperature = 10.0;
        if (temperature > 40.0) temperature = 40.0;
        
        // Clear screen
        lcd.fillScreen(TFT_BLACK);
        
        // Cycle through fonts
        switch (fontIndex % 3)
        {
            case 0:
                displayWithDigitalNumbers(temperature);
                break;
            case 1:
                displayWithNotoSans(temperature);
                break;
        }
        
        fontIndex++;
        lastUpdate = millis();
        Serial.printf("Temperature: %.1f C - Font: %d\n", temperature, fontIndex % 3);
    }
    
    delay(100);
}

void testDigitalNumbers()
{
    Serial.println("Testing DigitalNumbers font...");
    
    if (render.loadFont(DigitalNumbers, sizeof(DigitalNumbers)))
    {
        Serial.println("Error loading DigitalNumbers font!");
        return;
    }
    
    lcd.fillScreen(TFT_BLACK);
    
    // Title
    render.setFontSize(20);
    render.setFontColor(TFT_WHITE);
    //render.cdrawString("DigitalNumbers Font Test", 375, 30, TFT_WHITE);
    
    // Large number
    render.setFontSize(48);
    render.setFontColor(TFT_CYAN);
    render.cdrawString("23.7 C", 375, 120, TFT_CYAN);
    
    // Medium numbers
    render.setFontSize(32);
    render.setFontColor(TFT_YELLOW);
    render.cdrawString("123.456", 375, 200, TFT_YELLOW);
    
    // Small numbers
    render.setFontSize(20);
    render.setFontColor(TFT_GREEN);
    render.cdrawString("T12:34:56", 375, 280, TFT_GREEN);
    
    // Various digits
    render.setFontSize(24);
    render.setFontColor(TFT_MAGENTA);
    render.cdrawString("0123456789", 375, 350, TFT_MAGENTA);
    
    Serial.println("DigitalNumbers test complete");
}

void testNotoSansBold()
{
    Serial.println("Testing NotoSans_Bold font...");
    
    if (!render.loadFont(NotoSans_Bold, sizeof(NotoSans_Bold)))
    {
        Serial.println("Error loading NotoSans_Bold font!");
        return;
    }
    
    lcd.fillScreen(TFT_BLACK);
    
    // Title
    render.setFontSize(40);
    render.setFontColor(TFT_WHITE);
    render.cdrawString("NotoSans Bold Font Test", 375, 30, TFT_WHITE);
    
    // Large text
    render.setFontSize(36);
    render.setFontColor(TFT_CYAN);
    render.cdrawString("Temperature", 375, 100, TFT_CYAN);
    
    // Medium text
    render.setFontSize(28);
    render.setFontColor(TFT_YELLOW);
    render.cdrawString("25.3 C", 375, 160, TFT_YELLOW);
    
    // Mixed content
    render.setFontSize(18);
    render.setFontColor(TFT_GREEN);
    render.cdrawString("Current: 23.7 C", 375, 220, TFT_GREEN);
    render.cdrawString("Maximum: 31.2 C", 375, 250, TFT_GREEN);
    render.cdrawString("Minimum: 18.5 C", 375, 280, TFT_GREEN);
    
    // Small labels
    render.setFontSize(14);
    render.setFontColor(TFT_MAGENTA);
    render.cdrawString("Status: NORMAL", 375, 330, TFT_MAGENTA);
    render.cdrawString("System Ready", 375, 360, TFT_MAGENTA);
    
    Serial.println("NotoSans_Bold test complete");
}


void displayWithDigitalNumbers(float temperature)
{
    render.loadFont(DigitalNumbers, sizeof(DigitalNumbers));
    
    //render.setFontSize(80);
    //render.setFontColor(TFT_WHITE);
    //render.cdrawString("DigitalNumbers Font", 375, 50, TFT_WHITE);
    
    char tempStr[20];
    snprintf(tempStr, sizeof(tempStr), "%.1f ", temperature);
    
    render.setFontSize(80);
    render.setFontColor(TFT_CYAN);
    render.cdrawString(tempStr, 375, 200, TFT_CYAN);
}

void displayWithNotoSans(float temperature)
{
    render.loadFont(NotoSans_Bold, sizeof(NotoSans_Bold));
    
    render.setFontSize(50);
    render.setFontColor(TFT_WHITE);
    render.cdrawString("NotoSans Bold Font", 375, 50, TFT_WHITE);
    
    char tempStr[20];
    snprintf(tempStr, sizeof(tempStr), "%.1f C", temperature);
    
    render.setFontSize(50);
    render.setFontColor(TFT_YELLOW);
    render.cdrawString(tempStr, 375, 200, TFT_YELLOW);
}
