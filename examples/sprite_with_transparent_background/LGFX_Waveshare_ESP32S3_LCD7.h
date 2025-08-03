/*
 * Hardware Configuration for Waveshare ESP32S3 Touch LCD 7
 * 
 * This file contains ALL hardware setup from your working code.
 * Just include this file and call initDisplay() once.
 * 
 * Based on your successful diagnostic test configuration.
 */

#pragma once

#define LGFX_USE_V1
#include <LovyanGFX.hpp>
#include <lgfx/v1/platforms/esp32s3/Panel_RGB.hpp>
#include <lgfx/v1/platforms/esp32s3/Bus_RGB.hpp>
#include <driver/i2c.h>

// ============================================================================
// HARDWARE CONFIGURATION
// ============================================================================

// CH422G I2C address
#define CH422G_I2C_ADDR 0x20

// ============================================================================
// LGFX HARDWARE CLASS - Your Working Configuration
// ============================================================================

class LGFX : public lgfx::LGFX_Device
{
public:
    lgfx::Bus_RGB _bus_instance;
    lgfx::Panel_RGB _panel_instance;
    lgfx::Touch_GT911 _touch_instance;

    LGFX(void)
    {
        // Panel configuration (800x480)
        {
            auto cfg = _panel_instance.config();
            cfg.memory_width = 800;
            cfg.memory_height = 480;
            cfg.panel_width = 800;
            cfg.panel_height = 480;
            cfg.offset_x = 0;
            cfg.offset_y = 0;
            _panel_instance.config(cfg);
        }

        // Panel detail configuration
        {
            auto cfg = _panel_instance.config_detail();
            cfg.use_psram = 1;
            _panel_instance.config_detail(cfg);
        }

        // RGB Bus configuration - Using working parameters from diagnostic
        {
            auto cfg = _bus_instance.config();
            cfg.panel = &_panel_instance;
            
            // RGB Data pins - Confirmed working from diagnostic
            cfg.pin_d0 = GPIO_NUM_14;   // B3 (DATA0)
            cfg.pin_d1 = GPIO_NUM_38;   // B4 (DATA1)  
            cfg.pin_d2 = GPIO_NUM_18;   // B5 (DATA2)
            cfg.pin_d3 = GPIO_NUM_17;   // B6 (DATA3)
            cfg.pin_d4 = GPIO_NUM_10;   // B7 (DATA4)
            cfg.pin_d5 = GPIO_NUM_39;   // G2 (DATA5)
            cfg.pin_d6 = GPIO_NUM_0;    // G3 (DATA6)
            cfg.pin_d7 = GPIO_NUM_45;   // G4 (DATA7)
            cfg.pin_d8 = GPIO_NUM_48;   // G5 (DATA8)
            cfg.pin_d9 = GPIO_NUM_47;   // G6 (DATA9)
            cfg.pin_d10 = GPIO_NUM_21;  // G7 (DATA10)
            cfg.pin_d11 = GPIO_NUM_1;   // R3 (DATA11)
            cfg.pin_d12 = GPIO_NUM_2;   // R4 (DATA12)
            cfg.pin_d13 = GPIO_NUM_42;  // R5 (DATA13)
            cfg.pin_d14 = GPIO_NUM_41;  // R6 (DATA14)
            cfg.pin_d15 = GPIO_NUM_40;  // R7 (DATA15)

            // Control pins - Confirmed working
            cfg.pin_henable = GPIO_NUM_5;   // DE (Data Enable)
            cfg.pin_vsync = GPIO_NUM_3;     // VSYNC
            cfg.pin_hsync = GPIO_NUM_46;    // HSYNC  
            cfg.pin_pclk = GPIO_NUM_7;      // PCLK

            // Timing configuration - Using working parameters
            cfg.freq_write = 16000000;      // 16MHz pixel clock
            
            // Working timing from diagnostic
            cfg.hsync_polarity = 0;         
            cfg.hsync_front_porch = 8;      
            cfg.hsync_pulse_width = 4;      
            cfg.hsync_back_porch = 8;       
            
            cfg.vsync_polarity = 0;
            cfg.vsync_front_porch = 8;      
            cfg.vsync_pulse_width = 4;      
            cfg.vsync_back_porch = 8;       
            
            cfg.pclk_idle_high = 1;         

            _bus_instance.config(cfg);
        }
        _panel_instance.setBus(&_bus_instance);

        // Touch configuration (GT911)
        {
            auto cfg = _touch_instance.config();
            cfg.x_min = 0;
            cfg.x_max = 799;
            cfg.y_min = 0;
            cfg.y_max = 479;
            cfg.pin_int = GPIO_NUM_4;       // Touch interrupt
            cfg.bus_shared = true;          // Share I2C bus with CH422G
            cfg.offset_rotation = 0;
            
            // I2C configuration for touch
            cfg.i2c_port = 0;               // Same I2C port as CH422G
            cfg.pin_sda = GPIO_NUM_8;       // Touch SDA
            cfg.pin_scl = GPIO_NUM_9;       // Touch SCL
            cfg.pin_rst = -1;               // Reset handled by CH422G
            cfg.freq = 400000;              // 400kHz
            cfg.i2c_addr = 0x5D;            // GT911 default address
            
            _touch_instance.config(cfg);
        }
        _panel_instance.setTouch(&_touch_instance);

        // Set the panel
        setPanel(&_panel_instance);
    }
};

// ============================================================================
// GLOBAL LCD INSTANCE - Must be declared before functions that use it
// ============================================================================

// Create the global LCD instance
LGFX lcd;

// ============================================================================
// CH422G HARDWARE FUNCTIONS - Your Working Implementation
// ============================================================================

void writeCH422G(uint8_t value) {
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (CH422G_I2C_ADDR << 1) | I2C_MASTER_WRITE, true);
    i2c_master_write_byte(cmd, value, true);
    i2c_master_stop(cmd);
    esp_err_t ret = i2c_master_cmd_begin(I2C_NUM_0, cmd, pdMS_TO_TICKS(100));
    i2c_cmd_link_delete(cmd);
    
    if (ret != ESP_OK) {
        Serial.printf("CH422G write failed: %s\n", esp_err_to_name(ret));
    }
}

bool initCH422G() {
    Serial.println("Initializing CH422G I/O expander...");
    
    // Initialize I2C
    i2c_config_t conf = {};
    conf.mode = I2C_MODE_MASTER;
    conf.sda_io_num = GPIO_NUM_8;
    conf.scl_io_num = GPIO_NUM_9;
    conf.sda_pullup_en = GPIO_PULLUP_ENABLE;
    conf.scl_pullup_en = GPIO_PULLUP_ENABLE;
    conf.master.clk_speed = 400000;
    conf.clk_flags = 0;
    
    esp_err_t ret = i2c_param_config(I2C_NUM_0, &conf);
    if (ret != ESP_OK) {
        Serial.printf("I2C param config failed: %s\n", esp_err_to_name(ret));
        return false;
    }
    
    ret = i2c_driver_install(I2C_NUM_0, conf.mode, 0, 0, 0);
    if (ret != ESP_OK) {
        Serial.printf("I2C driver install failed: %s\n", esp_err_to_name(ret));
        return false;
    }
    
    // Test CH422G communication and initialize
    writeCH422G(0x00);  // All pins low initially
    delay(10);
    writeCH422G(0x0C);  // LCD_RST high (bit 3) + LCD_BL high (bit 2)
    delay(100);
    
    Serial.println("CH422G initialized successfully");
    return true;
}

// ============================================================================
// HARDWARE INITIALIZATION FUNCTION
// ============================================================================

bool initDisplay() {
    Serial.println("=== Waveshare ESP32S3 LCD 7 - LovyanGFX Working Version ===");
    
    // Check PSRAM
    if (psramFound()) {
        Serial.printf("PSRAM found: %d bytes free\n", ESP.getFreePsram());
    } else {
        Serial.println("WARNING: PSRAM not found!");
        return false;
    }
    
    // Initialize CH422G first (this enables backlight and resets LCD)
    if (!initCH422G()) {
        Serial.println("FAILED: CH422G initialization failed!");
        return false;
    }
    
    // Initialize display
    Serial.println("Initializing LCD...");
    lcd.init();
    
    Serial.println("LCD initialized successfully!");
    
    // Display info
    Serial.printf("Display size: %d x %d\n", lcd.width(), lcd.height());
    Serial.printf("Color depth: %d bits\n", lcd.getColorDepth());
    Serial.printf("Touch available: %s\n", lcd.touch() ? "Yes" : "No");
    
    // Set rotation and color depth
    lcd.setRotation(0);  // Try 0, 1, 2, 3 if display orientation is wrong
    lcd.setColorDepth(16); // RGB565
    
    return true;
}