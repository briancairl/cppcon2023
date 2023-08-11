#!/bin/bash

echo "DEFAULT" && ./build.sh && ./run.sh && \
echo "AERNA"   && ./build.sh -DUSE_ARENA && ./run.sh