#!/bin/sh
rm *.gcda *gcno *gcov *.obj *.dll
rm -rf build/*
scons -c