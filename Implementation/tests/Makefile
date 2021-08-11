all: testBenchmark clean
#binary_wiki kary_wiki binary_ctree kary_ctree binary_prot kary_prot binary_dna kary_dna

#---------testes comparando as estruturas criadas com a sdsl---------
binary_compareWithSdsl: lib_test_rmm_classic.a
	g++ dataset_bin.cpp -o dataset_b lib_test_rmm_classic.a  -I ~/include  /usr/local/lib/libsdsl.a  -lgtest -lpthread -Wall -g
	valgrind --leak-check=full -s ./dataset_b wiki.par 10

kary_comparWithSdsl: lib_test_rmm_optimized.a
	g++ -o dataset_k  dataset_k.cpp  lib_test_rmm_optimized.a  -std=c++11  -I ~/include  /usr/local/lib/libsdsl.a  -lgtest -lpthread -Wall -g 
	valgrind --leak-check=full -s ./dataset_k wiki.par 30 16


#---------teste simples de benchmark, usando a estrutura kary---------
testBenchmark: lib_test_rmm_optimized.a
	g++ test_benchmark.cpp -o test_benchmark lib_test_rmm_optimized.a  -std=c++11  -I ~/include  /usr/local/lib/libsdsl.a   -isystem benchmark/include   -Lbenchmark/build/src -lbenchmark -pthread -O2
	./test_benchmark vector.par 10 4 --benchmark_out=test_benchmark_i10_m4.csv --time-unit=ms --benchmark_out_format=csv


#---------compila os arquivos de testes de benchmark com as suas devidas bibliotecas---------
bm_binary_rmm_tree: lib_test_rmm_classic.a
	g++ bm_binary_rmm_tree.cpp -o bm_binary_rmm_tree  lib_test_rmm_classic.a  -std=c++11  -I ~/include /usr/local/lib/libsdsl.a -isystem benchmark/include    -Lbenchmark/build/src  -lbenchmark -pthread  -O2

bm_kary_rmm_tree: lib_test_rmm_optimized.a
	g++ bm_kary_rmm_tree.cpp -o bm_kary_rmm_tree lib_test_rmm_optimized.a -std=c++11    -I ~/include /usr/local/lib/libsdsl.a  -isystem benchmark/include   -Lbenchmark/build/src  -lbenchmark -pthread  -O2 


#---------testes mais complexos de benchmark---------
binary_wiki: bm_binary_rmm_tree
	./bm_binary_rmm_tree wiki.par 500000 --benchmark_out=results/wiki/bin_i500000.csv --time-unit=ms --benchmark_out_format=csv
	
kary_wiki: bm_kary_rmm_tree
	for order in 4 8 16  ; do\
		./bm_kary_rmm_tree wiki.par 500000 $$order --benchmark_out=results/wiki/kary_i500000_m$$order.csv  --benchmark_out_format=csv ; \
	done

binary_ctree: bm_binary_rmm_tree
	./bm_binary_rmm_tree ctree.par 100000  --benchmark_out=results/ctree/bin_i100000.csv --time-unit=ms --benchmark_out_format=csv 
 
kary_ctree: bm_kary_rmm_tree
	for order in 4 8 16  ; do\
		./bm_kary_rmm_tree ctree.par 100000 $$order --benchmark_out=results/ctree/kary_i100000_m$$order.csv  --benchmark_out_format=csv ; \
	done

binary_prot: bm_binary_rmm_tree
	./bm_binary_rmm_tree prot.par 300000  --benchmark_out=results/prot/bin_i300000.csv --time-unit=ms --benchmark_out_format=csv 

kary_prot: bm_kary_rmm_tree
	for order in 4 8 16 ; do\
		./bm_kary_rmm_tree prot.par 300000 $$order --benchmark_out=results/prot/kary_i300000_m$$order.csv  --benchmark_out_format=csv ; \
	done

binary_dna: bm_binary_rmm_tree
	./bm_binary_rmm_tree dna.par 1000000  --benchmark_out=results/dna/bin_i1000000.csv --time-unit=ms --benchmark_out_format=csv 
 

kary_dna: bm_kary_rmm_tree
	for order in 4 8 16 ; do\
		./bm_kary_rmm_tree dna.par 1000000 $$order --benchmark_out=results/dna/kary_i1000000_m$$order.csv  --benchmark_out_format=csv ; \
	done

#Sobe dados para o repositorio
gitUpdate:
	git add .
	git commit -m "Resultados obtidos das estruturas"
	git push

#--------- Compilando arquivos necessários ---------
rmmTreermM_tree_classic_v3.o: ../rmm-tree-classic/rmM_tree_classic_v3.cpp
	g++ -c ../rmm-tree-classic/rmM_tree_classic_v3.cpp -I ~/include  /usr/local/lib/libsdsl.a -lpthread -Wall -g 

rmM_tree_optimized1_1.o: ../rmm-tree-optimized/rmM_tree_optimized1_1.cpp
	g++ -c ../rmm-tree-optimized/rmM_tree_optimized1_1.cpp  -I ~/include  /usr/local/lib/libsdsl.a -lpthread -Wall -g
	
read_bp.o: read_bp/read_bp.cpp
	g++ -c read_bp/read_bp.cpp -I ~/include  /usr/local/lib/libsdsl.a
	ar crv read_bp/lib_read_bp.a 

generates_arguments.o: generates_arguments.cpp  read_bp/lib_read_bp.a
	g++ -c generates_arguments.cpp read_bp/lib_read_bp.a -I ~/include  /usr/local/lib/libsdsl.a


#---------gerando bibliotecas necessárias para testes --------- 
lib_test_rmm_classic.a: rmM_tree_classic_v3.o read_bp.o generates_arguments.o
	ar crv lib_test_rmm_classic.a rmM_tree_classic_v3.o read_bp.o generates_arguments.o

lib_test_rmm_optimized.a: rmM_tree_optimized1_1.o read_bp.o generates_arguments.o
	ar crv lib_test_rmm_optimized.a rmM_tree_optimized1_1.o read_bp.o generates_arguments.o
	
clean:
	rm  *.o