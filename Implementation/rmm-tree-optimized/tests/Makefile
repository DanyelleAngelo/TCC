all: generate_libs v1

v0:
	g++ -o rmMTreeTeste Test-rmMTreeOptimized.cpp  ../../libs/lib_rmm_bin.a ../../libs/lib_rmm_kary.a -I ~/include  /usr/local/lib/libsdsl.a -lgtest -lpthread -Wall 
#	valgrind --leak-check=full -s ./rmMTreeTeste
	./rmMTreeTeste

v1:
	g++ -o test_treev1 test_treev1.cpp  ../../libs/lib_rmm_bin.a ../../libs/lib_rmm_kary_v1.a -I ~/include  /usr/local/lib/libsdsl.a -lgtest -lpthread -Wall 
	./test_treev1

v2:
	g++ -o test_treev2 test_treev2.cpp  ../../libs/lib_rmm_bin.a ../../libs/lib_rmm_kary_v2.a -I ~/include  /usr/local/lib/libsdsl.a -lgtest -lpthread -Wall 
	./test_treev2

#----------------------------Bibliotecas
generate_libs:  readBp libRmmTreeKary_v0 libRmmTreeKary_v1 libRmmTreeKary_v2 libRmmTreeBin

#gera arquivos objetos
readBp: 
	cd ../../libs;\
	g++ -c ../read_bp/read_bp.cpp -I ~/include 

rmmTreeBin: 
	cd ../../libs;\
	g++ -c ../rmm-tree-classic/rmM_tree_classic_v3.cpp  -I ~/include  /usr/local/lib/libsdsl.a -lpthread -Wall -g 

rmmTreeKary_v0: 
	cd ../../libs;\
	g++ -c ../rmm-tree-optimized/rmM_tree_optimized.cpp  -I ~/include  /usr/local/lib/libsdsl.a -lpthread -Wall 

rmmTreeKary_v1: 
	cd ../../libs;\
	g++  -c ../rmm-tree-optimized/v1.cpp  -I ~/include  /usr/local/lib/libsdsl.a -lpthread -Wall

rmmTreeKary_v2: 
	cd ../../libs;\
	g++  -c ../rmm-tree-optimized/v2.cpp  -I ~/include  /usr/local/lib/libsdsl.a -lpthread -Wall

#liga as bibliotecas
libRmmTreeBin: rmmTreeBin readBp
	cd ../../libs;\
	ar crv lib_rmm_bin.a rmM_tree_classic_v3.o read_bp.o

libRmmTreeKary_v0: rmmTreeKary_v0 readBp
	cd ../../libs;\
	ar crv lib_rmm_kary.a rmM_tree_optimized.o read_bp.o

libRmmTreeKary_v1: rmmTreeKary_v1 readBp
	cd ../../libs;\
	ar crv lib_rmm_kary_v1.a v1.o read_bp.o

libRmmTreeKary_v2: rmmTreeKary_v2 readBp
	cd ../../libs;\
	ar crv lib_rmm_kary_v2.a v2.o read_bp.o

clean:
	rm *.o
	rm ../libs/*.o