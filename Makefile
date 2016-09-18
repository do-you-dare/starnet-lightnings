CFLAGS = -std=c++11

all: compile

stat_reader: src/stat_reader.o src/city.o
	g++ -g -o $@ -std=c++11 $^ -lz -pthread -lm -lX11

src/%.o: src/%.cpp
	g++ -o $@ -c -std=c++11 $^ -lz -pthread -lm -lX11

out.txt: stat_reader
	./$< $(shell ls data/*.gz | head -n 5)

compile: stat_reader
