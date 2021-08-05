all: rmmTreeOptimized1_1 bm_kary bm_binary gitUpdate binary_wiki kary_wiki gitUpdate binary_ctree kary_ctree gitUpdate binary_prot kary_prot binary_dna kary_dna gitUpdate
#rmmTreeClassic rmmTreeOptimized bm_binary bm_kary binary_wiki kary_wiki binary_ctree kary_ctree binary_prot kary_prot binary_dna kary_dna binary_osm kary_osm


#--------- gerando bibliotecas necessárias---------
rmmTreeClassic:
	g++ -c ../rmm-tree-classic/rmM_tree_classic_v3.cpp -I ~/include  /usr/local/lib/libsdsl.a -lpthread -Wall -g 
	ar crv lib_rmmTreeClassic.a rmM_tree_classic_v3.o

rmmTreeOptimized:
	g++ -c ../rmm-tree-optimized/rmM_tree_optimized.cpp  -I ~/include  /usr/local/lib/libsdsl.a -lpthread -Wall -g
	ar crv lib_rmmTreeOptimized.a rmM_tree_optimized1_1.o

rmmTreeOptimized1_1:
	g++ -c ../rmm-tree-optimized/rmM_tree_optimized1_1.cpp  -I ~/include  /usr/local/lib/libsdsl.a -lpthread -Wall -g
	ar crv lib_rmmTreeOptimized1_1.a rmM_tree_optimized1_1.o
	
readBp:
	g++ -c read_bp/read_bp.cpp -I ~/include  /usr/local/lib/libsdsl.a
	ar crv lib_read_bp.a read_bp.o


#---------testes comparando as estruturas criadas com a sdsl---------
binary_compareWithSdsl: lib_rmmTreeClassic.a lib_read_bp.a
	g++ dataset_bin.cpp -o dataset_b.bin lib_read_bp.a lib_rmmTreeClassic.a  -I ~/include  /usr/local/lib/libsdsl.a  -lgtest -lpthread -Wall -g
	valgrind --leak-check=full -s ./dataset_b wiki.par 10

kary_comparWithSdsl: lib_rmmTreeOptimized.a lib_read_bp.a
#	g++ dataset_k.cpp -o dataset_k.bin  lib_read_bp.a lib_rmmTreeOptimized.a  -I ~/include /usr/local/lib/libsdsl.a   -lgtest -lpthread -Wall -g 
	g++ -o dataset_k  dataset_k.cpp ../rmm-tree-optimized/rmM_tree_optimized1_1.cpp lib_read_bp.a -std=c++11  -I ~/include  /usr/local/lib/libsdsl.a  -lgtest -lpthread -Wall -g 
	valgrind --leak-check=full -s ./dataset_k wiki.par 10 16


#---------teste simples de benchmark, usando a estrutura kary---------
testBenchmark:  lib_rmmTreeOptimized.a lib_read_bp.a
	g++ test_benchmark.cpp -o test_benchmark lib_read_bp.a lib_rmmTreeOptimized.a  -std=c++11  -I ~/include  /usr/local/lib/libsdsl.a   -isystem benchmark/include   -Lbenchmark/build/src -lbenchmark -pthread -O2
	./test_benchmark vector.par 10 4 --benchmark_out=test_benchmark_i10_m4.csv --time-unit=ms --benchmark_out_format=csv

#---------executa testes para comparar as duas implementações da rmM-tree otimizada---------
kary_1.0:
	g++ test_benchmark.cpp -o test_benchmark lib_read_bp.a lib_rmmTreeOptimized.a  -std=c++11  -I ~/include  /usr/local/lib/libsdsl.a   -isystem benchmark/include   -Lbenchmark/build/src -lbenchmark -pthread -O2
	./test_benchmark vector.par 10 4 --benchmark_out=results/compare_versions_of_kary/kary_rmm_tree_v1_0.csv --time-unit=ms --benchmark_out_format=csv

kary_1.1:
	g++ -o test_benchmark test_benchmark.cpp lib_rmmTreeOptimized1_1.a lib_read_bp.a -std=c++11  -I ~/include  /usr/local/lib/libsdsl.a   -isystem benchmark/include   -Lbenchmark/build/src -lbenchmark -pthread -O2
	./test_benchmark vector.par 10 4 --benchmark_outresults/compare_versions_of_kary/kary_rmm_tree_v1_1.csv --time-unit=ms --benchmark_out_format=csv


#---------compila os arquivos de testes de benchmark com as suas devidas bibliotecas---------
bm_binary: lib_rmmTreeClassic.a lib_read_bp.a
	g++ bm_binary_rmm_tree.cpp -o bm_binary_rmm_tree lib_read_bp.a lib_rmmTreeClassic.a  -std=c++11  -I ~/include /usr/local/lib/libsdsl.a -isystem benchmark/include    -Lbenchmark/build/src  -lbenchmark -pthread  -O2

bm_kary: lib_rmmTreeOptimized1_1.a lib_read_bp.a
	g++ bm_kary_rmm_tree.cpp -o bm_kary_rmm_tree lib_read_bp.a lib_rmmTreeOptimized1_1.a  -std=c++11    -I ~/include /usr/local/lib/libsdsl.a  -isystem benchmark/include   -Lbenchmark/build/src  -lbenchmark -pthread  -O2 


#---------testes mais complexos de benchmark---------
binary_wiki: bm_binary
	./bm_binary_rmm_tree wiki.par 500000 --benchmark_out=results/wiki/bin_i500000.csv --time-unit=ms --benchmark_out_format=csv
	
kary_wiki: bm_kary
	for order in 4 8 16 ; do\
		./bm_kary_rmm_tree wiki.par 500000 $$order --benchmark_out=results/wiki/kary_i500000_m$$order.csv  --benchmark_out_format=csv ; \
	done

binary_ctree: bm_binary
	./bm_binary_rmm_tree ctree.par 100000  --benchmark_out=results/ctree/bin_i100000.csv --time-unit=ms --benchmark_out_format=csv 
 
kary_ctree: bm_kary
	for order in 4 8 16 ; do\
		./bm_kary_rmm_tree ctree.par 100000 $$order --benchmark_out=results/ctree/kary_i100000_m$$order.csv  --benchmark_out_format=csv ; \
	done

binary_prot: bm_binary
	./bm_binary_rmm_tree prot.par 300000  --benchmark_out=results/prot/bin_i300000.csv --time-unit=ms --benchmark_out_format=csv 

kary_prot: bm_kary
	for order in 4 8 16 ; do\
		./bm_kary_rmm_tree prot.par 300000 $$order --benchmark_out=results/prot/kary_i300000_m$$order.csv  --benchmark_out_format=csv ; \
	done

binary_dna: bm_binary
	./bm_binary_rmm_tree dna.par 1000000  --benchmark_out=results/dna/bin_i1000000.csv --time-unit=ms --benchmark_out_format=csv 
 

kary_dna: bm_kary
	for order in 4 8 16 ; do\
		./bm_kary_rmm_tree dna.par 1000000 $$order --benchmark_out=results/dna/kary_i1000000_m$$order.csv  --benchmark_out_format=csv ; \
	done

binary_osm: bm_binary
	./bm_binary_rmm_tree osm.par 10000  --benchmark_out=results/osm/bin_i10000_m4.csv --time-unit=ms --benchmark_out_format=csv

kary_osm: bm_kary
	for order in 4 8 16 ; do\
		./bm_kary_rmm_tree osm.par 10000 $$order --benchmark_out=results/osm/kary_i10000_m$$order.csv  --benchmark_out_format=csv ; \
	done


#Sobe dados para o repositorio
gitUpdate:
	git add .
	git commit -m "Resultados obtidos das estruturas"
	git push

clean:
	rm  *.o
