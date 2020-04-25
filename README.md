# Ping pong

This application spawns `num_threads` threads, each sharing a value that should be decreased to zero.
Each thread is responsible for decreasing it when its value modulo `num_threads` is a specific value, depending on the thread.
The application can be run in three modes: `wait`, `spin`, and `atomic`:
 * In mode `wait`, the value is mutex protected and a condition variable is used to let a thread wait until the value has changed by another thread.
 * In mode `spin`, the value is mutex protected and a lock is repeatedly acquired to check if the value has to change.
 * In mode `atomic`, the value is an atomic variable that is checked repeatedly by all threads.

## How to run

    $ ./pingpong wait|spin|atomic

## How to build?

Use cmake and your choice of C++-compiler:

    mkdir build
    CXX=mycompiler cmake ..
    cmake --build .

## Convenience script for building and running

There is a convenience script `buld_and_run.sh` that will try to build the application with `cl`, `clang++` and `g++` using Ninja in both Debug and Release configuration and then run the application in the three modes using the `time` command to perform timing of the application.
