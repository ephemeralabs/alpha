/**
 * Copyright (C) 2023, Ephemera Labs
 */

/**
 * @file EffectState.hpp
 */

namespace ephemera {
namespace alpha {

enum EffectState : uint8_t
{
    OFF_STATE = 0,
    CAROUSEL_STATE = 1,
    BLINK_STATE = 2,
    FADE_STATE = 3
};

String print_effect_state(EffectState& state)
{
    String ret;
    switch (state)
    {
    case ephemera::alpha::EffectState::OFF_STATE:
        ret = "OFF_STATE";
        break;
    case ephemera::alpha::EffectState::CAROUSEL_STATE:
        ret = "CAROUSEL_STATE";
        break;
    case ephemera::alpha::EffectState::BLINK_STATE:
        ret = "BLINK_STATE";
        break;
    case ephemera::alpha::EffectState::FADE_STATE:
        ret = "FADE_STATE";
        break;
    default:
        ret = "INVALID_STATE";
        break;
    }
    return ret;
}

}  // alpha
}  // ephemera
