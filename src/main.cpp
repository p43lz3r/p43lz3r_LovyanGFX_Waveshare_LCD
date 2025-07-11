/*
 * Main Application - Waveshare ESP32S3 Touch LCD 7
 *
 * Clean separation: Hardware config in header, application logic here.
 * Your working graphics functions extracted from the original code.
 */
#include <Arduino.h>
#include "LGFX_Waveshare_ESP32S3_LCD7.h"

LGFX_Sprite canvas;

static constexpr char text[] = "Hello world ! こんにちは世界！ this is long long string sample. 寿限無、寿限無、五劫の擦り切れ、海砂利水魚の、水行末・雲来末・風来末、喰う寝る処に住む処、藪ら柑子の藪柑子、パイポ・パイポ・パイポのシューリンガン、シューリンガンのグーリンダイ、グーリンダイのポンポコピーのポンポコナの、長久命の長助";
static constexpr size_t textlen = sizeof(text) / sizeof(text[0]);
size_t textpos = 0;

void testDisplay();
void testTouch();
void demoSprites();

void setup()
{
  Serial.begin(115200);
  delay(1000);

  // Initialize hardware (all config in header file)
  if (!initDisplay())
  {
    Serial.println("Display initialization failed!");
    while (1)
      delay(1000);
  }

  // Your application graphics start here
  // testDisplay();
  // testTouch();
  // demoSprites();

  lcd.init();

  // 画面が横長になるように回転
  if (lcd.width() < lcd.height())
    lcd.setRotation(lcd.getRotation() ^ 1);

  canvas.setColorDepth(8);
  canvas.setFont(&fonts::lgfxJapanMinchoP_32);
  canvas.setTextWrap(false);                 // 右端到達時のカーソル折り返しを禁止
  canvas.createSprite(lcd.width() + 36, 36); // 画面幅+１文字分の横幅を用意
}

void loop()
{
  int32_t cursor_x = canvas.getCursorX() - 1; // 現在のカーソル位置を取得し、1ドット左に移動
  if (cursor_x <= 0)                          // カーソル位置が左端に到達している場合は一周したと判断
  {
    textpos = 0;            // 文字列の読取り位置をリセット
    cursor_x = lcd.width(); // 新たな文字が画面右端に描画されるようにカーソル位置を変更
  }

  canvas.setCursor(cursor_x, 0);                       // カーソル位置を更新
  canvas.scroll(-1, 0);                                // キャンバスの内容を1ドット左にスクロール
  while (textpos < textlen && cursor_x <= lcd.width()) // 画面右端に文字が書けるか判定
  {
    canvas.print(text[textpos++]);  // 1バイトずつ出力 (マルチバイト文字でもこの処理で動作します)
    cursor_x = canvas.getCursorX(); // 出力後のカーソル位置を取得
  }

  canvas.pushSprite(&lcd, 0, 0);

  // Interactive touch test - your main application loop
  // uint16_t x, y;
  // if (lcd.getTouch(&x, &y))
  //{
  //  Serial.printf("Touch: (%d, %d)\n", x, y);
  //
  // Draw colorful circles at touch points
  //  static uint16_t colorIndex = 0;
  //  uint16_t colors[] = {TFT_RED, TFT_GREEN, TFT_BLUE, TFT_YELLOW, TFT_CYAN, TFT_MAGENTA, TFT_WHITE, TFT_ORANGE};
  //  lcd.fillCircle(x, y, 10, colors[colorIndex % 8]);
  //  colorIndex++;
  //}
  // delay(50);
}

// ============================================================================
// YOUR GRAPHICS APPLICATION FUNCTIONS
// ============================================================================

void testDisplay()
{
  Serial.println("Testing display functionality...");

  // Clear screen
  lcd.fillScreen(TFT_BLACK);
  delay(500);

  // Test solid colors
  Serial.println("Testing solid colors...");
  lcd.fillScreen(TFT_RED);
  delay(500);
  lcd.fillScreen(TFT_GREEN);
  delay(500);
  lcd.fillScreen(TFT_BLUE);
  delay(500);
  lcd.fillScreen(TFT_WHITE);
  delay(500);

  // Color gradient test
  Serial.println("Drawing color bars...");
  lcd.fillScreen(TFT_BLACK);
  int barWidth = lcd.width() / 8;
  lcd.fillRect(0 * barWidth, 0, barWidth, lcd.height(), TFT_RED);
  lcd.fillRect(1 * barWidth, 0, barWidth, lcd.height(), TFT_GREEN);
  lcd.fillRect(2 * barWidth, 0, barWidth, lcd.height(), TFT_BLUE);
  lcd.fillRect(3 * barWidth, 0, barWidth, lcd.height(), TFT_YELLOW);
  lcd.fillRect(4 * barWidth, 0, barWidth, lcd.height(), TFT_CYAN);
  lcd.fillRect(5 * barWidth, 0, barWidth, lcd.height(), TFT_MAGENTA);
  lcd.fillRect(6 * barWidth, 0, barWidth, lcd.height(), TFT_WHITE);
  lcd.fillRect(7 * barWidth, 0, barWidth, lcd.height(), TFT_ORANGE);

  // Add text overlay
  lcd.setTextColor(TFT_BLACK, TFT_WHITE);
  lcd.setTextSize(3);
  lcd.setCursor(50, 50);
  lcd.println("Waveshare ESP32S3");
  lcd.setCursor(50, 100);
  lcd.println("LCD 7 inch");
  lcd.setCursor(50, 150);
  lcd.println("LovyanGFX Working!");

  lcd.setTextSize(2);
  lcd.setCursor(50, 220);
  lcd.printf("Resolution: %dx%d", lcd.width(), lcd.height());
  lcd.setCursor(50, 250);
  lcd.println("RGB565 - 16-bit color");
  lcd.setCursor(50, 280);
  lcd.println("Touch: Tap anywhere");

  delay(3000);
}

void testTouch()
{
  Serial.println("Testing touch for 10 seconds...");

  lcd.fillScreen(TFT_BLACK);
  lcd.setTextColor(TFT_WHITE);
  lcd.setTextSize(2);
  lcd.setCursor(50, 100);
  lcd.println("Touch Test");
  lcd.setCursor(50, 150);
  lcd.println("Touch anywhere on screen");
  lcd.setCursor(50, 200);
  lcd.println("Colored circles will appear");
  lcd.setCursor(50, 250);
  lcd.println("Test runs for 10 seconds...");

  // Touch test for 10 seconds
  unsigned long startTime = millis();
  int touchCount = 0;
  while (millis() - startTime < 10000)
  {
    uint16_t x, y;
    if (lcd.getTouch(&x, &y))
    {
      touchCount++;
      Serial.printf("Touch %d: (%d, %d)\n", touchCount, x, y);

      // Draw colorful touch indicators
      uint16_t color = lcd.color888(random(255), random(255), random(255));
      lcd.fillCircle(x, y, 15, color);
      lcd.drawCircle(x, y, 20, TFT_WHITE);
    }
    delay(50);
  }

  lcd.setCursor(50, 320);
  lcd.printf("Touch test completed");
  lcd.setCursor(50, 350);
  lcd.printf("Touches detected: %d", touchCount);

  Serial.printf("Touch test completed. Total touches: %d\n", touchCount);
  delay(2000);
}

void demoSprites()
{
  Serial.println("Demonstrating sprite functionality...");

  // Create a sprite for smooth animation
  LGFX_Sprite sprite(&lcd);
  sprite.createSprite(100, 100);
  sprite.setColorDepth(16);

  lcd.fillScreen(TFT_BLACK);
  lcd.setTextColor(TFT_WHITE);
  lcd.setTextSize(2);
  lcd.setCursor(50, 50);
  lcd.println("Sprite Animation Demo");
  lcd.setCursor(50, 80);
  lcd.println("Moving circle with rotation");

  // Animate sprite for 5 seconds
  unsigned long startTime = millis();
  float angle = 0;
  int x = 0, y = 200;
  int dx = 3, dy = 2;

  while (millis() - startTime < 5000)
  {
    // Clear sprite
    sprite.fillSprite(TFT_BLACK);

    // Draw rotating content on sprite
    sprite.fillCircle(50, 50, 30, lcd.color888(255, (int)(128 + 127 * sin(angle)), (int)(128 + 127 * cos(angle))));
    sprite.drawCircle(50, 50, 40, TFT_WHITE);
    sprite.setTextColor(TFT_WHITE);
    sprite.setTextSize(1);
    sprite.setCursor(35, 45);
    sprite.printf("%.0f°", angle * 180 / PI);

    // Set rotation center and push to screen
    sprite.setPivot(50, 50);
    sprite.pushRotateZoom(x + 50, y + 50, angle * 180 / PI, 1.0, 1.0);

    // Update position and angle
    x += dx;
    y += dy;
    angle += 0.1;

    // Bounce off edges
    if (x <= 0 || x >= lcd.width() - 100)
      dx = -dx;
    if (y <= 120 || y >= lcd.height() - 100)
      dy = -dy;

    delay(50);
  }

  sprite.deleteSprite();
  Serial.println("Sprite demo completed");

  // Show final message
  lcd.fillScreen(TFT_BLACK);
  lcd.setTextColor(TFT_GREEN);
  lcd.setTextSize(3);
  lcd.setCursor(100, 200);
  lcd.println("LovyanGFX Setup");
  lcd.setCursor(100, 240);
  lcd.println("SUCCESSFUL!");

  lcd.setTextSize(2);
  lcd.setTextColor(TFT_WHITE);
  lcd.setCursor(50, 320);
  lcd.println("You can now use all LovyanGFX features:");
  lcd.setCursor(50, 350);
  lcd.println("- Display graphics");
  lcd.setCursor(50, 380);
  lcd.println("- Touch input");
  lcd.setCursor(50, 410);
  lcd.println("- Sprite animations");
}