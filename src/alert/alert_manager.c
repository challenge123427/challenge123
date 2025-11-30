#include "alert_manager.h"
#include <zephyr.h>
#include <sys/printk.h>

void alert_init(void) {
    // init GPIO / PWM for haptic and buzzer in real hardware
}

void alert_haptic_pulse(int ms) {
    printk("HAPTIC pulse %dms (mock)\n", ms);
}

void alert_buzzer_beep(int ms) {
    printk("BUZZER beep %dms (mock)\n", ms);
}

void alert_trigger_local(float risk_score, uint8_t reason) {
    // simple local policy: vibration + beep for high risk, light for medium
    if (risk_score > 0.85f) {
        alert_haptic_pulse(1000);
        alert_buzzer_beep(1000);
    } else if (risk_score > 0.6f) {
        alert_haptic_pulse(500);
        alert_buzzer_beep(300);
    } else {
        alert_haptic_pulse(200);
    }
    printk("Alert triggered: score=%.2f reason=%d\n", risk_score, reason);
}
