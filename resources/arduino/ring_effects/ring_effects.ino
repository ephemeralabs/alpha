/**
 * Copyright (C) 2023, Ephemera Labs
 */

#include "EffectState.hpp"
#include "NeoPixelRing.hpp"

/* Ring config */
constexpr uint16_t NUM_PIXELS = 8;
constexpr int16_t NEOPIXEL_PIN = 3;
constexpr uint8_t MAX_BRIGHTNESS = ephemera::alpha::NeoPixelRing::max_brightness();
ephemera::alpha::NeoPixelRing ring(NUM_PIXELS, NEOPIXEL_PIN, MAX_BRIGHTNESS);

/* Effects config */
constexpr size_t EFFECT_ITERATIONS = 3;
constexpr unsigned long CAROUSEL_DELAY = 100;
constexpr unsigned long BLINK_DELAY = 800;
constexpr unsigned long FADE_DELAY = 3000;

/* Button config */
constexpr int16_t BUTTON_PIN = 2;
ephemera::alpha::EffectState effect_state = ephemera::alpha::EffectState::OFF_STATE;
volatile int prev_pressed = 0;
volatile bool button_pressed = false;

void on_button_pressed()
{
    if (millis() - prev_pressed > 500)
    {
        button_pressed = true;
        prev_pressed = millis();
    }
}

void update_effect_state()
{
    switch (effect_state)
    {
    case ephemera::alpha::EffectState::OFF_STATE:
        effect_state = ephemera::alpha::EffectState::CAROUSEL_STATE;
        break;
    case ephemera::alpha::EffectState::CAROUSEL_STATE:
        effect_state = ephemera::alpha::EffectState::BLINK_STATE;
        break;
    case ephemera::alpha::EffectState::BLINK_STATE:
        effect_state = ephemera::alpha::EffectState::FADE_STATE;
        break;
    case ephemera::alpha::EffectState::FADE_STATE:
        effect_state = ephemera::alpha::EffectState::OFF_STATE;
        break;
    default:
        effect_state = ephemera::alpha::EffectState::OFF_STATE;
        break;
    }

    Serial.print("State set to ");
    Serial.println(ephemera::alpha::print_effect_state(effect_state));
}

void setup()
{
    Serial.begin(115200);
    while(!Serial);
    pinMode(BUTTON_PIN, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(BUTTON_PIN), on_button_pressed, RISING);
    Serial.println("Setup complete!");
}

void loop()
{
    if (button_pressed)
    {
        update_effect_state();
        button_pressed = false;
    }

    switch (effect_state)
    {
    case ephemera::alpha::EffectState::OFF_STATE:
        ring.off();
        break;
    case ephemera::alpha::EffectState::CAROUSEL_STATE:
        ring.carousel(CAROUSEL_DELAY);
        break;
    case ephemera::alpha::EffectState::BLINK_STATE:
        ring.blink(BLINK_DELAY);
        break;
    case ephemera::alpha::EffectState::FADE_STATE:
        ring.fade_in(FADE_DELAY);
        ring.fade_out(FADE_DELAY);
        break;
    default:
        ring.off();
        break;
    }
}
