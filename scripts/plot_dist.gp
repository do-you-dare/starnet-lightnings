# TODO: set plot title

# Sets output to a png image
set term png font "Helvetica"

# output file name
set output ARG2

set boxwidth 0.5

set style fill solid 1.0 border 0
set style histogram errorbars

plot ARG1 using 2:3: xtic(1) with histogram
