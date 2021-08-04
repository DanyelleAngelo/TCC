#ao executar cada teste, forneça na seguinte ordem os argumentos: iterations (refere-se ao numero de testes para cada função), ordem da árvore
all: testBenchmark clean

#--------- gerando bibliotecas necessárias---------
rmmTreeClassic:
	g++ -c ../rmm-tree-classic/rmM_tree_classic_v3.cpp -I ~/include  /usr/local/lib/libsdsl.a -lpthread -Wall -g 
	ar crv lib_rmmTreeClassic.a rmM_tree_classic_v3.o

rmmTreeOptimized:
	g++ -c ../rmm-tree-optimized/rmM_tree_optimized.cpp  -I ~/include  /usr/local/lib/libsdsl.a -lpthread -Wall -g
	ar crv lib_rmmTreeOptimized.a rmM_tree_optimized.o

readBp:
	g++ -c read_bp/read_bp.cpp -I ~/include  /usr/local/lib/libsdsl.a
	ar crv lib_read_bp.a read_bp.o

#---------testes comparando as estruturas criadas com a sdsl---------
binary_compareWithSdsl: lib_rmmTreeClassic.a lib_read_bp.a
	g++ dataset_bin.cpp -o dataset_bin lib_read_bp.a lib_rmmTreeClassic.a  -I ~/include   -lgtest  
	./dataset_bin wiki.par 

kary_comparWithSdsl: lib_rmmTreeOptimized.a lib_read_bp.a
	g++ dataset_k.cpp -o dataset_k  lib_read_bp.a lib_rmmTreeOptimized.a  -I ~/include   -lgtest  
	./dataset_k wiki.par 

#---------teste simples de benchmark, usando a estrutura kary---------
testBenchmark:  lib_rmmTreeOptimized.a lib_read_bp.a
	g++ test_benchmark.cpp -o test_benchmark lib_read_bp.a lib_rmmTreeOptimized.a  -std=c++11  -I ~/include  /usr/local/lib/libsdsl.a   -isystem benchmark/include   -Lbenchmark/build/src -lbenchmark -pthread -O2
	./test_benchmark vector.par 10 4 4  --benchmark_out= test_benchmark_10testes_m4_b4.csv --benchmark_out_format=csv


#---------testes mais complexos de benchmark---------

binary_wiki: lib_rmmTreeClassic.a lib_read_bp.a
	g++ bm_binary_rmm_tree.cpp -o bm_binary_rmm_tree lib_read_bp.a lib_rmmTreeClassic.a  read_bp/read_bp.cpp -std=c++11  -I ~/include   -isystem benchmark/include    -Lbenchmark/build/src  -lbenchmark -pthread  -O2 
	./bm_binary_rmm_tree wiki.par 500000 16 4 --benchmark_out=results/wiki/bin_i500000_m4_b16.csv --benchmark_out_format=csv
	./bm_binary_rmm_tree wiki.par 500000 16 8 --benchmark_out=results/wiki/bin_i500000_m8_b16.csv --benchmark_out_format=csv  
 	./bm_binary_rmm_tree wiki.par 500000 16 16 --benchmark_out=results/wiki/bin_i500000_m16_b16.csv --benchmark_out_format=csv 

kary_wiki: lib_rmmTreeOptimized.a lib_read_bp.a
	g++ bm_kary_rmm_tree.cpp -o bm_kary_rmm_tree lib_read_bp.a lib_rmmTreeOptimized.a  read_bp/read_bp.cpp -std=c++11    -I ~/include     -isystem benchmark/include   -Lbenchmark/build/src  -lbenchmark -pthread  -O2 
	./bm_kary_rmm_tree wiki.par 500000 16 4 --benchmark_out=results/wiki/kary_i500000_m4_b16.csv --benchmark_out=result_kary_rmm_tree.csv --benchmark_out_format=csv
	./bm_kary_rmm_tree wiki.par 500000 16 8 --benchmark_out=results/wiki/kary_i500000_m8_b16.csv --benchmark_out=result_kary_rmm_tree.csv --benchmark_out_format=csv 
	./bm_kary_rmm_tree wiki.par 500000 16 16 --benchmark_out=results/wiki/kary_i500000_m16_b16.csv --benchmark_out=result_kary_rmm_tree.csv --benchmark_out_format=csv  


binary_cstree: lib_rmmTreeClassic.a lib_read_bp.a
	g++ bm_binary_rmm_tree.cpp -o bm_binary_rmm_tree lib_read_bp.a lib_rmmTreeClassic.a  read_bp/read_bp.cpp -std=c++11  -I ~/include   -isystem benchmark/include    -Lbenchmark/build/src  -lbenchmark -pthread  -O2 
	./bm_binary_rmm_tree cstree.par 100000 16 4 --benchmark_out=results/cstree/bin_i100000_m4_b16.csv --benchmark_out_format=csv 
 	./bm_binary_rmm_tree cstree.par 100000 16 8 --benchmark_out=results/cstree/bin_i100000_m8_b16.csv --benchmark_out_format=csv 
 	./bm_binary_rmm_tree cstree.par 100000 16 16 --benchmark_out=results/cstree/bin_i100000_m16_b16.csv --benchmark_out_format=csv 
 
kary_cstree: lib_rmmTreeOptimized.a lib_read_bp.a
	g++ bm_kary_rmm_tree.cpp -o bm_kary_rmm_tree lib_read_bp.a lib_rmmTreeOptimized.a  read_bp/read_bp.cpp -std=c++11    -I ~/include     -isystem benchmark/include   -Lbenchmark/build/src  -lbenchmark -pthread  -O2 
	./bm_kary_rmm_tree cstree.par 100000 16 4 --benchmark_out=results/cstree/kary_i100000_m4_b16.csv --benchmark_out=result_kary_rmm_tree.csv --benchmark_out_format=csv 
	./bm_kary_rmm_tree cstree.par 100000 16 8 --benchmark_out=results/cstree/kary_i100000_m8_b16.csv --benchmark_out=result_kary_rmm_tree.csv --benchmark_out_format=csv 
	./bm_kary_rmm_tree cstree.par 100000 16 16 --benchmark_out=results/cstree/kary_i300000_m16_b16.csv --benchmark_out=result_kary_rmm_tree.csv --benchmark_out_format=csv 

binary_prot: lib_rmmTreeClassic.a lib_read_bp.a
	g++ bm_binary_rmm_tree.cpp -o bm_binary_rmm_tree lib_read_bp.a lib_rmmTreeClassic.a  read_bp/read_bp.cpp -std=c++11  -I ~/include   -isystem benchmark/include    -Lbenchmark/build/src  -lbenchmark -pthread  -O2 
	./bm_binary_rmm_tree prot.par 300000 16 4 --benchmark_out=results/prot/bin_i300000_m4_b16.csv --benchmark_out_format=csv
	./bm_binary_rmm_tree prot.par 300000 16 8 --benchmark_out=results/prot/bin_i300000_m8_b16.csv --benchmark_out_format=csv 
	./bm_binary_rmm_tree prot.par 300000 16 16 --benchmark_out=results/prot/bin_i300000_m16_b16.csv --benchmark_out_format=csv  
 

kary_prot: lib_rmmTreeOptimized.a lib_read_bp.a
	g++ bm_kary_rmm_tree.cpp -o bm_kary_rmm_tree lib_read_bp.a lib_rmmTreeOptimized.a  read_bp/read_bp.cpp -std=c++11    -I ~/include     -isystem benchmark/include   -Lbenchmark/build/src  -lbenchmark -pthread  -O2 
	./bm_kary_rmm_tree prot.par 300000 16 4 --benchmark_out=results/prot/kary_i300000_m4_b16.csv --benchmark_out=result_kary_rmm_tree.csv --benchmark_out_format=csv
	./bm_kary_rmm_tree prot.par 300000 16 8 --benchmark_out=results/prot/kary_i300000_m8_b16.csv --benchmark_out=result_kary_rmm_tree.csv --benchmark_out_format=csv 
	./bm_kary_rmm_tree prot.par 300000 16 16 --benchmark_out=results/prot/kary_i300000_m16_b16.csv --benchmark_out=result_kary_rmm_tree.csv --benchmark_out_format=csv  

binary_dna: lib_rmmTreeClassic.a lib_read_bp.a
	g++ bm_binary_rmm_tree.cpp -o bm_binary_rmm_tree lib_read_bp.a lib_rmmTreeClassic.a  read_bp/read_bp.cpp -std=c++11  -I ~/include   -isystem benchmark/include    -Lbenchmark/build/src  -lbenchmark -pthread  -O2 
	./bm_binary_rmm_tree dna.par 1000000 16 4 --benchmark_out=results/dna/bin_i1000000_m4_b16.csv --benchmark_out_format=csv 
	./bm_binary_rmm_tree dna.par 1000000 16 8 --benchmark_out=results/dna/bin_i1000000_m8_b16.csv --benchmark_out_format=csv 
	./bm_binary_rmm_tree dna.par 1000000 16 16 --benchmark_out=results/dna/bin_i1000000_m16_b16.csv --benchmark_out_format=csv 
 

kary_dna: lib_rmmTreeOptimized.a lib_read_bp.a
	g++ bm_kary_rmm_tree.cpp -o bm_kary_rmm_tree lib_read_bp.a lib_rmmTreeOptimized.a  read_bp/read_bp.cpp -std=c++11    -I ~/include     -isystem benchmark/include   -Lbenchmark/build/src  -lbenchmark -pthread  -O2 
	./bm_kary_rmm_tree dna.par 1000000 16 4 --benchmark_out=results/dna/kary_i1000000_m4_b16.csv --benchmark_out=result_kary_rmm_tree.csv --benchmark_out_format=csv
	./bm_kary_rmm_tree dna.par 1000000 16 8 --benchmark_out=results/dna/kary_i1000000_m8_b16.csv --benchmark_out=result_kary_rmm_tree.csv --benchmark_out_format=csv  
	./bm_kary_rmm_tree dna.par 1000000 16 16 --benchmark_out=results/dna/kary_i300000_m16_b16.csv --benchmark_out=result_kary_rmm_tree.csv --benchmark_out_format=csv 


binary_osm: lib_rmmTreeClassic.a lib_read_bp.a
	g++ bm_binary_rmm_tree.cpp -o bm_binary_rmm_tree lib_read_bp.a lib_rmmTreeClassic.a  read_bp/read_bp.cpp -std=c++11  -I ~/include   -isystem benchmark/include    -Lbenchmark/build/src  -lbenchmark -pthread  -O2 
	./bm_binary_rmm_tree osm.par 2000000 16 4 --benchmark_out=results/osm/bin_i2000000_m4_b16.csv --benchmark_out_format=csv 
 	./bm_binary_rmm_tree osm.par 2000000 16 8 --benchmark_out=results/osm/bin_i2000000_m8_b16.csv --benchmark_out_format=csv 
 	./bm_binary_rmm_tree osm.par 2000000 16 16 --benchmark_out=results/osm/bin_i2000000_m16_b16.csv --benchmark_out_format=csv 
 

kary_osm: lib_rmmTreeOptimized.a lib_read_bp.a
	g++ bm_kary_rmm_tree.cpp -o bm_kary_rmm_tree lib_read_bp.a lib_rmmTreeOptimized.a  read_bp/read_bp.cpp -std=c++11    -I ~/include     -isystem benchmark/include   -Lbenchmark/build/src  -lbenchmark -pthread  -O2 
	./bm_kary_rmm_tree osm.par 2000000 16 4 --benchmark_out=results/cstree/kary_i2000000_m4_b16.csv --benchmark_out=result_kary_rmm_tree.csv --benchmark_out_format=csv 
	./bm_kary_rmm_tree osm.par 2000000 16 8 --benchmark_out=results/cstree/kary_i2000000_m8_b16.csv --benchmark_out=result_kary_rmm_tree.csv --benchmark_out_format=csv 
	./bm_kary_rmm_tree osm.par 2000000 16 16 --benchmark_out=results/cstree/kary_i2000000_m16_b16.csv --benchmark_out=result_kary_rmm_tree.csv --benchmark_out_format=csv 



clean:
	rm  *.o
