#ifndef VITALINK_H
#define VITALINK_H

#include <stdint.h>
#include <stdbool.h>

#define VL_MAX_PAYLOAD 1024
#define VL_TIMESTAMP() (uint64_t)k_uptime_get()

typedef struct {
	// core vitals
	uint16_t heart_rate;     // bpm
	uint8_t spo2;            // %
	float skin_temp_c;       // C
	// environment
	float ambient_temp_c;
	float humidity_pct;
	float air_quality_index;
	float gas_ppm;
	float vibration_m_s2;
	float noise_db;
	// motion / posture
	int16_t accel_x, accel_y, accel_z;
	uint64_t timestamp_ms;
} vl_sample_t;

typedef struct {
	uint8_t device_id[16];
	uint32_t seq_num;
} vl_meta_t;

#endif // VITALINK_H
