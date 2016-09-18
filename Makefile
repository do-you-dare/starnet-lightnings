CFLAGS = -std=c++11

all: compile

stat_reader: src/stat_reader.cpp src/city.cpp
	g++ -g -o $@ -std=c++11 $^ -lz -pthread -lm -lX11

# For some strange reason, this doesn't work. Gives a 'no main' error.
src/%.o: src/%.cpp
	g++ -o $@ -std=c++11 $^ -lz -pthread -lm -lX11

out.txt: stat_reader
	./$< $(shell ls data/*.gz | head -n 5)

compile: stat_reader
