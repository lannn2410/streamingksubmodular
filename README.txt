This zip includes:
+ Source code of our 2 streaming algorithms, greedy and SGr.
+ Facebook and Sensor dataset (in "data" folder) for testing the algorithms.

Since estimating F in influence maximization is very time consuming, our code uses OpenMP for parallelization (https://en.wikipedia.org/wiki/OpenMP).

To build our code, run:
	g++ -std=c++11 *.cpp -o ksub -DIL_STD -fopenmp -g

After building, to run our code, run:
	./ksub -f <data filename> 
		-V <size of V> 
		-t <type of experiment, 0: influence maximization, 1: sensor placement> 
		-k <value of k> 
		-B <value of B> 
		-M <value of M> 
		-e <value of epsilon>
		-n <value of eta - denoise step for RStream> 
		-g <value of gamma> 
		-a <algorithm, 0: greedy, 1: DStream, 2: RStream, 3: SGr. Please use SSA source code for testing IM algorithm>
		-p <number of threads (OpenMP) to running algorithms>

We conducted experiments on a Linux machine with 2.3Ghz Xeon 18 core processor and 256Gb of RAM. With 70 threads, DStream usually terminates after 20 minutes, RStream is 2 hours.