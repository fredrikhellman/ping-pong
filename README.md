# Ping pong

This application that spawns two threads, each sharing a state that can be toggled between `false` and `true`.
One thread toggles it to `true` if it is found `false` and the other does the opposite.
A mutex protects the state.
The application can be run in two modes: `wait` and `spin`:
 * In mode `wait`, a condition variable is used to let a thread wait until the state has changed by the other thread.
 * In mode `spin`, the lock is repeatedly acquired to check if the state has to change.
 * In mode `atomic`, the state is an atomic variable that is checked repeatedly by both threads.

## How to run

    $ ./pingpong wait|spin|atomic

## How to build?

Use cmake and your choice of C++-compiler:

    mkdir build
    CXX=mycompiler cmake ..
    cmake --build .

## Convenience script for building and running

There is a convenience script `buld_and_run.sh` that will try to build the application with `cl`, `clang++` and `g++` using Ninja in both Debug and Release configuration and then run the application in the three modes using the `time` command to perform timing of the application.
