/*
 * exponential_retry.c
 *
 *  Created on: Apr 3, 2024
 *  Author: Vinay Divakar
 */
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include "math.h"
#include "exponential_retry.h"

/**
 * @brief calculates the next backoff retry period
 *
 * @warning delay_backoff_max_s should always have a valid (>0) value,
 * if invalid (<=0), subsequent retry intervals are based off delay_base_s
 *
 * @param this points to the context to use
 * @param config configuration to use to calculate the exponential retry period
 * @return next exponential retry period in seconds
 */
uint32_t exponential_retry_next(struct exponential_retry_t *this,
		const struct exponential_retry_config_t *config) {

	int deadline = config->delay_base_s;

	if (this->retry_counter >= config->retry_count_max) {
		printf("retries exhausted %u\r\n", this->retry_counter);
		//perform subsequent retries in delay_backoff_max_s seconds if valid
		deadline =
				config->delay_backoff_max_s <= 0 ?
						config->delay_base_s : config->delay_backoff_max_s;
		this->total_jitter_period_s = 0; // reset and base it off delay_backoff_max_s
		goto RETRY_PERIOD_COMPUTE;
	}

	for (uint8_t i = 0; i < this->retry_counter; i++) {
		deadline += deadline;
	}

	this->retry_counter++; // track number of retries completed

	RETRY_PERIOD_COMPUTE:
	uint32_t jitter_max_deviation = (deadline * config->jitter_percentage)
			/ 100;
	uint32_t jitter_current = rand() % (jitter_max_deviation + 1U);
	this->total_jitter_period_s += jitter_current;

	uint32_t retry_period_s = deadline - this->total_jitter_period_s;

	printf("count: %u, deadline:%us, send_in:%us, total_jitter:%us\r\n",
			this->retry_counter, deadline, retry_period_s,
			this->total_jitter_period_s);

	return retry_period_s;
}

/**
 * @brief determine how long it will take to consume all retries
 *
 * @warning delays which occur outside the retry scheduling not included (ie network communications)
 *
 * @param config retry configuration
 * @return number of seconds until all retries are consumed
 */
uint32_t exponential_retry_expiration(
		const struct exponential_retry_config_t *config) {
	return config->delay_base_s << config->retry_count_max;
}

/**
 * @brief initializes the context
 * @param this points to the context to use
 */
void exponential_retry_init(struct exponential_retry_t *this) {
	memset(this, 0, sizeof(*this));
}

