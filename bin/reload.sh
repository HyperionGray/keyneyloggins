#!/bin/bash

sudo rmmod rootkit 2>/dev/null
sudo insmod src/rootkit.ko
dmesg | tail -20
