#!/bin/bash

CMD=`command -v clang-format`
if [ $? -ne 0 ] ; then
    CMD=`command -v clang-format-3.5`
fi
if [ $? -ne 0 ] ; then
	CMD=`command -v clang-format-3.7`
fi
echo Using command: $CMD

format()
{
	echo "Formatting " $1
	$CMD -i $1
}

DIR=$(dirname $0)
echo $DIR
for i in `find $DIR \( -name "*.cpp" -o -name "*.h" -o -name "*.hpp" \) | grep -v "build/"`
do 
	format $i
done
