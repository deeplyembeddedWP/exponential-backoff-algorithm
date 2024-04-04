/*
 * exponential_retry.h
 *
 *  Created on: Apr 3, 2024
 *  Author: Vinay Divakar
 */

#ifndef EXPONENTIAL_RETRY_H_
#define EXPONENTIAL_RETRY_H_

#include "stdint.h"

#define printf(...) do { } while (0)

struct exponential_retry_config_t {
	uint32_t delay_base_s;      	// base delay between retries
	uint32_t delay_backoff_max_s; // maximum backoff delay when all retries have exhausted
	uint32_t jitter_percentage; 	// maximum jitter added to retry_period_s
	uint8_t retry_count_max;    	// maximum number of retries
};

struct exponential_retry_t {
	uint8_t retry_counter;
	uint32_t total_jitter_period_s;
};

void exponential_retry_init(struct exponential_retry_t *this);
uint32_t exponential_retry_next(struct exponential_retry_t *this,
		const struct exponential_retry_config_t *config);
uint32_t exponential_retry_expiration(
		const struct exponential_retry_config_t *config);

#endif /* EXPONENTIAL_RETRY_H_ */
