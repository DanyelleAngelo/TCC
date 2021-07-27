all: binary_benchmark kary_benchmark clean



binary:
	g++ -o test_dataset.bin ../rmm-tree-classic/rmM_tree_classic_v3.cpp read_bp/read_bp.cpp test_dataset.cpp  -I ~/include  /usr/local/lib/libsdsl.a  -lgtest -lpthread -Wall 
	./test_dataset.bin wiki.par

binary_benchmark:
	g++ test_binary_rmm_tree.cpp ../rmm-tree-classic/rmM_tree_classic_v3.cpp read_bp/read_bp.cpp -std=c++11  -I ~/include  /usr/local/lib/libsdsl.a  -isystem benchmark/include   -Lbenchmark/build/src -lbenchmark -pthread -o test_binary_rmm_tree
	./test_binary_rmm_tree --benchmark_out=result_binary_rmm_tree.csv --benchmark_out_format=csv

kary_benchmark:
	g++ test_kary_rmm_tree.cpp ../rmm-tree-optimized/rmM_tree_optimized.cpp read_bp/read_bp.cpp -std=c++11  -I ~/include  /usr/local/lib/libsdsl.a  -isystem benchmark/include   -Lbenchmark/build/src -lbenchmark -pthread -o test_kary_rmm_tree
	./test_kary_rmm_tree --benchmark_out=result_kary_rmm_tree.csv --benchmark_out_format=csv

clean:
	rm *.o
