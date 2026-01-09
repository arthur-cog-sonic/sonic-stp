/*
 * Copyright 2019 Broadcom. The term "Broadcom" refers to Broadcom Inc. and/or
 * its subsidiaries.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef __STP_TIMER_H__
#define __STP_TIMER_H__

/*#include "kstart.h"*/

/* Application Timers Library
 * - Allow applications to use one system timer tick to process
 *   multiple application timers
 * - Applications would register to be notified about the system clock tick
 *   maybe every 100 ms. For a 100ms timer, this implementation can
 *   time upto 54 minutes.
 * - Applications requiring a longer time can use this functionality by making
 *   their tick function coarser. For an example, look at stptimer_tick().
 * - For an example routine, look at the comment at the end of this file
 */

/*
 * Y2K38 Fix: Changed TIMER value from UINT32 to UINT64 to prevent
 * timestamp overflow on January 19, 2038. The 32-bit value would
 * overflow at 2^31-1 seconds (2147483647 = 2038-01-19 03:14:07 UTC).
 */
typedef struct TIMER
{
	UINT32 active;
	UINT64 value;
} __attribute__((aligned(8))) TIMER;

uint64_t sys_get_seconds();
/*
 * start_timer()
 *		this function initializes the timer to the input start_value_in_ticks
 *		and marks it as an active timer.
 */
void start_timer(TIMER *timer, UINT64 start_value_in_ticks);

/*
 * stop_timer()
 *		this function stops the timer and marks it as an inactive timer
 */
void stop_timer(TIMER *timer);

/*
 * timer_expired()
 *		can be called every system tick.
 *		- if the timer is inactive, this function will return FALSE
 *		- if the timer is active, this function increments the timer value
 *		  by 1. after incrementing, checks if the timer value exceeds the
 *		  timer_limit_in_ticks. if it exceeds or equal to the limit, stops the
 *		  timer and returns TRUE, other wise returns FALSE
 */
bool timer_expired(TIMER *timer, UINT64 timer_limit_in_ticks);

/*
 * is_timer_active()
 *		returns TRUE if the timer is active, else returns FALSE
 */
bool is_timer_active(TIMER *timer);

/*
 * get_timer_value()
 *		fills in the the current value of the timer in ticks, return FALSE
 *		if the timer is inactive.
 */
bool get_timer_value(TIMER *timer, UINT64 *value_in_ticks);

/* USAGE EXAMPLE
 * ---------------------------------------------------------------------------
 *
 * - when system tick arrives, call application timer processing routine
 *   for example, app_tick()
 *
 *	void app_tick()
 *	{
 *		if (timer_expired(&timer1, 10)) // timer expiry every second
 *		{
 *			// do timer expiry routine for timer1
 *			timer1_expiry();
 *		}
 *
 *		if (timer_expired(&timer2, 100)) // timer expiry every 10 seconds
 *		{
 *			// do timer expiry routine for timer2
 *		}
 *	}
 *
 *	void timer1_expiry()
 *	{
 *		// processing
 *		start_timer(&timer1, 0); // restart timer
 *	}
 *
 *	void timer2_expiry()
 *	{
 *		if (condition)
 *		{
 *			// processing - do not restart timer
 *		}
 *		else
 *		{
 *			// else processing
 *			start_timer(&timer2, 10); // restart timer from second 1
 *		}
 *	}
 */
#endif /* __STP_TIMER_H__ */
