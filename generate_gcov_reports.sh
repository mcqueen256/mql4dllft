#!/bin/bash
for file in src/*.cpp
do
	f_base=$(basename $file)
	echo "f_base: $f_base"
	f_name=${f_base%.*}
	echo "f_name: $f_name"
	f="$f_name.test.cpp"
	echo "f: $f"
	gcov -cbf $f
done
