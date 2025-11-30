#ifndef SECURE_COMM_H
#define SECURE_COMM_H
#include <stdint.h>

void secure_comm_init(void);
bool secure_comm_is_connected(void);
int secure_comm_send(const uint8_t *data, int len);
int secure_comm_receive(uint8_t *buf, int maxlen);

// Mutual auth token management
int secure_comm_request_token(const char *role, uint32_t lifetime_s);
bool secure_comm_validate_remote(const uint8_t *cert, int len);

#endif // SECURE_COMM_H
