/**
 * Copyright (C) 2023, Ephemera Labs
 */

#include "NeoPixelRing.hpp"

/* Ring config */
constexpr uint16_t NUM_PIXELS = 8;
constexpr int16_t NEOPIXEL_PIN = 3;
constexpr uint8_t MAX_BRIGHTNESS = ephemera::alpha::NeoPixelRing::max_brightness();

/* Effects config */
constexpr size_t EFFECT_ITERATIONS = 3;
constexpr unsigned long CAROUSEL_DELAY = 100;
constexpr unsigned long BLINK_DELAY = 800;
constexpr unsigned long FADE_DELAY = 3000;

void setup()
{
}

void loop()
{
    ephemera::alpha::NeoPixelRing ring(NUM_PIXELS, NEOPIXEL_PIN, MAX_BRIGHTNESS);

    // Three iterations of green carousel
    for (size_t i = 0; i < EFFECT_ITERATIONS; i++)
    {
        ring.carousel(CAROUSEL_DELAY);
    }

    // Three iterations of red blinking
    for (size_t i = 0; i < EFFECT_ITERATIONS; i++)
    {
        ring.blink(BLINK_DELAY);
    }

    // Three iterations of red fading in and out
    for (size_t i = 0; i < EFFECT_ITERATIONS; i++)
    {
        ring.fade_in(FADE_DELAY);
        ring.fade_out(FADE_DELAY);
    }
}
