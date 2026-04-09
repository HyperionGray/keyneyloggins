#!/bin/bash

SCRIPT_DIR="$(dirname "$(readlink -f "$0")")" 
PROJECT_DIR="$(dirname "$SCRIPT_DIR")"

sudo rmmod rootkit 2>/dev/null
sudo insmod "$PROJECT_DIR/build/rootkit.ko"
dmesg | tail -20
