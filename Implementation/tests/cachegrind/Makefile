CPP_FLAGS=-std=c++11 -O2 -g -Wall
ITERATIONS=35000

all: generateLibs wiki gitUpdate dna gitUpdate prot gitUpdate ctree gitUpdate clean 

gitUpdate:
	git add .
	git commit -m "Novos dados de cache"
	git push

generateLibs:  readBp libRmmTreeKary libRmmTreeBin

wiki: kary_wiki bin_wiki
dna: kary_dna bin_dna
prot: kary_prot bin_prot
ctree: kary_ctree bin_ctree

kary_wiki:
	g++ -o kary kary.cpp  ../../libs/lib_rmm_kary.a -std=c++11  -I ~/include  /usr/local/lib/libsdsl.a  -lgtest -lpthread -Wall -g
	for order in 4 8 16  ; do\
		valgrind --tool=cachegrind ./kary ../../dataset/wiki.par $(ITERATIONS) $$order ; \
		cp cachegrind.out.* wiki_kary_$$order.out ; \
		rm cachegrind.out.* ; \
	done
	
bin_wiki:
	g++ -o bin bin.cpp  ../../libs/lib_rmm_bin.a -std=c++11  -I ~/include  /usr/local/lib/libsdsl.a  -lgtest -lpthread -Wall -g
	valgrind --tool=cachegrind ./bin ../../dataset/wiki.par $(ITERATIONS)
	cp cachegrind.out.* wiki_bin.out
	rm cachegrind.out.*

kary_dna:
	g++ -o kary kary.cpp  ../../libs/lib_rmm_kary.a -std=c++11  -I ~/include  /usr/local/lib/libsdsl.a  -lgtest -lpthread -Wall -g
	for order in 4 8 16  ; do\
		valgrind --tool=cachegrind ./kary ../../dataset/dna.par $(ITERATIONS) $$order ; \
		cp cachegrind.out.* dna_kary_$$order.out ; \
		rm cachegrind.out.* ; \
	done
	
bin_dna:
	g++ -o bin bin.cpp  ../../libs/lib_rmm_bin.a -std=c++11  -I ~/include  /usr/local/lib/libsdsl.a  -lgtest -lpthread -Wall -g
	valgrind --tool=cachegrind ./bin ../../dataset/dna.par $(ITERATIONS)
	cp cachegrind.out.* dna_bin.out
	rm cachegrind.out.*

kary_prot:
	g++ -o kary kary.cpp  ../../libs/lib_rmm_kary.a -std=c++11  -I ~/include  /usr/local/lib/libsdsl.a  -lgtest -lpthread -Wall -g
	for order in 4 8 16  ; do\
		valgrind --tool=cachegrind ./kary ../../dataset/prot.par $(ITERATIONS) $$order ; \
		cp cachegrind.out.* prot_kary_$$order.out ; \
		rm cachegrind.out.* ; \
	done
	
bin_prot:
	g++ -o bin bin.cpp  ../../libs/lib_rmm_bin.a -std=c++11  -I ~/include  /usr/local/lib/libsdsl.a  -lgtest -lpthread -Wall -g
	valgrind --tool=cachegrind ./bin ../../dataset/prot.par $(ITERATIONS)
	cp cachegrind.out.* prot_bin.out
	rm cachegrind.out.*

kary_ctree:
	g++ -o kary kary.cpp  ../../libs/lib_rmm_kary.a -std=c++11  -I ~/include  /usr/local/lib/libsdsl.a  -lgtest -lpthread -Wall -g
	for order in 4 8 16  ; do\
		valgrind --tool=cachegrind ./kary ../../dataset/ctree.par $(ITERATIONS) $$order ; \
		cp cachegrind.out.* ctree_kary_$$order.out ; \
		rm cachegrind.out.* ; \
	done
	
bin_ctree:
	g++ -o bin bin.cpp  ../../libs/lib_rmm_bin.a -std=c++11  -I ~/include  /usr/local/lib/libsdsl.a  -lgtest -lpthread -Wall -g
	valgrind --tool=cachegrind ./bin ../../dataset/ctree.par $(ITERATIONS)
	cp cachegrind.out.* ctree_bin.out
	rm cachegrind.out.*

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
