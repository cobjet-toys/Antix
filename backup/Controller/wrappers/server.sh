#!/bin/bash
# replace hostname with the appropriate command
# for example, ./antix
# use ../.. etc to back up to the approriate folder for the commmand
SERVER_START_COMMAND=hostname

# runs it in the background
# output will go to antix.hostname.out,
# where hostname is the machine hostname
$SERVER_START_COMMAND &
