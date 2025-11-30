#ifndef POWER_MGMT_H
#define POWER_MGMT_H

void power_init(void);
void power_update_inputs(float solar_v, float kinetic_mw);
uint32_t power_get_sampling_interval_ms(void);
void power_set_mode_low(bool low_power);

#endif // POWER_MGMT_H
