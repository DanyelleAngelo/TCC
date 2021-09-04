all: generateLibs execTestsBenchmark clean 

gitUpdate:
	git add .
	git commit -m "Resultados obtidos das estruturas"
	git push

generateLibs: testArguments rmmTreeOptimized_v0 libTestRmmTreeOptimized_v0  rmmTreeOptimized_v1 libTestRmmTreeOptimized_v1 rmmTreeClassic libTestRmmTreeClassic 

execTestsBenchmark: bm_kary_rmmTree bm_binary_rmmTree kary_wiki binary_wiki kary_ctree binary_ctree kary_prot binary_prot kary_dna binary_dna

execTestsComparison: binary_compareWithSdsl kary_compareWithSdsl

#--------- comparando as estruturas criadas com a sdsl---------
binary_compareWithSdsl:
	g++ dataset_bin.cpp -o dataset_b lib_test_rmm_classic.a  -I ~/include  /usr/local/lib/libsdsl.a  -lgtest -lpthread -Wall -g
	./dataset_b wiki.par 25000

kary_compareWithSdsl:
	g++ -o dataset_k  dataset_k.cpp  lib_test_rmm_optimized_v0.a  -std=c++11  -I ~/include  /usr/local/lib/libsdsl.a  -lgtest -lpthread -Wall -g
	./dataset_k wiki.par 5000000 16
#	valgrind --leak-check=full -s ./dataset_k wiki.par 30 16


#---------teste simples de benchmark, usando a estrutura kary---------
testBenchmark:
	g++ test_benchmark.cpp -o test_benchmark lib_test_rmm_optimized.a -std=c++11  -I ~/include  /usr/local/lib/libsdsl.a   -isystem benchmark/include   -Lbenchmark/build/src -lbenchmark -pthread -O2
	valgrind --leak-check=full -s ./test_benchmark vector.par 10 4 --benchmark_out=test_benchmark_i10_m4.csv --time-unit=ms --benchmark_out_format=csv

#---------testes mais complexos de benchmark---------
binary_wiki:
	./bm_binary_rmm_tree wiki.par 3000000 --benchmark_out=results/wiki/bin_i3000000.csv --time-unit=ms --benchmark_out_format=csv

kary_wiki:
	for order in 4 8 16  ; do\
		./bm_kary_rmm_tree wiki.par 3000000 $$order --benchmark_out=results/wiki/kary_i3000000_m$$order.csv  --benchmark_out_format=csv ; \
	done

binary_ctree:
	./bm_binary_rmm_tree ctree.par 3000000  --benchmark_out=results/ctree/bin_i3000000.csv --time-unit=ms --benchmark_out_format=csv

kary_ctree:
	for order in 4 8 16  ; do\
		./bm_kary_rmm_tree ctree.par 3000000 $$order --benchmark_out=results/ctree/kary_i3000000_m$$order.csv  --benchmark_out_format=csv ; \
	done

binary_prot:
	./bm_binary_rmm_tree prot.par 3000000  --benchmark_out=results/prot/bin_i3000000.csv --time-unit=ms --benchmark_out_format=csv

kary_prot:
	for order in 4 8 16 ; do\
		./bm_kary_rmm_tree prot.par 3000000 $$order --benchmark_out=results/prot/kary_i3000000_m$$order.csv  --benchmark_out_format=csv ; \
	done

binary_dna:
	./bm_binary_rmm_tree dna.par 3000000  --benchmark_out=results/dna/bin_i3000000.csv --time-unit=ms --benchmark_out_format=csv

kary_dna:
	for order in 4 8 16 ; do\
		./bm_kary_rmm_tree dna.par 3000000 $$order --benchmark_out=results/dna/kary_i3000000_m$$order.csv  --benchmark_out_format=csv ; \
	done

binary_osm:
	./bm_binary_rmm_tree osm.par 3000000  --benchmark_out=results/osm/bin_i3000000.csv --time-unit=ms --benchmark_out_format=csv

kary_osm:
	for order in 4 8 16 ; do\
		./bm_kary_rmm_tree osm.par 3000000 $$order --benchmark_out=results/osm/kary_i3000000_m$$order.csv  --benchmark_out_format=csv ; \
	done


#---------compila os arquivos de testes de benchmark com as suas devidas bibliotecas---------
bm_binary_rmmTree:
	g++ bm_binary_rmm_tree.cpp -o bm_binary_rmm_tree  lib_test_rmm_classic.a  -std=c++11  -I ~/include /usr/local/lib/libsdsl.a -isystem benchmark/include    -Lbenchmark/build/src  -lbenchmark -pthread  -O2

bm_kary_rmmTree:
	g++ bm_kary_rmm_tree.cpp -o bm_kary_rmm_tree lib_test_rmm_optimized_v0.a -std=c++11    -I ~/include /usr/local/lib/libsdsl.a  -isystem benchmark/include   -Lbenchmark/build/src  -lbenchmark -pthread  -O2


#---------gerando arquivos objetos ---------
rmmTreeClassic: 
	g++ -c ../rmm-tree-classic/rmM_tree_classic_v3.cpp -I ~/include  /usr/local/lib/libsdsl.a -lpthread -Wall -g

rmmTreeOptimized_v0: 
	g++ -c ../rmm-tree-optimized/rmM_tree_optimized.cpp  -I ~/include  /usr/local/lib/libsdsl.a -lpthread -Wall -g

rmmTreeOptimized_v1: 
	g++ -c ../rmm-tree-optimized/rmM_tree_optimized1_1.cpp  -I ~/include  /usr/local/lib/libsdsl.a -lpthread -Wall -g

readBp: 
	g++ -c read_bp/read_bp.cpp -I ~/include

testArguments:
	g++ -c generates_arguments.cpp read_bp/read_bp.cpp -I ~/include

#---------gerando biblioteca para testes ---------
libTestRmmTreeOptimized_v0: rmmTreeOptimized_v0 readBp testArguments
	ar crv lib_test_rmm_optimized_v0.a rmM_tree_optimized.o read_bp.o generates_arguments.o

libTestRmmTreeOptimized_v1: rmmTreeOptimized_v1 readBp testArguments
	ar crv lib_test_rmm_optimized_v1.a rmM_tree_optimized1_1.o read_bp.o generates_arguments.o

libTestRmmTreeClassic: rmmTreeClassic readBp testArguments
	ar crv lib_test_rmm_classic.a rmM_tree_classic_v3.o read_bp.o generates_arguments.o

clean:
	rm  *.o
