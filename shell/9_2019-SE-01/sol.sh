#!/bin/bash

cat "./planets.txt" | sed '/^name/d' | sort -k3 -t ";" -n 1> sorted.txt;

fardest_type=$( cat sorted.txt| tail -n1 |  cut -f2 -d ";");
closest_object=$(awk -F ";" -v type=${fardest_type} '$2 ~ type {print $1"\t"$4}' sorted.txt | head -n1);

echo $(echo ${closest_object});

exit 0;
