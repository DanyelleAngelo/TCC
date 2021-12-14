all: generateLibs perfs clean 

#ações básicas
gitUpdate:
	git add .
	git commit -m "Resultados obtidos das estruturas"
	git push

generateLibs:  readBp libRmmTreeKary libRmmTreeBin

execTestsBenchmark: bm_kary_rmmTree bm_binary_rmmTree kary_wiki binary_wiki kary_ctree binary_ctree kary_prot binary_prot kary_dna binary_dna

execTestsComparison: binary_compareWithSdsl kary_compareWithBin

perfs: perfKary

perfKary:
	g++ perf_kary.cpp -o perf_kary ../libs/lib_rmm_kary.a -std=c++11  -I ~/include  /usr/local/lib/libsdsl.a   -pthread -O2
	./perf_kary vector.par 10 4


#--------- Responsável pela execução dos testes de comparação com os conjuntos de dados reais---------
binary_compareWithSdsl:
	g++ dataset_bin.cpp -o dataset_b ../libs/lib_rmm_bin.a  -I ~/include  /usr/local/lib/libsdsl.a  -lgtest -lpthread -Wall -g
	./dataset_b ../dataset/wiki.par 25000

kary_compareWithBin:
	g++ -o dataset_k  dataset_k.cpp  ../libs/lib_rmm_kary.a ../libs/lib_rmm_bin.a  -std=c++11  -I ~/include  /usr/local/lib/libsdsl.a  -lgtest -lpthread -Wall -g
	./dataset_k ../dataset/wiki.par 20000 4
#	valgrind --leak-check=full -s ./dataset_k wiki.par 30 16


#--------- Teste simples de benchmark ---------
testBenchmark:
	g++ test_benchmark.cpp -o test_benchmark ../libs/lib_rmm_kary.a -std=c++11  -I ~/include  /usr/local/lib/libsdsl.a ~/benchmark/build/src   -isystem benchmark/include   -Lbenchmark/build/src -lbenchmark -pthread -O2
	valgrind --leak-check=full -s ./test_benchmark vector.par 10 4 --benchmark_out=test_benchmark_i10_m4.csv --time-unit=ms --benchmark_out_format=csv


#--------- Testes reais de benchmark ---------
binary_wiki:
	./bm_binary_rmm_tree ../dataset/wiki.par 3000000 --benchmark_out=results/wiki/bin_i3000000.csv --time-unit=ms --benchmark_out_format=csv

kary_wiki:
	for order in 4 8 16  ; do\
		./bm_kary_rmm_tree ../dataset/wiki.par 3000000 $$order --benchmark_out=results/wiki/kary_i3000000_m$$order.csv  --benchmark_out_format=csv ; \
	done

binary_ctree:
	./bm_binary_rmm_tree ../dataset/ctree.par 3000000  --benchmark_out=results/ctree/bin_i3000000.csv --time-unit=ms --benchmark_out_format=csv

kary_ctree:
	for order in 4 8 16  ; do\
		./bm_kary_rmm_tree ../dataset/ctree.par 3000000 $$order --benchmark_out=results/ctree/kary_i3000000_m$$order.csv  --benchmark_out_format=csv ; \
	done

binary_prot:
	./bm_binary_rmm_tree ../daset/prot.par 3000000  --benchmark_out=results/prot/bin_i3000000.csv --time-unit=ms --benchmark_out_format=csv

kary_prot:
	for order in 4 8 16 ; do\
		./bm_kary_rmm_tree ../dataset/prot.par 3000000 $$order --benchmark_out=results/prot/kary_i3000000_m$$order.csv  --benchmark_out_format=csv ; \
	done

binary_dna:
	./bm_binary_rmm_tree ../dataset/dna.par 3000000  --benchmark_out=results/dna/bin_i3000000.csv --time-unit=ms --benchmark_out_format=csv

kary_dna:
	for order in 4 8 16 ; do\
		./bm_kary_rmm_tree ../dataset/dna.par 3000000 $$order --benchmark_out=results/dna/kary_i3000000_m$$order.csv  --benchmark_out_format=csv ; \
	done

binary_osm:
	./bm_binary_rmm_tree ../dataset/osm.par 3000000  --benchmark_out=results/osm/bin_i3000000.csv --time-unit=ms --benchmark_out_format=csv

kary_osm:
	for order in 4 8 16 ; do\
		./bm_kary_rmm_tree ../dataset/osm.par 3000000 $$order --benchmark_out=results/osm/kary_i3000000_m$$order.csv  --benchmark_out_format=csv ; \
	done



#------- Iniciando a construção das bibliotecas -------- 

#---------compila os arquivos de testes de benchmark com as suas devidas bibliotecas---------
bm_binary_rmmTree:
	g++ bm_binary_rmm_tree.cpp -o bm_binary_rmm_tree  ../libs/lib_rmm_bin.a  -std=c++11  -I ~/include /usr/local/lib/libsdsl.a -isystem benchmark/include    -Lbenchmark/build/src  -lbenchmark -pthread  -O2

bm_kary_rmmTree:
	g++ bm_kary_rmm_tree.cpp -o bm_kary_rmm_tree ../libs/lib_rmm_kary.a -std=c++11    -I ~/include /usr/local/lib/libsdsl.a  -isystem benchmark/include   -Lbenchmark/build/src  -lbenchmark -pthread  -O2


#Arquivos objetos
readBp: 
	cd ../libs;\
	g++ -c ../read_bp/read_bp.cpp  ../tests/generates_arguments.cpp -I ~/include 

rmmTreeBin: 
	cd ../libs;\
	g++ -c ../rmm-tree-classic/rmM_tree_classic_v3.cpp  -I ~/include  /usr/local/lib/libsdsl.a -lpthread -Wall -g 

rmmTreeKary: 
	cd ../libs;\
	g++ -c ../rmm-tree-optimized/rmM_tree_optimized.cpp  -I ~/include  /usr/local/lib/libsdsl.a -lpthread -Wall 

#libs
libRmmTreeKary: rmmTreeKary readBp
	cd ../libs;\
	ar crv lib_rmm_kary.a rmM_tree_optimized.o read_bp.o  generates_arguments.o

libRmmTreeBin: rmmTreeBin readBp
	cd ../libs;\
	ar crv lib_rmm_bin.a rmM_tree_classic_v3.o read_bp.o  generates_arguments.o

clean:
	rm  *.o
	rm  ../libs/*.o
