#!/bin/sh

if [ "$1" = "" ]; then
    echo "Script compiles all demos"
    echo "Usage: check_compilation.sh [platform]"
    exit 1
fi

# set -x

for platform in $1
do

    rm -rf ../bld
    for topdir in `cd ../examples && ls -d */*/`
    do
        example=${topdir%?}
        ./build_and_run.sh -p $platform $2 $example
        if [ $? -ne 0 ]; then
            echo "ERROR"
            exit 1
        fi
    done

done

