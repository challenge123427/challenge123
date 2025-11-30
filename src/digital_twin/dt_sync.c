#include "dt_sync.h"
#include "storage/storage.h"
#include "comm/secure_comm.h"
#include <string.h>
#include <stdio.h>

void dt_init(void) {}

void dt_enqueue_sample(const vl_sample_t *s, const ai_result_t *r) {
    // compact delta representation: absolute timestamp + key fields + risk
    struct {
        uint64_t ts;
        uint16_t hr;
        uint8_t spo2;
        float amb_temp;
        float gas_ppm;
        float risk;
    } rec;
    rec.ts = s->timestamp_ms;
    rec.hr = s->heart_rate;
    rec.spo2 = s->spo2;
    rec.amb_temp = s->ambient_temp_c;
    rec.gas_ppm = s->gas_ppm;
    rec.risk = r->risk_score;
    // append to storage as binary record (simple)
    storage_append_sample((const vl_sample_t*)&rec); // reuse append for simplicity
}

int dt_sync_flush(void) {
    uint8_t buf[1024];
    int n = storage_peek_batch(buf, sizeof(buf));
    if (n <= 0) return 0;
    // Build secure envelope and send
    int sent = secure_comm_send(buf, n);
    if (sent > 0) {
        storage_pop_batch(n);
        return sent;
    }
    return 0;
}
