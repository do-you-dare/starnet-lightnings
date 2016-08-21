
all: plots/distribution.pdf

distribution: src/weekly_by_day.o src/lightning.o src/image.o
	g++ -o $@ $^ -std=c++0x -lz `Magick++-config --ldflags --libs`

dist_statistics: src/distribution_statistics.cpp
	g++ -o $@ $< -std=c++0x -lpthread

src/weekly_by_day.o: src/weekly_by_day.cpp
	g++ -o $@ -c `Magick++-config --cxxflags` $^ -std=c++0x

src/lightning.o: src/lightning.cpp
	g++ -o $@ -c `Magick++-config --cxxflags` $^ -std=c++0x

src/image.o: src/image.cpp
	g++ -o $@ -c $^ -std=c++0x `Magick++-config --cxxflags`

distribution_data.txt: distribution
	echo "day, c1, c2, c3, c4" > distribution_data.txt
	ls data/*.gz | head -n 7 | parallel ./distribution >> distribution_data.txt

stat_results.txt: dist_statistics distribution_data.txt results/
	./$^

results/:
	mkdir $@

plots/distribution.pdf: distribution_data.txt scripts/plot_distribution.R
	Rscript scripts/plot_distribution.R

compile: distribution dist_statistics

clean:
	 rm -rf src/*.o src/*.gch *.txt
	 rm -f distribution dist_statistics

