#!/bin/bash

echo "wait"
time ./pingpong wait || exit 1
echo
echo "spin"
time ./pingpong spin || exit 1
echo
