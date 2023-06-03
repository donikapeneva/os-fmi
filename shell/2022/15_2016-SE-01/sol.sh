#!/bin/bash

if [ "$#" -ne "1" ] ; then
	echo "Wrong params count";
	exit 1;
fi

if [ ! -d "$1" ]; then
	echo "$1 is not a folder";
	exit 1;
fi

#find $1 -type l -exec file {} \; | grep "broken";


find $1 -xtype l;

exit 0;
