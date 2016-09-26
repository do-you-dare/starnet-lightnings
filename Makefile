CFLAGS = -std=c++11

all: compile

stat_reader: src/stat_reader.o src/city.o
	g++ -g -o $@ -std=c++11 $^ -lz -pthread -lm -lX11

src/%.o: src/%.cpp
	g++ -g -o $@ -c -std=c++11 $^ -lz -pthread -lm -lX11

out.txt: stat_reader
	./$< $(shell ls data/*.gz)

test-out.txt: stat_reader
	./$< $(shell ls data/*.gz | head -n 9)

compile: stat_reader
