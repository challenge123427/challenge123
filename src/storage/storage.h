#ifndef STORAGE_H
#define STORAGE_H
#include "vitalink.h"
#include "ai/edge_ai.h"

void storage_init(void);
int storage_append_sample(const vl_sample_t *s);
int storage_peek_batch(uint8_t *buf, int maxlen);
int storage_pop_batch(int bytes);

#endif // STORAGE_H
