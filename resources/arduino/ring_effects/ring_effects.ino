/**
 * Copyright (C) 2023, Ephemera Labs
 */

/*
 * http://busyducks.com/ascii-art-arduinos
 *
 *                                +-----+
 *   +----[PWR]-------------------| USB |--+
 *   |                            +-----+  |
 *   |         GND/RST2  [ ][ ]            |
 *   |       MOSI2/SCK2  [ ][ ]  A5/SCL[ ] |   C5
 *   |          5V/MISO2 [ ][ ]  A4/SDA[ ] |   C4
 *   |                             AREF[ ] |
 *   |                              GND[ ] |
 *   | [ ]N/C                    SCK/13[ ] |   B5
 *   | [ ]IOREF                 MISO/12[ ] |   .
 *   | [ ]RST                   MOSI/11[ ]~|   .
 *   | [ ]3V3    +---+               10[ ]~|   .
 *   | [ ]5v    -| A |-               9[ ]~|   .
 *   | [ ]GND   -| R |-               8[ ] |   B0
 *   | [ ]GND   -| D |-                    |
 *   | [ ]Vin   -| U |-               7[ ] |   D7
 *   |          -| I |-               6[ ]~|   .
 *   | [ ]A0    -| N |-               5[ ]~|   .
 *   | [ ]A1    -| O |-               4[ ] |   .
 *   | [P]A2     +---+           INT1/3[R]~|   .
 *   | [ ]A3                     INT0/2[B] |   .
 *   | [ ]A4/SDA  RST SCK MISO     TX>1[ ] |   .
 *   | [ ]A5/SCL  [ ] [ ] [ ]      RX<0[ ] |   D0
 *   |            [ ] [ ] [ ]              |
 *   |  UNO_R3    GND MOSI 5V  ____________/
 *    \_______________________/
 *
 * R: NeoPixel Ring          B: Change effect button
 *
 * +-------+                        +-----------+
 * |       |                        |           |
 * |   RS [ ]---D3           GND---[ ] BG   B2 [ ]
 * |       |                        |           |
 * |   RV [ ]---5v                 [ ] B1   BS [ ]---D2
 * |       |                        |           |
 * |   RG [ ]---GND                 +-----------+
 * |       |
 * +-------+
 *
 * P: Potentiometer
 *
 * +-------+
 * |       |
 * |   PS [ ]---A2
 * |       |
 * |   PV [ ]---5v
 * |       |
 * |   PG [ ]---GND
 * |       |
 * +-------+
 */

#include <util/atomic.h>

#include "EffectState.hpp"
#include "NeoPixelRing.hpp"

/* Serial config */
constexpr unsigned long BAUDRATE = 115200;

/* Ring config */
constexpr uint16_t NUM_PIXELS = 8;
constexpr int16_t NEOPIXEL_PIN = 3;
constexpr uint8_t MAX_BRIGHTNESS = ephemera::alpha::NeoPixelRing::max_brightness();
ephemera::alpha::NeoPixelRing ring(NUM_PIXELS, NEOPIXEL_PIN, MAX_BRIGHTNESS);

/* Button config */
constexpr int8_t BUTTON_PIN = 2;
constexpr uint8_t BUTTON_MIN_DIFF_MS = 500;
ephemera::alpha::EffectState effect_state = ephemera::alpha::EffectState::OFF_STATE;
volatile int prev_pressed = 0;
volatile bool button_pressed = false;

/* Potentiometer config */
constexpr int8_t POTENTIOMETER_PIN = A2;
constexpr uint8_t NUM_EFFECTS = 6;
constexpr uint16_t MAX_POTENTIOMETER_READ = 1024;
constexpr uint8_t POTENTIOMETER_MIN_DIFF = 75;
constexpr uint8_t POTENTIOMETER_READS_FOR_AVG = 10;
int potentiometer_read_prev = 0;
int potentiometer_read = 0;

/* Effects config */
constexpr uint8_t INCREMENTAL_CAROUSEL_LOOPS = 5;
unsigned long effect_delays_ms[NUM_EFFECTS] = {
        0,     // OFF_STATE  (not used)
        100,   // CAROUSEL_STATE
        800,   // BLINK_STATE
        2000,  // FADE_STATE
        1000,  // FADE_CAROUSEL_STATE
        50,   // INCREMENTAL_CAROUSEL_STATE
};
constexpr unsigned long max_effect_delays_ms[NUM_EFFECTS] = {
        0,     // OFF_STATE  (not used)
        1000,  // CAROUSEL_STATE
        3000,  // BLINK_STATE
        6000,  // FADE_STATE
        6000,  // FADE_CAROUSEL_STATE
        1000,  // INCREMENTAL_CAROUSEL_STATE
};

void on_button_pressed()
{
    if (millis() - prev_pressed > BUTTON_MIN_DIFF_MS)
    {
        button_pressed = true;
        prev_pressed = millis();
    }
}

void print_active_state()
{
    Serial.print("Active state: ");
    Serial.print(ephemera::alpha::print_effect_state(effect_state));
    Serial.print(" - ");
    Serial.print(effect_delays_ms[effect_state]);
    Serial.println(" [ms]");
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
        effect_state = ephemera::alpha::EffectState::FADE_CAROUSEL_STATE;
        break;
    case ephemera::alpha::EffectState::FADE_CAROUSEL_STATE:
        effect_state = ephemera::alpha::EffectState::INCREMENTAL_CAROUSEL_STATE;
        break;
    case ephemera::alpha::EffectState::INCREMENTAL_CAROUSEL_STATE:
        effect_state = ephemera::alpha::EffectState::OFF_STATE;
        break;
    default:
        effect_state = ephemera::alpha::EffectState::OFF_STATE;
        break;
    }

    print_active_state();
}

bool get_button_pressed_atomic()
{
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE);
    return button_pressed;
}

int read_potentiometer()
{
    int ret = 0;
    for (auto i = 0; i < POTENTIOMETER_READS_FOR_AVG; i++)
    {
        ret += analogRead(POTENTIOMETER_PIN);
    }
    return ret / 10;
}

void update_effect_delay()
{
    if (effect_state == ephemera::alpha::EffectState::OFF_STATE)
    {
        return;
    }

    int potentiometer_read = read_potentiometer();
    if (abs(potentiometer_read - potentiometer_read_prev) > POTENTIOMETER_MIN_DIFF)
    {
        Serial.print("Updated state: ");
        Serial.print(ephemera::alpha::print_effect_state(effect_state));
        Serial.print(" - ");
        Serial.print(effect_delays_ms[effect_state]);
        effect_delays_ms[effect_state] = map(
            potentiometer_read,               // Mapped value
            0,                                // fromLow
            MAX_POTENTIOMETER_READ,           // fromHigh
            0,                                // toLow
            max_effect_delays_ms[effect_state]   // toHigh
        );
        Serial.print(" [ms] -> ");
        Serial.print(effect_delays_ms[effect_state]);
        Serial.println(" [ms]");
        potentiometer_read_prev = potentiometer_read;
    }
}

void print_start()
{
    Serial.println("");
    Serial.println("****************************************");
    Serial.println("* EPHEMERA LABS - ALPHA TEST PROTOTYPE *");
    Serial.println("****************************************");
    Serial.println("");
    Serial.println("Current supported effects and there default configuration delays are:");
    Serial.println("");

    for (auto i = 0; i < NUM_EFFECTS; i++)
    {
        ephemera::alpha::EffectState state = static_cast<ephemera::alpha::EffectState>(i);;
        Serial.print("  - ");
        Serial.print(print_effect_state(state));
        Serial.print(" - ");
        Serial.print(effect_delays_ms[i]);
        Serial.println(" [ms]");
    }

    Serial.println("");
    Serial.println("1. To switch between effects, please press the button.");
    Serial.println("2. To adjust the configuration delay of the active state, please");
    Serial.println("   rotate the potentiometer");
    Serial.println("");
    Serial.println("STATUS LOG");
    Serial.println("----------");
    print_active_state();
}

void setup()
{
    Serial.begin(BAUDRATE);
    while(!Serial);

    pinMode(BUTTON_PIN, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(BUTTON_PIN), on_button_pressed, RISING);

    pinMode(POTENTIOMETER_PIN, INPUT);
    potentiometer_read_prev = read_potentiometer();
    potentiometer_read = potentiometer_read_prev;

    print_start();
}

void loop()
{
    if (button_pressed)
    {
        update_effect_state();
        button_pressed = false;
    }

    update_effect_delay();

    switch (effect_state)
    {
    case ephemera::alpha::EffectState::OFF_STATE:
        ring.off();
        break;
    case ephemera::alpha::EffectState::CAROUSEL_STATE:
        ring.carousel(
            effect_delays_ms[effect_state],
            MAX_BRIGHTNESS,
            get_button_pressed_atomic);
        break;
    case ephemera::alpha::EffectState::BLINK_STATE:
        ring.blink(
            effect_delays_ms[effect_state],
            MAX_BRIGHTNESS,
            get_button_pressed_atomic);
        break;
    case ephemera::alpha::EffectState::FADE_STATE:
        ring.fade_in(
            effect_delays_ms[effect_state],
            MAX_BRIGHTNESS,
            get_button_pressed_atomic);
        ring.fade_out(
            effect_delays_ms[effect_state],
            MAX_BRIGHTNESS,
            get_button_pressed_atomic);
        break;
    case ephemera::alpha::EffectState::FADE_CAROUSEL_STATE:
        ring.fade_carousel(
            effect_delays_ms[effect_state],
            MAX_BRIGHTNESS,
            get_button_pressed_atomic);
        break;
    case ephemera::alpha::EffectState::INCREMENTAL_CAROUSEL_STATE:
        ring.incremental_carousel(
            effect_delays_ms[effect_state],
            MAX_BRIGHTNESS,
            INCREMENTAL_CAROUSEL_LOOPS,
            get_button_pressed_atomic);
        break;
    default:
        ring.off();
        break;
    }
}
