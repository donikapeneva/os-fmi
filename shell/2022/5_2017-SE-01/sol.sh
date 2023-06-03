#!/bin/bash

find . -printf '%n %p\n'| sort -k1 -nr | head -n5 | cut -d " " -f2 ;

exit 0;
