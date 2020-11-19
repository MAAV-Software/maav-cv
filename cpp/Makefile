CC=g++ -g -Wall -fno-builtin -ldl -I/usr/local/include/opencv4 -I.

# Indicate that these make commands dont rely on any existing files
.PHONY: clean

# List of driver files and their respective object files
BENCH_SOURCES=benchmark.cpp
BENCH_OBJS=${BENCH_SOURCES:.cpp=.o}
DRIVER_OBJS=driver.o

all: drivers benchmark

# Make recipes for camera drivers
chereddy: chereddy/chereddy-driver.o
	mv chereddy-driver.o driver.o
drewskis: drewskis/pipeline.o
	mv pipeline.o driver.o

# Make recipes for camera benchmarker
benchmark: benchmark.cpp
	${CC} -o $@ $^ driver.o

# Clean up generated files
clean:
	rm -f ${DRIVER_OBJS} ${BENCH_OBJS}

# # Generic rules for compiling a source file to an object file
%.o: %.cpp
	${CC} -c $<
%.o: %.cc
	${CC} -c $<
