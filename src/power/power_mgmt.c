#include "power_mgmt.h"
#include "sensors/sensor_driver.h"
#include <zephyr.h>
#include <sys/printk.h>

static float solar_v = 0.0f;
static float kinetic_mw = 0.0f;
static bool low_power_mode = false;

void power_init(void) {
    // Initialize ADCs, PMIC, supercap/battery monitor on real hardware
    solar_v = 0.0f;
    kinetic_mw = 0.0f;
}

void power_update_inputs(float solar, float kinetic) {
    solar_v = solar; kinetic_mw = kinetic;
    // Balance supercap vs battery (placeholder)
    if (solar_v > 2.5f || kinetic_mw > 50.0f) {
        low_power_mode = false;
    } else {
        low_power_mode = true;
    }
    power_set_mode_low(low_power_mode);
}

uint32_t power_get_sampling_interval_ms(void) {
    if (low_power_mode) return 15000; // 15s in low power
    return 3000; // 3s normal
}

void power_set_mode_low(bool low_power) {
    low_power_mode = low_power;
    sensors_set_low_power(low_power);
    printk("Power mode set: %s\n", low_power ? "LOW" : "NORMAL");
}
