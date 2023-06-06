#!/bin/bash

dir=$1 
count_hardlinks=$2

if (( $# == 2 )); then
	find $1 -printf "%n %f path:(%p)\n" | awk -v count=$count_hardlinks '$1>=count { print $1 $2 FS $3; }' 
else
	#find $1 -xtype l
	find $1 -type l -exec test ! -e {} \; -printf "%f\n"
fi


