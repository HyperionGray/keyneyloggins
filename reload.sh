#!/bin/bash

SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"

sudo rmmod rootkit 2>/dev/null
sudo insmod "${SCRIPT_DIR}/src/rootkit.ko"
dmesg | tail -20
