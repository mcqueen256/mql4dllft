#!/bin/sh
scons -c
rm *.gcda *gcno *gcov *.obj *.dll
rm -rf build/*
rm -rf src
rm -rf include
rm -rf test/src