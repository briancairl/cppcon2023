#!/bin/bash

echo "--------------------------------------"
for (( i = 0; i < 6; i++ )); do
  sudo perf stat \
    -r 1 \
    -e cpu_core/cache-references/,cpu_core/cache-misses/,cpu_core/cycles/,cpu_core/instructions/,cpu_core/branches/,cpu_core/branch-misses/,faults,migrations \
    -- \
    sudo nice -n -5 \
    taskset -c 1 \
    ./build/main v$i ~/Downloads/supermarket.json ~/Downloads/supermarket.problems.json ~/Downloads/supermarket.paths.v$i.json
done
echo "--------------------------------------"