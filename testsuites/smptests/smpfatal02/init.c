/*
 * Copyright (c) 2014, 2016 embedded brains GmbH.  All rights reserved.
 *
 *  embedded brains GmbH
 *  Dornierstr. 4
 *  82178 Puchheim
 *  Germany
 *  <rtems@embedded-brains.de>
 *
 * The license and distribution terms for this file may be
 * found in the file LICENSE in this distribution or at
 * http://www.rtems.org/license/LICENSE.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "tmacros.h"

#include <rtems.h>
#include <rtems/score/percpu.h>
#include <rtems/score/smpimpl.h>
#include <rtems/score/smpbarrier.h>

#include <assert.h>
#include <stdlib.h>

const char rtems_test_name[] = "SMPFATAL 2";

#define MAX_CPUS 32

static uint32_t main_cpu;

static SMP_barrier_Control barrier = SMP_BARRIER_CONTROL_INITIALIZER;

static void Init(rtems_task_argument arg)
{
  assert(0);
}

static void fatal_extension(
  rtems_fatal_source source,
  bool always_set_to_false,
  rtems_fatal_code code
)
{
  SMP_barrier_State barrier_state = SMP_BARRIER_STATE_INITIALIZER;
  uint32_t self = rtems_scheduler_get_processor();
  uint32_t cpu_count = rtems_scheduler_get_processor_maximum();

  assert(!always_set_to_false);

  if ( source == RTEMS_FATAL_SOURCE_APPLICATION ) {
    uint32_t cpu;

    assert(self == main_cpu);
    assert(code == 0xdeadbeef);

    _SMP_Request_shutdown();
    _SMP_barrier_Wait(&barrier, &barrier_state, cpu_count);

    for (cpu = 0; cpu < cpu_count; ++cpu) {
      const Per_CPU_Control *per_cpu = _Per_CPU_Get_by_index( cpu );
      Per_CPU_State state = _Per_CPU_Get_state(per_cpu);

      assert(state == PER_CPU_STATE_SHUTDOWN);
    }

    for (cpu = cpu_count; cpu < MAX_CPUS; ++cpu) {
      const Per_CPU_Control *per_cpu = _Per_CPU_Get_by_index( cpu );
      Per_CPU_State state = _Per_CPU_Get_state(per_cpu);

      assert(state == PER_CPU_STATE_INITIAL);
    }

    TEST_END();
  } else if ( source == RTEMS_FATAL_SOURCE_SMP ) {
    assert(self != main_cpu);
    assert(code == SMP_FATAL_SHUTDOWN_RESPONSE);
    _SMP_barrier_Wait(&barrier, &barrier_state, cpu_count);
    _SMP_barrier_Wait(&barrier, &barrier_state, cpu_count);
  }
}

static rtems_status_code test_driver_init(
  rtems_device_major_number major,
  rtems_device_minor_number minor,
  void *arg
)
{
  uint32_t self = rtems_scheduler_get_processor();
  uint32_t cpu_count = rtems_scheduler_get_processor_maximum();
  uint32_t cpu;

  TEST_BEGIN();

  assert(rtems_configuration_get_maximum_processors() == MAX_CPUS);

  main_cpu = self;

  for (cpu = 0; cpu < MAX_CPUS; ++cpu) {
    const Per_CPU_Control *per_cpu = _Per_CPU_Get_by_index( cpu );
    Per_CPU_State state = _Per_CPU_Get_state(per_cpu);

    if (cpu == self) {
      assert(state == PER_CPU_STATE_INITIAL);
    } else if (cpu < cpu_count) {
      assert(
        state == PER_CPU_STATE_INITIAL
          || state == PER_CPU_STATE_READY_TO_START_MULTITASKING
      );
    } else {
      assert(state == PER_CPU_STATE_INITIAL);
    }
  }

  if (cpu_count > 1) {
    rtems_fatal(RTEMS_FATAL_SOURCE_APPLICATION, 0xdeadbeef);
  } else {
    TEST_END();
    exit(0);
  }

  return RTEMS_SUCCESSFUL;
}

#define CONFIGURE_APPLICATION_DOES_NOT_NEED_CLOCK_DRIVER
#define CONFIGURE_APPLICATION_NEEDS_SIMPLE_CONSOLE_DRIVER

#define CONFIGURE_APPLICATION_EXTRA_DRIVERS \
  { .initialization_entry = test_driver_init }

#define CONFIGURE_INITIAL_EXTENSIONS \
  { .fatal = fatal_extension }, \
  RTEMS_TEST_INITIAL_EXTENSION

#define CONFIGURE_MAXIMUM_PROCESSORS MAX_CPUS

#define CONFIGURE_MAXIMUM_TASKS 1

#define CONFIGURE_RTEMS_INIT_TASKS_TABLE

#define CONFIGURE_INIT

#include <rtems/confdefs.h>
