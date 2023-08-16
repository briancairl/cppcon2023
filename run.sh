#!/bin/bash

perf stat -B -e cpu_core/cache-references/,cpu_core/cache-misses/,cpu_core/cycles/,cpu_core/instructions/,cpu_core/branches/,cpu_core/branch-misses/,faults,migrations ./main.out
