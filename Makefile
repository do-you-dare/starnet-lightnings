all: most_lightnings.txt

pre_process: src/main.o src/lightning.o src/image.o
	g++ -o $@ $^ -std=c++0x -lz `Magick++-config --ldflags --libs`

src/main.o: src/main.cpp
	g++ -o $@ -c `Magick++-config --cxxflags` $^ -std=c++0x

src/lightning.o: src/lightning.cpp
	g++ -o $@ -c `Magick++-config --cxxflags` $^ -std=c++0x

src/image.o: src/image.cpp
	g++ -o $@ -c $^ -std=c++0x `Magick++-config --cxxflags`

post_process: src/city.cpp
	g++ -g -o $@ $^ -std=c++0x -lpthread

ranking: src/most_lightnings.cpp
	g++ -std=c++0x $^ -o $@ -lpthread

p_results.txt: pre_process
	echo "dia, cidade, lat, lon" > p_results.txt
	ls data/*.gz | parallel ./pre_process >> p_results.txt

most_lightnings.txt: ranking results.txt
	./ranking results.txt data/todas-cidades-brasil.csv

results.txt: post_process p_results.txt
	./post_process data/todas-cidades-brasil.csv p_results.txt results.txt
	sed -i '1s/^/dia, cidade, raios, dist\n/g' results.txt

clean:
	 rm -rf src/*.o src/*.gch

