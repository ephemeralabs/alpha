/**
 * Copyright (C) 2023, Ephemera Labs
 */

/**
 * @file NeoPixelRing.hpp
 */

#include <Adafruit_NeoPixel.h>

namespace ephemera {
namespace alpha {

class NeoPixelRing
{
public:
    NeoPixelRing(
            uint16_t num_pixels,
            int16_t pin)
        : NeoPixelRing(num_pixels, pin, max_brightness())
    {
    }

    NeoPixelRing(
            uint16_t num_pixels,
            int16_t pin,
            uint8_t default_brightness)
        : num_pixels_(num_pixels)
        , pin_(pin)
        , default_brightness_(default_brightness)
        , pixels_(num_pixels_, pin_, NEO_GRB + NEO_KHZ800)
    {
        pixels_.begin();
        pixels_.setBrightness(default_brightness_);
    }

    ~NeoPixelRing()
    {
        pixels_.clear();
        pixels_.show();
    }

    void off()
    {
        fill_all(0, 0, 0);
    }

    void carousel(
        unsigned long delay_ms)
    {
        carousel(255, 255, 255, delay_ms);
    }

    void carousel(
        uint8_t red,
        uint8_t green,
        uint8_t blue,
        unsigned long delay_ms)
    {
        off();

        for(auto pixel = 0; pixel < num_pixels_; pixel++)
        {
            fill(pixel, red, green, blue);
            delay(delay_ms);
        }

        for(auto pixel = 0; pixel < num_pixels_; pixel++)
        {
            fill(pixel, 0, 0, 0);
            delay(delay_ms);
        }
    }

    void blink(
        unsigned long delay_ms)
    {
        blink(255, 255, 255, delay_ms);
    }

    void blink(
        uint8_t red,
        uint8_t green,
        uint8_t blue,
        unsigned long period_ms)
    {
        off();
        fill_all(red, green, blue);
        delay(period_ms);
        off();
        delay(period_ms);
    }

    void fade_in(
        unsigned long delay_ms)
    {
        fade_in(255, 255, 255, default_brightness_, delay_ms);
    }

    void fade_in(
        uint8_t red,
        uint8_t green,
        uint8_t blue,
        uint8_t max_brightness,
        unsigned long duration_ms)
    {
        auto brightness_step = 1;
        auto num_steps = max_brightness;
        unsigned long delay_ms = duration_ms / num_steps;

        off();

        for (uint8_t brightness = 0; brightness < max_brightness; brightness += brightness_step)
        {
            fill_all(red, green, blue, brightness);
            delay(delay_ms);
        }
    }

    void fade_out(
        unsigned long delay_ms)
    {
        fade_out(255, 255, 255, default_brightness_, delay_ms);
    }

    void fade_out(
        uint8_t red,
        uint8_t green,
        uint8_t blue,
        uint8_t max_brightness,
        unsigned long duration_ms)
    {
        auto brightness_step = 1;
        auto num_steps = max_brightness;
        unsigned long delay_ms = duration_ms / num_steps;

        off();
        fill_all(red, green, blue, max_brightness);

        for (uint8_t brightness = max_brightness; brightness != 0; brightness -= brightness_step)
        {
            fill_all(red, green, blue, brightness);
            delay(delay_ms);
        }
    }

    void fill_all(
        uint8_t red,
        uint8_t green,
        uint8_t blue)
    {
        fill_all(red, green, blue, default_brightness_);
    }

    void fill_all(
        uint8_t red,
        uint8_t green,
        uint8_t blue,
        uint8_t brightness)
    {
        pixels_.clear();
        for(auto i = 0; i < num_pixels_; i++)
        {
            pixels_.setPixelColor(i, pixels_.Color(red, green, blue));
        }
        pixels_.setBrightness(brightness);
        pixels_.show();
    }

    void fill(
        uint16_t pixel,
        uint8_t red,
        uint8_t green,
        uint8_t blue)
    {
        fill(pixel, red, green, blue, default_brightness_);
    }

    void fill(
        uint16_t pixel,
        uint8_t red,
        uint8_t green,
        uint8_t blue,
        uint8_t brightness)
    {
        pixels_.setPixelColor(pixel, pixels_.Color(red, green, blue));
        pixels_.setBrightness(brightness);
        pixels_.show();
    }

    static constexpr uint8_t max_brightness()
    {
        return 255;
    }

private:

    uint16_t num_pixels_;

    int16_t pin_;

    uint8_t default_brightness_;

    Adafruit_NeoPixel pixels_;
};

}  // alpha
}  // ephemera
