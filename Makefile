N := 00 01 02 03
PREFIX = cidade

all: plot

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

results/distribution_data.txt: distribution results/
	echo "day, c1, c2, c3, c4" > $@
	ls data/*.gz | parallel ./distribution >> $@

results/stat_results.txt: dist_statistics results/distribution_data.txt results/
	./$^ > results/stat_results.txt

results/$(PREFIX)00.csv: results/stat_results.txt
	split -l7 -d $< results/$(PREFIX) --additional-suffix=.csv

plots/%.png: results/%.csv
	gnuplot -c scripts/plot_dist.gp $< $@

results/:
	mkdir $@

plots/:
	mkdir $@

compile: distribution dist_statistics
plot: plots/ $(foreach n,$(N),plots/$(PREFIX)$(n).png)

clean:
	 rm -rf src/*.o src/*.gch *.txt
	 rm -f distribution dist_statistics

