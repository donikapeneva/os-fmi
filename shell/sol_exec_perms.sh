#!/bin/bash

find . -maxdepth 2 -name "sol*" -a -name "*.sh"  -type f  -exec chmod 0744 {} \; ;

exit 0;
