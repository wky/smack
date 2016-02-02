#!/bin/bash

. /mnt/local/smack-project/smack.environment
host=$(hostname)
nohup ./SMACKBenchServer.py --queue-file queue2 --thread-count 64 --memory-limit 2000 >>data/${host}.log 2>&1&
