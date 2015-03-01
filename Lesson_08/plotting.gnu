#!/usr/bin/gnuplot

set terminal postscript eps enhanced color font 'Helvetica,10'
set output 'execution.eps'

execution = 'log/execution.log'

set xlabel "x"
set ylabel "y"

plot \
execution [0:360] using 1:3 with lines title 'execution', \
execution [0:360] using 1:2 with lines title 'reference'



