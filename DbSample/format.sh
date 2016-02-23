#!/bin/bash

CMD=`command clang-format`
if [ $? -ne 0 ] ; then
    CMD=`command -v clang-format-3.5`
fi
echo $CMD

DIR=$(dirname $0)
echo $DIR
find $DIR \( -name "*.cpp" -o -name "*.h" -o -name "*.hpp" \) | grep -v "build/" | xargs -Iarg $CMD -i arg
