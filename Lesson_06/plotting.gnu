#!/usr/bin/gnuplot

set terminal postscript eps enhanced color font 'Helvetica,10'
set output 'execution.eps'

reference = 'log/reference.log'
execution = 'log/execution.log'

set xlabel "Time (s)"
set ylabel "Values"

plot \
execution using 1:2 with lines title 'execution', \
reference  using 1:2 with lines title 'reference'



