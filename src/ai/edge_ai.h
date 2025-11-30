#ifndef EDGE_AI_H
#define EDGE_AI_H
#include "vitalink.h"

typedef struct {
	float heat_probability;
	float fatigue_probability;
	float hr_anomaly_probability;
	float gas_anomaly_probability;
	float unsafe_behavior_probability;
	float risk_score;
	uint8_t primary_reason;
} ai_result_t;

void ai_init(void);
ai_result_t ai_analyze_sample(const vl_sample_t *s);

#endif // EDGE_AI_H
