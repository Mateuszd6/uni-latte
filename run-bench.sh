#!/bin/sh

# LD_PRELOAD=../../sandbox/mimalloc/out/release/libmimalloc.so valgrind --tool=callgrind ./latc ./stress-test.lat
GPRO2DOT=/home/mateusz/.local/bin/gprof2dot
$(GPRO2DOT) --format=callgrind --output=out-mimalloc.dot ./callgrind.out.43190
dot -Tsvg out-mimalloc.dot -o graph-mimalloc.svg
