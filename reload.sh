#!/bin/bash

sudo rmmod rootkit.ko
sudo insmod rootkit
dmesg
