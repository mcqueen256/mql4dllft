#!/bin/bash
for file in src/*.cpp
do
	f_base=$(basename $file)
	f_name=${f_base%.*}
	f="$f_name.test.cpp"
	gcov -cbf $f
done

for file in include/*.hpp
do
	f_base=$(basename $file)
	f_name=${f_base%.*}
	f="$f_name.test.hpp"
	gcov -cbf $f
done

for file in test/src/*.cpp
do
	f_base=$(basename $file)
	f_name=${f_base%.*}
	f="$f_name.test.cpp"
	gcov -cbf $f
done
