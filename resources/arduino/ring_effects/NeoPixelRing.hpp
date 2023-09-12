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
        unsigned long delay_ms,
        uint8_t brightness)
    {
        carousel(255, 255, 255, delay_ms, brightness, []() -> bool
            {
                return false;
            });
    }

    void carousel(
        unsigned long delay_ms,
        uint8_t brightness,
        bool (*stop_predicate)())
    {
        carousel(255, 255, 255, delay_ms, brightness, stop_predicate);
    }

    void carousel(
        uint8_t red,
        uint8_t green,
        uint8_t blue,
        unsigned long delay_ms,
        uint8_t brightness,
        bool (*stop_predicate)())
    {
        off();

        for(auto pixel = 0; pixel < num_pixels_ && !(*stop_predicate)(); pixel++)
        {
            fill(pixel, red, green, blue, brightness);
            step_delay(delay_ms, 10, stop_predicate);
        }

        for(auto pixel = 0; pixel < num_pixels_ && !(*stop_predicate)(); pixel++)
        {
            fill(pixel, 0, 0, 0, brightness);
            step_delay(delay_ms, 10, stop_predicate);
        }
    }

    void blink(
        unsigned long delay_ms)
    {
        blink(255, 255, 255, delay_ms, []() -> bool
            {
                return false;
            });
    }

    void blink(
        unsigned long delay_ms,
        bool (*stop_predicate)())
    {
        blink(255, 255, 255, delay_ms, stop_predicate);
    }

    void blink(
        uint8_t red,
        uint8_t green,
        uint8_t blue,
        unsigned long period_ms,
        bool (*stop_predicate)())
    {
        off();
        fill_all(red, green, blue);
        step_delay(period_ms, 10, stop_predicate);
        off();
        step_delay(period_ms, 10, stop_predicate);
    }

    void fade_in(
        unsigned long delay_ms)
    {
        fade_in(255, 255, 255, default_brightness_, delay_ms, []() -> bool
            {
                return false;
            });
    }

    void fade_in(
        unsigned long delay_ms,
        bool (*stop_predicate)())
    {
        fade_in(255, 255, 255, default_brightness_, delay_ms, stop_predicate);
    }

    void fade_in(
        uint8_t red,
        uint8_t green,
        uint8_t blue,
        uint8_t max_brightness,
        unsigned long duration_ms,
        bool (*stop_predicate)())
    {
        auto brightness_step = 1;
        auto num_steps = max_brightness;
        unsigned long delay_ms = duration_ms / num_steps;

        off();

        for (uint8_t brightness = 0; brightness < max_brightness && !(*stop_predicate)(); brightness += brightness_step)
        {
            fill_all(red, green, blue, brightness);
            delay(delay_ms);
        }
    }

    void fade_out(
        unsigned long delay_ms)
    {
        fade_out(255, 255, 255, default_brightness_, delay_ms, []() -> bool
            {
                return false;
            });
    }

    void fade_out(
        unsigned long delay_ms,
        bool (*stop_predicate)())
    {
        fade_out(255, 255, 255, default_brightness_, delay_ms, stop_predicate);
    }

    void fade_out(
        uint8_t red,
        uint8_t green,
        uint8_t blue,
        uint8_t max_brightness,
        unsigned long duration_ms,
        bool (*stop_predicate)())
    {
        auto brightness_step = 1;
        auto num_steps = max_brightness;
        unsigned long delay_ms = duration_ms / num_steps;

        off();
        fill_all(red, green, blue, max_brightness);

        for (uint8_t brightness = max_brightness; brightness != 0 && !(*stop_predicate)(); brightness -= brightness_step)
        {
            fill_all(red, green, blue, brightness);
            delay(delay_ms);
        }
    }

    void fade_carousel(
        unsigned long delay_ms,
        uint8_t max_brightness)
    {
        fade_carousel(255, 255, 255, delay_ms, max_brightness, []() -> bool
            {
                return false;
            });
    }

    void fade_carousel(
        unsigned long delay_ms,
        uint8_t max_brightness,
        bool (*stop_predicate)())
    {
        fade_carousel(255, 255, 255, delay_ms, max_brightness, stop_predicate);
    }

    void fade_carousel(
        uint8_t red,
        uint8_t green,
        uint8_t blue,
        unsigned long delay_ms,
        uint8_t max_brightness,
        bool (*stop_predicate)())
    {
        off();

        uint8_t brightness_step = max_brightness / num_pixels_;

        for(auto pixel = 0; pixel < num_pixels_ && !(*stop_predicate)(); pixel++)
        {
            fill(pixel, red, green, blue, (pixel + 1) * brightness_step);
            step_delay(delay_ms, 10, stop_predicate);
        }

        for(auto pixel = 0; pixel < num_pixels_ && !(*stop_predicate)(); pixel++)
        {
            fill(pixel, 0, 0, 0);
            step_delay(delay_ms, 10, stop_predicate);
        }
    }

    void incremental_carousel(
        unsigned long delay_ms,
        uint8_t max_brightness,
        uint8_t loops)
    {
        incremental_carousel(255, 255, 255, delay_ms, max_brightness, loops, []() -> bool
            {
                return false;
            });
    }

    void incremental_carousel(
        unsigned long delay_ms,
        uint8_t max_brightness,
        uint8_t loops,
        bool (*stop_predicate)())
    {
        incremental_carousel(255, 255, 255, delay_ms, max_brightness, loops, stop_predicate);
    }

    void incremental_carousel(
        uint8_t red,
        uint8_t green,
        uint8_t blue,
        unsigned long delay_ms,
        uint8_t max_brightness,
        uint8_t loops,
        bool (*stop_predicate)())
    {
        uint8_t brightness_step = max_brightness / loops;

        for(auto loop = 0; loop < loops && !(*stop_predicate)(); loop++)
        {
            carousel(red, green, blue, delay_ms,  (loop + 1) * brightness_step, stop_predicate);
        }

        for(auto loop = loops; loop != 0 && !(*stop_predicate)(); loop--)
        {
            carousel(red, green, blue, delay_ms,  loop * brightness_step, stop_predicate);
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

protected:

    void step_delay(
        unsigned long total_delay_ms,
        unsigned long delay_step,
        bool (*stop_predicate)())
    {
        unsigned long remaining_delay = total_delay_ms;
        while (remaining_delay > 0 && !(*stop_predicate)())
        {
            delay(remaining_delay >= delay_step ? delay_step : remaining_delay);
            remaining_delay = remaining_delay - delay_step;
        }
    }

private:

    uint16_t num_pixels_;

    int16_t pin_;

    uint8_t default_brightness_;

    Adafruit_NeoPixel pixels_;
};

}  // alpha
}  // ephemera
