#include "edge_ai.h"
#include <zephyr.h>
#include <math.h>

/*
  Real deployment: load TFLM model binaries from /models and run inference.
  Here: provide a lightweight rule-based fallback and hooks for integrating TFLM.
*/

void ai_init(void) {
    // Load tflm models if present (placeholder).
}

static float clamp01(float v) { if (v<0) return 0; if (v>1) return 1; return v; }

ai_result_t ai_analyze_sample(const vl_sample_t *s) {
    ai_result_t r = {0};
    if (!s) return r;

    // Heat stress heuristic: high ambient + high skin temp + high humidity
    float heat_score = (s->ambient_temp_c - 25.0f) / 15.0f;
    heat_score += (s->skin_temp_c - 33.0f)/5.0f;
    heat_score += (s->humidity_pct - 40.0f)/60.0f;
    heat_score = clamp01(heat_score/3.0f);
    r.heat_probability = heat_score;

    // Fatigue heuristic: low movement + elevated HR variability (mocked via accel low)
    float movement = fabsf(s->accel_x) + fabsf(s->accel_y) + fabsf(s->accel_z);
    float move_score = 1.0f - clamp01(movement / 3000.0f);
    float hr_score = (s->heart_rate > 90) ? 0.5f : 0.1f;
    r.fatigue_probability = clamp01((move_score + hr_score)/2.0f);

    // HR anomaly: extremes
    r.hr_anomaly_probability = (s->heart_rate < 40 || s->heart_rate > 120) ? 0.9f : 0.05f;

    // Gas anomaly: PPM threshold
    r.gas_anomaly_probability = (s->gas_ppm > 100.0f) ? 0.9f : 0.02f;

    // Unsafe behavior: vibration/noise + movement patterns
    float vib_score = clamp01(s->vibration_m_s2 / 20.0f);
    float noise_score = clamp01((s->noise_db - 70.0f) / 30.0f);
    r.unsafe_behavior_probability = clamp01((vib_score + noise_score)/2.0f);

    // Risk scoring engine (weighted)
    r.risk_score = clamp01(
        r.heat_probability * 0.25f +
        r.fatigue_probability * 0.20f +
        r.hr_anomaly_probability * 0.25f +
        r.gas_anomaly_probability * 0.20f +
        r.unsafe_behavior_probability * 0.10f
    );

    // primary reason index (1..5)
    float probs[5] = { r.heat_probability, r.fatigue_probability, r.hr_anomaly_probability, r.gas_anomaly_probability, r.unsafe_behavior_probability };
    int idx = 0; float maxp = probs[0];
    for (int i=1;i<5;i++) if (probs[i] > maxp) { maxp = probs[i]; idx = i; }
    r.primary_reason = (uint8_t)(idx+1);
    return r;
}
