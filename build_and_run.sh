#!/bin/bash

compilers_to_try="cl clang++ g++"
generator=Ninja

for compiler in $compilers_to_try; do
  if [ -x "$(command -v $compiler)" ]; then 
    for config in Debug Release; do
      mkdir -p build-$compiler-$config || exit 1
      (
        cd build-$compiler-$config
        echo "Building $config with $compiler."
	CXX=$compiler cmake .. -G$generator "-DCMAKE_INSTALL_PREFIX=$(pwd)/install" -DCMAKE_BUILD_TYPE=$config || exit 1	
        cmake --build . --target install --config $config || exit 1
        echo "Running executable built in $config with $compiler."
        bash install/run.sh || exit 1
      )
    done
  fi
done
