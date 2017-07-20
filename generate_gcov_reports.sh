#!/bin/bash
for file in src/*.cpp
do
	f_base=$(basename $file)
	f_name=${f_base%.*}
	f="$f_name.test.cpp"
	gcov -cbf $f
done
