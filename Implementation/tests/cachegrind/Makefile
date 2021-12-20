CPP_FLAGS=-std=c++11 -O2 -g -Wall
ITERATIONS=25000

all: generateLibs wiki gitUpdate prot gitUpdate dna gitUpdate ctree gitUpdate clean 

gitUpdate:
	git add .
	git commit -m "Novos dados de cache"
	git push

generateLibs:  readBp libRmmTreeKary libRmmTreeBin

wiki:
	g++ -o rmm rmm.cpp  ../../libs/lib_rmm_kary.a ../../libs/lib_rmm_bin.a -std=c++11  -I ~/include  /usr/local/lib/libsdsl.a  -lgtest -lpthread -Wall -g
	valgrind --tool=cachegrind ./rmm ../../dataset/wiki.par $(ITERATIONS)

prot:
	g++ -o rmm rmm.cpp  ../../libs/lib_rmm_kary.a ../../libs/lib_rmm_bin.a -std=c++11  -I ~/include  /usr/local/lib/libsdsl.a  -lgtest -lpthread -Wall -g
	valgrind --tool=cachegrind ./rmm ../../dataset/prot.par $(ITERATIONS)

dna:
	g++ -o rmm rmm.cpp  ../../libs/lib_rmm_kary.a ../../libs/lib_rmm_bin.a -std=c++11  -I ~/include  /usr/local/lib/libsdsl.a  -lgtest -lpthread -Wall -g
	valgrind --tool=cachegrind ./rmm ../../dataset/dna.par $(ITERATIONS)

ctree:
	g++ -o rmm rmm.cpp  ../../libs/lib_rmm_kary.a ../../libs/lib_rmm_bin.a -std=c++11  -I ~/include  /usr/local/lib/libsdsl.a  -lgtest -lpthread -Wall -g
	valgrind --tool=cachegrind ./rmm ../../dataset/ctree.par $(ITERATIONS)

#Arquivos objetos
readBp: 
	cd ../../libs;\
	g++ -c ../read_bp/read_bp.cpp  ../tests/generates_arguments.cpp -I ~/include 

rmmTreeBin: 
	cd ../../libs;\
	g++ -c ../rmm-tree-classic/rmM_tree_classic_v3.cpp  -I ~/include  /usr/local/lib/libsdsl.a -lpthread -Wall -g 
rmmTreeKary: 
	cd ../../libs;\
	g++ -c ../rmm-tree-optimized/rmM_tree_optimized.cpp  -I ~/include  /usr/local/lib/libsdsl.a -lpthread -Wall 

#libs
libRmmTreeKary: rmmTreeKary readBp
	cd ../../libs;\
	ar crv lib_rmm_kary.a rmM_tree_optimized.o read_bp.o  generates_arguments.o

libRmmTreeBin: rmmTreeBin readBp
	cd ../../libs;\
	ar crv lib_rmm_bin.a rmM_tree_classic_v3.o read_bp.o  generates_arguments.o

clean:
	rm  *.o
	rm  ../../libs/*.o
