add wave *
add wave -position insertpoint /mult/line__71/number1_tmp
add wave -position insertpoint /mult/line__71/number2_tmp
add wave -position insertpoint /mult/line__71/e
add wave -position insertpoint /mult/line__71/m
add wave -position insertpoint /mult/line__71/m1
add wave -position insertpoint /mult/line__71/m2

force clock 1 20 ns, 0 40 ns -repeat 40
force reset 1 0

force number1_port 0100000001011111110111010111000010100011110101110000101000111101 0
force number2_port 0100000001000101000000000000000000000000000000000000000000000000 0
force numbers_areready 1 0 ns, 0 120 ns

run 10000
