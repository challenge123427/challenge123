#include <zephyr.h>
#include <sys/printk.h>
#include "vitalink.h"
#include "sensors/sensor_driver.h"
#include "ai/edge_ai.h"
#include "storage/storage.h"
#include "digital_twin/dt_sync.h"
#include "comm/secure_comm.h"
#include "power/power_mgmt.h"
#include "alert/alert_manager.h"

void sample_and_process(void)
{
    vl_sample_t s;
    if (!sensor_read_all(&s)) {
        printk("Sensor read failed\n");
        return;
    }
    // Local storage
    storage_append_sample(&s);
    // On-device inference
    ai_result_t res = ai_analyze_sample(&s);
    // Risk scoring & alerts
    if (res.risk_score > 0.7f) {
        alert_trigger_local(res.risk_score, res.primary_reason);
    }
    // Digital twin enqueue
    dt_enqueue_sample(&s, &res);
}

void comm_task(void)
{
    while (1) {
        // Try connect and sync batched deltas
        if (secure_comm_is_connected()) {
            // Mutual auth + send
            dt_sync_flush();
        } else {
            // low-power wait until next attempt
        }
        k_sleep(K_SECONDS(30));
    }
}

void main(void)
{
    printk("VitaLink starting\n");
    power_init();
    sensors_init();
    ai_init();
    storage_init();
    dt_init();
    secure_comm_init();
    alert_init();

    // Periodic sampling thread
    while (1) {
        sample_and_process();
        // Adaptive sampling based on power manager
        k_sleep(K_MSEC(power_get_sampling_interval_ms()));
    }
}