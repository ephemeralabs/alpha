/**
 * Copyright (C) 2023, Ephemera Labs
 */

#include "NeoPixelRing.hpp"

constexpr uint16_t NUM_PIXELS     = 8;
constexpr int16_t  NEOPIXEL_PIN   = 3;
constexpr uint8_t  MAX_BRIGHTNESS = ephemera::alpha::NeoPixelRing::max_brightness();

void setup()
{
    Serial.begin(115200);
    while(!Serial);
}

void loop()
{
    ephemera::alpha::NeoPixelRing ring(NUM_PIXELS, NEOPIXEL_PIN, MAX_BRIGHTNESS);

    // Three iterations of green carousel
    for (size_t i = 0; i < 3; i++)
    {
        ring.carousel(100);
    }

    // Three iterations of red blinking
    for (size_t i = 0; i < 3; i++)
    {
        ring.blink(800);
    }

    // Three iterations of red fading in and out
    for (size_t i = 0; i < 3; i++)
    {
        ring.fade_in(2500);
        ring.fade_out(2500);
    }
}
