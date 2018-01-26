#!/bin/bash

# ./log.sh <firstL> <message>

DATE=`date '+%Y-%m-%d %H:%M:%S'`
ENTRY="$1 -- $DATE\n$2\n"

if [ $# -eq 2 ]; then
    if [ ! -e DEVLOG.txt ]; then
        touch DEVLOG.txt
    fi
    echo -e "$ENTRY" >> DEVLOG.txt
else
    echo -e "Incorrect number of arguments supplied\nUsage: $ ./log.sh <firstL> <message>"
fi