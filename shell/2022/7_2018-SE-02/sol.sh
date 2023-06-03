#!/bin/bash

find ~ -type f -printf "%n %p\n" | awk '$1 > 1  { print $2 }' | xargs -I {} stat -c "%Y %i %n"  {} | sort -k1 -nr | awk '{print $2}' | head -n1;

#find ~ -type f -printf "%n %T@ %p\n"


exit 0;
