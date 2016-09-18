CFLAGS = -std=c++11

all: compile

stat_reader: src/stat_reader.cpp
	g++ -g -o $@ -std=c++11 $^ -lz -pthread

src/%.o: src/%.cpp
	g++ -o $@ -std=c++11 $< -lz -pthread

out.txt: stat_reader
	./$< $(shell ls data/*.gz | head -n 5)

include/csv.h:
	wget https://raw.githubusercontent.com/ben-strasser/fast-cpp-csv-parser/master/csv.h
	mv csv.h $@

include/CImg.h:
	wget https://raw.githubusercontent.com/dtschump/CImg/master/CImg.h
	mv CImg.h $@

dependencies: include/csv.h include/CImg.h

compile: stat_reader
