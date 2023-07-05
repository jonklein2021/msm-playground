#!/bin/bash
touch tests.out
rm -f tests.out 2>&1 >/dev/null
make -f makefile clean
make
passed=0
rc=$?
if [[ $rc == 0 ]]; then
    ./build/bin/test | tee -a tests.out
fi