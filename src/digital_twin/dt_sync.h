#ifndef DT_SYNC_H
#define DT_SYNC_H
#include "vitalink.h"
#include "ai/edge_ai.h"

void dt_init(void);
void dt_enqueue_sample(const vl_sample_t *s, const ai_result_t *r);
int dt_sync_flush(void); // attempt to send batches via secure_comm

#endif // DT_SYNC_H
