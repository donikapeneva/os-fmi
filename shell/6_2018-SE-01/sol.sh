#!/bin/bash

find . -type d 2> /dev/null | xargs -I {} chmod 0755 {};

exit 0;
