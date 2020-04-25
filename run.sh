#!/bin/bash

echo "wait"
time ./pingpong wait 2 || exit 1
echo
echo "spin"
time ./pingpong spin 2 || exit 1
echo
echo "atomic"
time ./pingpong atomic 2 || exit 1
echo
