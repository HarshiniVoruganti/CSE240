make clean
clear
make
bunzip2 -kc ../traces/int_1.bz2 | ./predictor --gshare:18
#bunzip2 -kc ../traces/fp_1.bz2 | ./predictor --tournament:14:12:14
#bunzip2 -kc ../traces/fp_2.bz2 | ./predictor --tournament:14:12:14
#bunzip2 -kc ../traces/int_1.bz2 | ./predictor --tournament:14:12:14
#bunzip2 -kc ../traces/int_2.bz2 | ./predictor --tournament:14:12:14
#bunzip2 -kc ../traces/mm_1.bz2 | ./predictor --tournament:14:12:14
#bunzip2 -kc ../traces/mm_2.bz2 | ./predictor --tournament:14:12:14
