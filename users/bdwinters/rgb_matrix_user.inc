RGB_MATRIX_EFFECT(white_and_alpha)
RGB_MATRIX_EFFECT(bdwinters)

#    ifdef RGB_MATRIX_CUSTOM_EFFECT_IMPLS

static bool white_and_alpha(effect_params_t* params) {
    RGB_MATRIX_USE_LIMITS(led_min, led_max);

    // TODO: capslock color

    HSV hsv = rgb_matrix_config.hsv;
    RGB field_keys = rgb_matrix_hsv_to_rgb(hsv);
    hsv.h = 0;
    hsv.s = 0;
    if (hsv.v > 0x40) {
        hsv.v -= 0x40;
    }
    RGB mod_keys = rgb_matrix_hsv_to_rgb(hsv);

    for (uint8_t i = led_min; i < led_max; i++) {
        RGB_MATRIX_TEST_LED_FLAGS();
        if (HAS_FLAGS(g_led_config.flags[i], LED_FLAG_MODIFIER)) {
            rgb_matrix_set_color(i, mod_keys.r, mod_keys.g, mod_keys.b);
        //} else if (HAS_FLAGS(g_led_config.flags[i], LED_FLAG_UNDERGLOW)) {
        } else {
            rgb_matrix_set_color(i, field_keys.r, field_keys.g, field_keys.b);
        }
    }
    return led_max < DRIVER_LED_TOTAL;
}

HSV bdwinters_math(HSV hsv, int16_t dx, int16_t dy, uint8_t dist, uint16_t tick) {
    uint16_t effect = tick - 2 * dist;
    if (effect > 255) effect = 255;
    hsv.v = qadd8(255 - hsv.v, effect);
    return hsv;
}

static bool bdwinters(effect_params_t* params) {
    RGB_MATRIX_USE_LIMITS(led_min, led_max);

    HSV hsv = rgb_matrix_config.hsv;
    hsv.h = 0;
    hsv.s = 0;
    if (hsv.v > 0x40) {
        hsv.v -= 0x40;
    }
    RGB mod_keys = rgb_matrix_hsv_to_rgb(hsv);

    uint8_t count = g_last_hit_tracker.count;
    for (uint8_t i = led_min; i < led_max; i++) {
        RGB_MATRIX_TEST_LED_FLAGS();
        if (HAS_FLAGS(g_led_config.flags[i], LED_FLAG_MODIFIER)) {
            rgb_matrix_set_color(i, mod_keys.r, mod_keys.g, mod_keys.b);
        } else {
            hsv = rgb_matrix_config.hsv;
            hsv.v = 0;
            for (uint8_t j = 0; j < count; j++) {
                int16_t  dx   = g_led_config.point[i].x - g_last_hit_tracker.x[j];
                int16_t  dy   = g_led_config.point[i].y - g_last_hit_tracker.y[j];
                uint8_t  dist = sqrt16(dx * dx + dy * dy);
                uint16_t tick = scale16by8(g_last_hit_tracker.tick[j], rgb_matrix_config.speed);
                hsv           = bdwinters_math(hsv, dx, dy, dist, tick);
            }
            hsv.v   = scale8(hsv.v, rgb_matrix_config.hsv.v);
            RGB rgb = rgb_matrix_hsv_to_rgb(hsv);
            rgb_matrix_set_color(i, rgb.r, rgb.g, rgb.b);
        }
    }
    return led_max < DRIVER_LED_TOTAL;
}

#    endif  // RGB_MATRIX_CUSTOM_EFFECT_IMPLS
