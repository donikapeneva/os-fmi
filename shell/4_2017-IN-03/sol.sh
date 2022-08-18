#!/bin/bash

cat /etc/passwd | grep "СИ:"  | cut -f1 -d , |cut -f1,5 -d : | grep "^s[0-9]" | grep "а$" | cut -c3-4 | sort -nr | uniq -c | sort -k1 -nr | head -n1;

exit 0;
