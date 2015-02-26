#!/usr/bin/gnuplot

set terminal postscript eps enhanced color font 'Helvetica,10'
set output 'execution.eps'

execution = 'log/outputs.dat'

set xlabel "Time (s)"
set ylabel "Values"

plot \
execution using 1:3 with lines title 'execution', \
execution  using 1:2 with lines title 'reference'



