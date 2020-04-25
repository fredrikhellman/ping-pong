#!/bin/bash

compile_and_run () {
    $1 pingpong.cc -o pingpong -lpthread || exit 1
    echo "wait ($1)"
    time ./pingpong wait || exit 1
    echo "spin ($1)"
    time ./pingpong spin || exit 1
}

compile_and_run "g++ -O0"
compile_and_run "g++ -O3"
compile_and_run "clang++ -O0"
compile_and_run "clang++ -O3"

# Results
#
# ➜  ping-pong bash run_linux.sh
# wait (g++ -O0)

# real	0m21,219s
# user	0m3,803s
# sys	0m24,157s
# spin (g++ -O0)

# real	0m4,130s
# user	0m4,480s
# sys	0m3,371s
# wait (g++ -O3)

# real	0m21,530s
# user	0m2,856s
# sys	0m25,218s
# spin (g++ -O3)

# real	0m4,217s
# user	0m3,606s
# sys	0m4,664s
# wait (clang++ -O0)

# real	0m19,778s
# user	0m5,133s
# sys	0m22,812s
# spin (clang++ -O0)

# real	0m4,067s
# user	0m3,279s
# sys	0m4,740s
# wait (clang++ -O3)

# real	0m20,381s
# user	0m2,668s
# sys	0m24,731s
# spin (clang++ -O3)

# real	0m3,891s
# user	0m2,899s
# sys	0m4,823s
