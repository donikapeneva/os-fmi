#!/bin/bash

cat /etc/passwd | awk -F "[:,]" '$9 ~ /СИ/ {print $1,$5,$10}' | cut -c2- | sort -k1 -n | cut -f2- -d " "  ; 

exit 0;
