#!/bin/bash

if [[ $# != 2 ]]; then
	echo ">>> usage: <dir_to_search> <arch>"
	exit 1
fi

if [[ ! -d $1 ]]; then
	echo ">>> ${1} does not exits"
	exit 1
fi

dir=$1

if [[ -z $2 ]]; then
	echo ">>> Please, pass an arch you want to search. Available archs:"
	#show options
	
	find $dir -maxdepth 1 -regex '.*/vmlinuz-[0-9]+\.[0-9]+\.[0-9]+-.*$' -printf "%f\n" | cut -d"-" -f3 | sort | uniq

	exit 1
fi


arch=$2

versions=$(find $dir -maxdepth 1 -regex ".*/vmlinuz-[0-9]+\.[0-9]+\.[0-9]+-${arch}\$" -printf "%f\n" > temp)

hightest_v=$(cat temp | cut -d"-" -f2 | sort -k 1,3 -t "." -nr | head -n1)

found=$(cat temp | wc -l )

if [[ $found != 0 ]] ; then
	cat temp | grep $hightest_v 
else 
	echo ">>> Sorry, arch ${arch} not found"
fi

rm temp
