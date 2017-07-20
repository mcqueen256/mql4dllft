#!/bin/sh
rm *.gcda *gcno *gcov
rm -rf build/*
scons -c