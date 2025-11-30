#ifndef SENSOR_DRIVER_H
#define SENSOR_DRIVER_H
#include "vitalink.h"
#include <stdbool.h>

void sensors_init(void);
bool sensor_read_all(vl_sample_t *s);

// power aware hook: let drivers reduce sample rate
void sensors_set_low_power(bool low_power);

#endif // SENSOR_DRIVER_H
