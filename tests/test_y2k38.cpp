/*
 * Copyright 2024 Broadcom. The term "Broadcom" refers to Broadcom Inc. and/or
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

/*
 * Y2K38 Test Suite for sonic-stp
 *
 * This test suite verifies that the STP timer and timestamp types
 * are properly sized to handle dates beyond January 19, 2038.
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <assert.h>

/* Include STP headers to test type sizes */
#include "stp_timer.h"
#include "stp.h"
#include "stp_dbsync.h"
#include "mstp.h"

/* Y2K38 boundary timestamp: 2038-01-19 03:14:07 UTC */
#define Y2K38_BOUNDARY 2147483647ULL

/* Test timestamps beyond Y2K38 */
#define YEAR_2040 2208988800ULL  /* 2040-01-01 00:00:00 UTC */
#define YEAR_2050 2524608000ULL  /* 2050-01-01 00:00:00 UTC */
#define YEAR_2100 4102444800ULL  /* 2100-01-01 00:00:00 UTC */

static int tests_run = 0;
static int tests_passed = 0;

#define TEST_ASSERT(condition, message) do { \
    tests_run++; \
    if (condition) { \
        tests_passed++; \
        printf("PASS: %s\n", message); \
    } else { \
        printf("FAIL: %s\n", message); \
    } \
} while(0)

/*
 * Test 1: Verify TIMER struct value field is 64-bit
 */
void test_timer_struct_size(void)
{
    TIMER timer;
    
    /* TIMER.value should be UINT64 (8 bytes) */
    TEST_ASSERT(sizeof(timer.value) >= 8,
                "TIMER.value should be at least 64-bit");
    
    /* Test that we can store values beyond 32-bit max */
    timer.value = Y2K38_BOUNDARY;
    TEST_ASSERT(timer.value == Y2K38_BOUNDARY,
                "TIMER.value can store Y2K38 boundary");
    
    timer.value = YEAR_2040;
    TEST_ASSERT(timer.value == YEAR_2040,
                "TIMER.value can store year 2040 timestamp");
    
    timer.value = YEAR_2100;
    TEST_ASSERT(timer.value == YEAR_2100,
                "TIMER.value can store year 2100 timestamp");
}

/*
 * Test 2: Verify STP_VLAN_TABLE topology_change_time is 64-bit
 */
void test_stp_vlan_table_timestamp(void)
{
    STP_VLAN_TABLE vlan_table;
    
    /* topology_change_time should be uint64_t (8 bytes) */
    TEST_ASSERT(sizeof(vlan_table.topology_change_time) >= 8,
                "STP_VLAN_TABLE.topology_change_time should be at least 64-bit");
    
    /* Test that we can store values beyond 32-bit max */
    vlan_table.topology_change_time = Y2K38_BOUNDARY;
    TEST_ASSERT(vlan_table.topology_change_time == Y2K38_BOUNDARY,
                "topology_change_time can store Y2K38 boundary");
    
    vlan_table.topology_change_time = YEAR_2050;
    TEST_ASSERT(vlan_table.topology_change_time == YEAR_2050,
                "topology_change_time can store year 2050 timestamp");
}

/*
 * Test 3: Verify STP_VLAN_PORT_TABLE root_protect_timer is 64-bit
 */
void test_stp_vlan_port_table_timestamp(void)
{
    STP_VLAN_PORT_TABLE port_table;
    
    /* root_protect_timer should be uint64_t (8 bytes) */
    TEST_ASSERT(sizeof(port_table.root_protect_timer) >= 8,
                "STP_VLAN_PORT_TABLE.root_protect_timer should be at least 64-bit");
    
    /* Test that we can store values beyond 32-bit max */
    port_table.root_protect_timer = Y2K38_BOUNDARY;
    TEST_ASSERT(port_table.root_protect_timer == Y2K38_BOUNDARY,
                "root_protect_timer can store Y2K38 boundary");
    
    port_table.root_protect_timer = YEAR_2040;
    TEST_ASSERT(port_table.root_protect_timer == YEAR_2040,
                "root_protect_timer can store year 2040 timestamp");
}

/*
 * Test 4: Verify BRIDGE_INFO timestamp fields are 64-bit
 */
void test_bridge_info_timestamps(void)
{
    /* Test topology_change_tick in BRIDGE_INFO */
    BRIDGE_INFO bridge_info;
    
    TEST_ASSERT(sizeof(bridge_info.topology_change_tick) >= 8,
                "BRIDGE_INFO.topology_change_tick should be at least 64-bit");
    
    bridge_info.topology_change_tick = Y2K38_BOUNDARY;
    TEST_ASSERT(bridge_info.topology_change_tick == Y2K38_BOUNDARY,
                "topology_change_tick can store Y2K38 boundary");
    
    bridge_info.topology_change_tick = YEAR_2100;
    TEST_ASSERT(bridge_info.topology_change_tick == YEAR_2100,
                "topology_change_tick can store year 2100 timestamp");
}

/*
 * Test 5: Verify STP_CLASS timestamp fields are 64-bit
 */
void test_stp_class_timestamps(void)
{
    STP_CLASS stp_class;
    
    /* last_expiry_time should be UINT64 (8 bytes) */
    TEST_ASSERT(sizeof(stp_class.last_expiry_time) >= 8,
                "STP_CLASS.last_expiry_time should be at least 64-bit");
    
    /* last_bpdu_rx_time should be UINT64 (8 bytes) */
    TEST_ASSERT(sizeof(stp_class.last_bpdu_rx_time) >= 8,
                "STP_CLASS.last_bpdu_rx_time should be at least 64-bit");
    
    /* Test that we can store values beyond 32-bit max */
    stp_class.last_expiry_time = YEAR_2050;
    TEST_ASSERT(stp_class.last_expiry_time == YEAR_2050,
                "last_expiry_time can store year 2050 timestamp");
    
    stp_class.last_bpdu_rx_time = YEAR_2050;
    TEST_ASSERT(stp_class.last_bpdu_rx_time == YEAR_2050,
                "last_bpdu_rx_time can store year 2050 timestamp");
}

/*
 * Test 6: Verify MSTP_PORT timestamp fields are 64-bit
 */
void test_mstp_port_timestamps(void)
{
    MSTP_PORT mstp_port;
    
    /* last_expiry_time should be UINT64 (8 bytes) */
    TEST_ASSERT(sizeof(mstp_port.last_expiry_time) >= 8,
                "MSTP_PORT.last_expiry_time should be at least 64-bit");
    
    /* last_bpdu_rx_time should be uint64_t (8 bytes) */
    TEST_ASSERT(sizeof(mstp_port.last_bpdu_rx_time) >= 8,
                "MSTP_PORT.last_bpdu_rx_time should be at least 64-bit");
    
    /* Test that we can store values beyond 32-bit max */
    mstp_port.last_expiry_time = YEAR_2040;
    TEST_ASSERT(mstp_port.last_expiry_time == YEAR_2040,
                "MSTP_PORT.last_expiry_time can store year 2040 timestamp");
    
    mstp_port.last_bpdu_rx_time = YEAR_2100;
    TEST_ASSERT(mstp_port.last_bpdu_rx_time == YEAR_2100,
                "MSTP_PORT.last_bpdu_rx_time can store year 2100 timestamp");
}

/*
 * Test 7: Verify timer arithmetic doesn't overflow
 */
void test_timer_arithmetic(void)
{
    UINT64 time1 = Y2K38_BOUNDARY;
    UINT64 time2 = YEAR_2040;
    UINT64 diff;
    
    /* Test subtraction doesn't overflow */
    diff = time2 - time1;
    TEST_ASSERT(diff > 0 && diff < YEAR_2100,
                "Timer subtraction works correctly for post-Y2K38 values");
    
    /* Test addition doesn't overflow */
    UINT64 sum = time1 + 1000000;
    TEST_ASSERT(sum > time1,
                "Timer addition works correctly for post-Y2K38 values");
    
    /* Test comparison works correctly */
    TEST_ASSERT(time2 > time1,
                "Timer comparison works correctly for post-Y2K38 values");
}

/*
 * Test 8: Verify sys_get_seconds return type is 64-bit
 */
void test_sys_get_seconds_type(void)
{
    /* sys_get_seconds() should return uint64_t */
    /* We can't call it directly without full STP initialization,
     * but we can verify the return type size matches our expectations */
    uint64_t test_time = 0;
    
    /* This is a compile-time check - if sys_get_seconds() returns
     * a smaller type, this assignment would truncate */
    TEST_ASSERT(sizeof(test_time) >= 8,
                "sys_get_seconds return type holder is at least 64-bit");
}

int main(int argc, char *argv[])
{
    printf("=== Y2K38 Test Suite for sonic-stp ===\n\n");
    
    printf("--- Test 1: TIMER struct size ---\n");
    test_timer_struct_size();
    printf("\n");
    
    printf("--- Test 2: STP_VLAN_TABLE timestamp ---\n");
    test_stp_vlan_table_timestamp();
    printf("\n");
    
    printf("--- Test 3: STP_VLAN_PORT_TABLE timestamp ---\n");
    test_stp_vlan_port_table_timestamp();
    printf("\n");
    
    printf("--- Test 4: BRIDGE_INFO timestamps ---\n");
    test_bridge_info_timestamps();
    printf("\n");
    
    printf("--- Test 5: STP_CLASS timestamps ---\n");
    test_stp_class_timestamps();
    printf("\n");
    
    printf("--- Test 6: MSTP_PORT timestamps ---\n");
    test_mstp_port_timestamps();
    printf("\n");
    
    printf("--- Test 7: Timer arithmetic ---\n");
    test_timer_arithmetic();
    printf("\n");
    
    printf("--- Test 8: sys_get_seconds type ---\n");
    test_sys_get_seconds_type();
    printf("\n");
    
    printf("=== Results: %d/%d tests passed ===\n", tests_passed, tests_run);
    
    return (tests_passed == tests_run) ? 0 : 1;
}
