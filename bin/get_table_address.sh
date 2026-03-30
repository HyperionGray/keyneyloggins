#!/bin/bash

cat /boot/System.map-$(uname -r)| grep sys_call_table | awk '{print $1}'| head -n 1
