#!/bin/bash

cat /etc/passwd | awk -F '[:,;]' '$1 ~ /s[0-9]+/ && $5 ~ /а$/ && $9 ~ /СИ/ {print $1}' | cut -c3-4 | sort -nr | uniq -c | sort -k1 -nr | head -n1 ;

exit 0;
