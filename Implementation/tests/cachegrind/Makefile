CPP_FLAGS=-std=c++11 -O2 -g -Wall
ITERATIONS=20000

all: wiki gitUpdate dna gitUpdate prot gitUpdate  ctree gitUpdate clean 

gitUpdate:
	git add .
	git commit -m "Novos dados de cache"
	git push

generateLibs:  readBp libRmmTreeKary libRmmTreeBin

wiki:kary_wiki bin_wiki
dna: kary_dna bin_dna
prot: kary_prot bin_prot
ctree: kary_ctree bin_ctree

kary_wiki:
	g++ -o kary kary.cpp  ../../libs/lib_rmm_kary.a -std=c++11  -I ~/include  /usr/local/lib/libsdsl.a  -lgtest -lpthread -Wall -g
	for order in 4 8 16  ; do\
		valgrind --tool=cachegrind ./kary ../../dataset/wiki.par $(ITERATIONS) $$order ; \
		mkdir wiki_$$order ; \
		cp cachegrind* wiki_$$order/ ; \
		rm cachegrind* ; \
	done
	
bin_wiki:
	g++ -o bin bin.cpp  ../../libs/lib_rmm_bin.a -std=c++11  -I ~/include  /usr/local/lib/libsdsl.a  -lgtest -lpthread -Wall -g
	valgrind --tool=cachegrind ./bin ../../dataset/wiki.par $(ITERATIONS)
	mkdir wiki_bin
	cp cachegrind* wiki_bin/
	rm cachegrind*

kary_dna:
	g++ -o kary kary.cpp  ../../libs/lib_rmm_kary.a -std=c++11  -I ~/include  /usr/local/lib/libsdsl.a  -lgtest -lpthread -Wall -g
	for order in 4 8 16  ; do\
		valgrind --tool=cachegrind ./kary ../../dataset/dna.par $(ITERATIONS) $$order ; \
		mkdir dna_$$order ; \
		cp cachegrind* dna_$$order/ ; \
		rm cachegrind* ; \
	done
	
bin_dna:
	g++ -o bin bin.cpp  ../../libs/lib_rmm_bin.a -std=c++11  -I ~/include  /usr/local/lib/libsdsl.a  -lgtest -lpthread -Wall -g
	valgrind --tool=cachegrind ./bin ../../dataset/dna.par $(ITERATIONS)
	mkdir dna_bin
	cp cachegrind dna_bin/
	rm cachegrind*

kary_prot:
	g++ -o kary kary.cpp  ../../libs/lib_rmm_kary.a -std=c++11  -I ~/include  /usr/local/lib/libsdsl.a  -lgtest -lpthread -Wall -g
	for order in 4 8 16  ; do\
		valgrind --tool=cachegrind ./kary ../../dataset/prot.par $(ITERATIONS) $$order ; \
		mkdir prot_$$order ; \
		cp cachegrind* prot_$$order/ ; \
		rm cachegrind* ; \
	done
	
bin_prot:
	g++ -o bin bin.cpp  ../../libs/lib_rmm_bin.a -std=c++11  -I ~/include  /usr/local/lib/libsdsl.a  -lgtest -lpthread -Wall -g
	valgrind --tool=cachegrind ./bin ../../dataset/prot.par $(ITERATIONS)
	mkdir bin_prot
	cp cachegrind* bin_prot/
	rm cachegrind*

kary_ctree:
	g++ -o kary kary.cpp  ../../libs/lib_rmm_kary.a -std=c++11  -I ~/include  /usr/local/lib/libsdsl.a  -lgtest -lpthread -Wall -g
	for order in 4 8 16  ; do\
		valgrind --tool=cachegrind ./kary ../../dataset/ctree.par $(ITERATIONS) $$order ; \
		mkdir ctree_$$order ; \
		cp cachegrind* ctree_$$order/ ; \
		rm cachegrind* ; \
	done
	
bin_ctree:
	g++ -o bin bin.cpp  ../../libs/lib_rmm_bin.a -std=c++11  -I ~/include  /usr/local/lib/libsdsl.a  -lgtest -lpthread -Wall -g
	valgrind --tool=cachegrind ./bin ../../dataset/ctree.par $(ITERATIONS)
	mkdir bin_ctree
	cp cachegrind* bin_ctree/
	rm cachegrind*

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
