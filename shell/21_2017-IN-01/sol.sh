#!/bin/bash

file_name=$1
str_1=$2
str_2=$3

# -- validation --
if [[ -z $1 || -z $2 || -z $3 ]]; then
	echo "command usage: [FILE_NAME] [STR1] [STR2]"
	exit 1
fi

if [[ ! -e $1 ]]; then
	echo "command usage: [FILE_NAME] [STR1] [STR2]"
	exit 1
fi

# -- action --

str_1_val=$(grep $str_1= $file_name | cut -d"=" -f2)
str_2_val=$(grep $str_2= $file_name | cut -d"=" -f2)

#prepare for comm, as comm expects files
# -- v1 use temp files --
#$(echo $str_1_val | tr " " "\n" | sort > tempFile1) 
#$(echo $str_2_val | tr " " "\n" | sort > tempFile2)

# -- v2 fake file --

unique_in_str_2=$(comm -1 -3 <(echo $str_1_val | tr " " "\n" | sort) <(echo $str_2_val | tr " " "\n" | sort) | xargs)

echo ">>> unique values in str2 $unique_in_str_2"

$(sed -i "s/$str_2=.*/$str_2=$unique_in_str_2/" $file_name) 
exit 0
