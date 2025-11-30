#include "sensor_driver.h"
#include <zephyr.h>
#include <stdlib.h>
#include <string.h>
#include <sys/rand32.h>

static bool mock_mode = true;
static bool low_power = false;

void sensors_init(void) {
#if defined(CONFIG_VITALINK_MOCK_SENSORS)
    mock_mode = true;
#else
    mock_mode = false;
    // Initialize real hardware sensors here
#endif
}

void sensors_set_low_power(bool lp) {
    low_power = lp;
}

static uint32_t rnd(uint32_t min, uint32_t max) {
    return min + (sys_rand32_get() % (max - min + 1));
}

bool sensor_read_all(vl_sample_t *s) {
    if (!s) return false;
    memset(s, 0, sizeof(*s));
    if (mock_mode) {
        s->heart_rate = 60 + rnd(0, 60);
        s->spo2 = 95 + (rnd(0,5));
        s->skin_temp_c = 32.0f + (rnd(0,100)/100.0f);
        s->ambient_temp_c = 20.0f + (rnd(0,300)/100.0f);
        s->humidity_pct = 30.0f + (rnd(0,700)/100.0f);
        s->air_quality_index = (float)rnd(10,150);
        s->gas_ppm = (float)rnd(0,200);
        s->vibration_m_s2 = (float)(rnd(0,200))/10.0f;
        s->noise_db = (float)rnd(30,100);
        s->accel_x = (int16_t)rnd(-2000,2000);
        s->accel_y = (int16_t)rnd(-2000,2000);
        s->accel_z = (int16_t)rnd(-2000,2000);
        s->timestamp_ms = k_uptime_get();
    } else {
        // Read actual sensors and fill s
    }
    // If low power, do lightweight smoothing or down-sampling here
    if (low_power) {
        s->heart_rate = (s->heart_rate/5)*5;
    }
    return true;
}
