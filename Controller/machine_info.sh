#!/bin/bash
# get the ip of the machine
ifconfig | grep inet | grep -v 127.0.0.1 | grep -v inet6 | cut -d " " -f12 | cut -d : -f2
# get the load averages
cat /proc/loadavg
# get the cpu idle %age
top -b -n 1 | grep "Cpu(s)" | cut -d " " -f5
