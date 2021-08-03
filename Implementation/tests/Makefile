all: kary_benchmark


binary:
	g++ -o test_dataset.bin ../rmm-tree-classic/rmM_tree_classic_v3.cpp read_bp/read_bp.cpp test_dataset.cpp  -I ~/include  /usr/local/lib/libsdsl.a  -lgtest -lpthread -Wall -g 
	./test_dataset.bin wiki.par 

kary:
	g++ -o test_dataset_k.bin ../rmm-tree-optimized/rmM_tree_optimized.cpp read_bp/read_bp.cpp test_dataset_k.cpp  -I ~/include  /usr/local/lib/libsdsl.a  -lgtest -lpthread -Wall  
	./test_dataset_k.bin wiki.par 

binary_benchmark:
	g++ -o test_binary_rmm_tree.bin  test_binary_rmm_tree.cpp ../rmm-tree-classic/rmM_tree_classic_v3.cpp read_bp/read_bp.cpp -std=c++11  -I ~/include  /usr/local/lib/libsdsl.a  -isystem benchmark/include   -Lbenchmark/build/src -lbenchmark -pthread  -O2 
	./test_binary_rmm_tree.bin --benchmark_out=result_binary_rmm_tree.csv --benchmark_out_format=csv 
 

kary_benchmark:
	g++ -o 1.bin 1.cpp ../rmm-tree-optimized/rmM_tree_optimized.cpp read_bp/read_bp.cpp -std=c++11  -I ~/include  /usr/local/lib/libsdsl.a  -isystem benchmark/include   -Lbenchmark/build/src -lbenchmark -pthread -O2
	./1.bin  --benchmark_out=result_kary_rmm_tree.csv --benchmark_out_format=csv

clean:
	rm *.o
