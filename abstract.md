# Title

Cache-friendly design and robot path planning


# Abstract

Point-to-point planning is a major component in virtually all mobile robotic systems. These systems generally require agents to generate paths over very large areas on demand; and sometimes several more times during their journey as part of intelligent recovery routines to cope with highly dynamic environmental conditions. Since planning over large areas can be a resource-intensive operation, particularly in higher-dimensional state-spaces, planning routines must be carefully tuned to execute as efficiently as possible so as to prevent perceptible delays in robot responsiveness.

Attendees can expect an overview of common search-based planning algorithms used in robotics, such as Dijkstra's and A-star, to provide a motivating context. Rather than focus on tricks specific to a particular environment or robot morphology, we will instead focus on getting the most out of these algorithms by way of leveraging cache-friendly design of requisite graph and memoization structures; and memory layouts. In particular, we will focus on achieving these results using common C++ STL facilities; and compile-time features to promote genericness and design clarity without loss of program optimality, where possible. Finally, we will provide comparisons between naive and cache-optimized implementations; and guides for effectively profiling these algorithms for system-specific tuning purposes.

# Note:
- https://easyperf.net/blog/2019/08/02/Perf-measurement-environment-on-Linux#4-set-cpu-affinity
- https://developers.redhat.com/blog/2014/03/10/determining-whether-an-application-has-poor-cache-performance-2

# Hotspot

/home/brian/dev/cppcon2023/build/run_demo
/home/brian/Downloads/BeanCoDistributionFacilities.graph.json /home/brian/Downloads/BeanCoDistributionFacilities.results.json 0.00001 10
/home/brian/dev/cppcon2023/build
instructions,cpu-cycles,longest_lat_cache.miss,l1d.hwpf_miss,l2_request.miss


# Running

```
./extract.py ~/Downloads/BeanCoDistributionFacilities.png ~/Downloads/BeanCoDistributionFacilities.graph.json -n 2 -s -b 20
```

```
./ui.py ~/Downloads/BeanCoDistributionFacilities.graph.json -d v2
```