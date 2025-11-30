#include "storage.h"
#include <zephyr.h>
#include <stdio.h>
#include <string.h>
#include <sys/printk.h>

static const char *queue_file = "vitalink_queue.bin";

void storage_init(void) {
#if defined(CONFIG_NATIVE_POSIX)
    // ensure file exists
    FILE *f = fopen(queue_file, "ab+");
    if (f) fclose(f);
#endif
}

int storage_append_sample(const vl_sample_t *s) {
#if defined(CONFIG_NATIVE_POSIX)
    FILE *f = fopen(queue_file, "ab");
    if (!f) return -1;
    fwrite(s, sizeof(*s), 1, f);
    fclose(f);
    return sizeof(*s);
#else
    // On-device: write to flash ring buffer
    return -1;
#endif
}

int storage_peek_batch(uint8_t *buf, int maxlen) {
#if defined(CONFIG_NATIVE_POSIX)
    FILE *f = fopen(queue_file, "rb");
    if (!f) return 0;
    int read = fread(buf, 1, maxlen, f);
    fclose(f);
    return read;
#else
    return 0;
#endif
}

int storage_pop_batch(int bytes) {
#if defined(CONFIG_NATIVE_POSIX)
    // Read whole file, skip bytes, rewrite remainder
    FILE *f = fopen(queue_file, "rb");
    if (!f) return -1;
    fseek(f, 0, SEEK_END);
    long len = ftell(f);
    fseek(f, 0, SEEK_SET);
    uint8_t *tmp = malloc(len);
    if (!tmp) { fclose(f); return -1; }
    fread(tmp, 1, len, f);
    fclose(f);
    FILE *f2 = fopen(queue_file, "wb");
    if (!f2) { free(tmp); return -1; }
    if (bytes < len) fwrite(tmp+bytes, 1, len-bytes, f2);
    fclose(f2);
    free(tmp);
    return 0;
#else
    return -1;
#endif
}
