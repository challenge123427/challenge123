#ifndef ALERT_MANAGER_H
#define ALERT_MANAGER_H

void alert_init(void);
void alert_trigger_local(float risk_score, uint8_t reason);
void alert_haptic_pulse(int ms);
void alert_buzzer_beep(int ms);

#endif // ALERT_MANAGER_H
