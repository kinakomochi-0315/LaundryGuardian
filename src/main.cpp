#include <Arduino.h>
#include <M5Unified.h>
#include <M5UnitENV.h>
#include <Wire.h>

constexpr int UPDATE_MS = 1000;

BMP280 bmp280;
M5Canvas canvas(&M5.Lcd);

uint32_t nextBeepMs = 0;
bool isWarningCancelled = false;

void showLaundryWarningPage();
void showPressure(float pressure);

void setup()
{
    M5.begin();
    M5.Lcd.setRotation(1);

    canvas.setPsram(true);
    canvas.createSprite(M5.Lcd.width(), M5.Lcd.height());

    if (bmp280.begin(&Wire, BMP280_I2C_ADDR, 32, 33, 400000))
    {
        bmp280.setSampling(BMP280::MODE_NORMAL, BMP280::SAMPLING_X2, BMP280::SAMPLING_X16, BMP280::FILTER_X16,
                           BMP280::STANDBY_MS_500);
    }
}

void loop()
{
    M5.update();
    bmp280.update();

    const float pressure = bmp280.pressure / 100.0;

    if (M5.BtnA.wasPressed())
    {
        isWarningCancelled = true;
    }

    if ((pressure < 1000 && !isWarningCancelled) || M5.BtnB.isPressed() /*デバッグ用*/)
    {
        if (nextBeepMs < millis())
        {
            tone(2, 440, 1000);
            nextBeepMs = millis() + 2000;
        }

        showLaundryWarningPage();
    }
    else
    {
        noTone(2);
        isWarningCancelled = false;
        showPressure(pressure);
    }

    canvas.pushSprite(0, 0);
}

void showLaundryWarningPage()
{
    const int bgColor = (millis() / 1000) % 2 ? TFT_BLACK : TFT_RED;
    const int textColor = (millis() / 1000) % 2 ? TFT_WHITE : TFT_BLACK;

    const int width = M5.Lcd.width();
    const int height = M5.Lcd.height();

    canvas.fillScreen(bgColor);
    canvas.setTextColor(textColor, bgColor);

    canvas.drawCentreString("WARNING", width / 2, 8, &FreeSansBoldOblique18pt7b);

    canvas.drawCentreString("もうすぐ雨が降りそうです!!", width / 2, height / 2 - 18, &efontJA_16);
    canvas.drawCentreString("洗濯物を屋内に移動", width / 2, height / 2 + 10, &efontJA_16);
    canvas.drawCentreString("してください!!", width / 2, height / 2 + 34, &efontJA_16);
}

void showPressure(const float pressure)
{
    // 文字列に変換
    char pressureStr[10];
    dtostrf(pressure, 4, 0, pressureStr);

    canvas.fillScreen(TFT_WHITE);

    // 単位を表示
    canvas.setTextColor(TFT_DARKGRAY);
    canvas.drawCentreString("hPa", M5.Lcd.width() / 2, M5.Lcd.height() / 2 + 20, &FreeSans12pt7b);

    // 気圧を表示
    canvas.setTextColor(TFT_BLACK);
    canvas.drawCentreString(pressureStr, M5.Lcd.width() / 2, M5.Lcd.height() / 2 - 36, &Font7);
}