#!/bin/bash
if [ "$1" = "clean" ]
then
    (make clean && cd Tests/ && make clean)
    exit
fi

(cd ../ && ./format.sh HeliosSoftwareDev)
# (make -j4 && cd Tests/ && make)
