/*
 *   libdataflow
 *   Copyright (C) 2018
 *
 *   This program is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef DF_AFFINITY_H_
#define DF_AFFINITY_H_

#include <pthread.h>

#ifdef __linux__
  #include <sched.h>

#elif __APPLE__

  #include <cstdint>
  #include <sys/types.h>
  #include <sys/sysctl.h>
  #include <mach/mach_types.h>
  #include <mach/thread_act.h>
  #include <cpuid.h>

  #define SYSCTL_CORE_COUNT "machdep.cpu.core_count"
  
  #define CPUID(INFO, LEAF, SUBLEAF) __cpuid_count(LEAF, SUBLEAF, INFO[0], INFO[1], INFO[2], INFO[3])

  static inline int sched_getcpu() {
        uint32_t CPUInfo[4];
        CPUID(CPUInfo, 1, 0);
        /* CPUInfo[1] is EBX, bits 24-31 are APIC ID */
        if ( (CPUInfo[3] & (1 << 9)) == 0) {
          return -1;  /* no APIC on chip */
        } else {
          return (unsigned)CPUInfo[1] >> 24;
        }
  }

  typedef struct cpu_set {
	uint32_t count;
  } cpu_set_t;

  static inline void CPU_ZERO(cpu_set_t *cs) {
	cs->count = 0;
  }

  static inline void CPU_SET(int num, cpu_set_t *cs) {
       cs->count |= (1 << num); 
  }

  static inline int CPU_ISSET(int num, cpu_set_t *cs) {
	return (cs->count & (1 << num));
  }

  static int sched_getaffinity(pid_t pid, size_t cpu_size, cpu_set_t *cpu_set) {
	int32_t core_count = 0;
	size_t len = sizeof(core_count);
	int ret = sysctlbyname(SYSCTL_CORE_COUNT, &core_count, &len, 0, 0);
	if (ret) {
		return -1;
	}
	cpu_set->count = 0;
	for (int i=0 ; i<core_count ; i++) {
		cpu_set->count |= (1 << i);
	}

	return 0;
  }
 
  static int pthread_setaffinity_np(pthread_t thread, size_t cpu_size, cpu_set_t *cpu_set) {
	thread_port_t mach_thread;
	int core = 0;

	for (core = 0 ; core < 8 * cpu_size ; core++) {
		if (CPU_ISSET(core, cpu_set))
			break;
	}

	thread_affinity_policy_data_t policy = { core };
	mach_thread = pthread_mach_thread_np(thread);
	thread_policy_set(mach_thread, THREAD_AFFINITY_POLICY, 
			(thread_policy_t)&policy, 1);
	return 0;
  }


#endif

#endif /* DF_AFFINITY_H_ */
