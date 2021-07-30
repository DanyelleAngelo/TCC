all: kary_benchmark binary_benchmark


binary:
	g++ -o test_dataset.bin ../rmm-tree-classic/rmM_tree_classic_v3.cpp read_bp/read_bp.cpp test_dataset.cpp  -I ~/include  /usr/local/lib/libsdsl.a  -lgtest -lpthread -Wall -g 
	./test_dataset.bin prot.par 

kary:
	g++ -o test_dataset_k.bin ../rmm-tree-optimized/rmM_tree_optimized.cpp read_bp/read_bp.cpp test_dataset_k.cpp  -I ~/include  /usr/local/lib/libsdsl.a  -lgtest -lpthread -Wall  
	./test_dataset_k.bin prot.par 

binary_benchmark:
	g++ test_binary_rmm_tree.cpp ../rmm-tree-classic/rmM_tree_classic_v3.cpp read_bp/read_bp.cpp -std=c++11  -I ~/include  /usr/local/lib/libsdsl.a  -isystem benchmark/include   -Lbenchmark/build/src -lbenchmark -pthread -o test_binary_rmm_tree -O2 
	./test_binary_rmm_tree wiki.par --benchmark_out=result_binary_rmm_tree.csv --benchmark_out_format=csv 
 

kary_benchmark:
	g++ test_kary_rmm_tree.cpp ../rmm-tree-optimized/rmM_tree_optimized.cpp read_bp/read_bp.cpp -std=c++11  -I ~/include  /usr/local/lib/libsdsl.a  -isystem benchmark/include   -Lbenchmark/build/src -lbenchmark -pthread -o test_kary_rmm_tree -O2
	./test_kary_rmm_tree  --benchmark_out=result_kary_rmm_tree.csv --benchmark_out_format=csv

clean:
	rm *.o
