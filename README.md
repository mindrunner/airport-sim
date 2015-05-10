# airport-sim by Lukas Elsner


The task was to develop an airport simulator using C and POSIX threads. The
airport should have one runway, which is the critical resource, that cannot be
used at the same time by multiple threads. The planes should land and take-off
randomly based on given probabilities and being parked in the bay of the
airport, which can hold op to ten planes.


Compile and run:

1. cd src/                  - to change into src directory
2. compile
  1. make -f Makefile.make    - to compile airport-sim with make
  2. or "cmake . && make"        - to compile airport-sim with CMake
3. ./airport-sim -h         - for help
4. ./airport-sim            - for starting with default probability values (50,50)
5. ./airport-sim 30 70      - for starting with custom probability values (30,70)


Run 'doxygen doxy.gen' to regenerate source code documentation.
