#!/bin/bash

cat /etc/passwd | awk -F ':' '$5 ~ /СИ/ {print $6}' | xargs -I {} stat -c '%Z %n' {} | awk '$1 > 1660313489 && $1 < 1660830683 {print $2 }'| xargs -I {} grep {} /etc/passwd | awk -F "[:,]" '{print $1, $5}' | cut -c2-;

exit 0;
