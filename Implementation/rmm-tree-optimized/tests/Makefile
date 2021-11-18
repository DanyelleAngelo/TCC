all: generate_libs v0

v0:
	g++ -o rmMTreeTeste Test-rmMTreeOptimized.cpp  ../../libs/lib_rmm_bin.a ../../libs/lib_rmm_kary.a -I ~/include  /usr/local/lib/libsdsl.a -lgtest -lpthread -Wall 
#	valgrind --leak-check=full -s ./rmMTreeTeste
	./rmMTreeTeste

v1:
	g++ -o rmMTreeTeste1_1 ../rmM_tree_optimized1_1.cpp Test-rmMTreeOptimized.cpp  -I ~/include  /usr/local/lib/libsdsl.a -lgtest -lpthread -Wall 
	valgrind --leak-check=full -s ./rmMTreeTeste1_1

#Bibliotecas
generate_libs:  readBp libRmmTreeKary libRmmTreeBin

readBp: 
	cd ../../libs;\
	g++ -c ../read_bp/read_bp.cpp -I ~/include 

rmmTreeBin: 
	cd ../../libs;\
	g++ -c ../rmm-tree-classic/rmM_tree_classic_v3.cpp  -I ~/include  /usr/local/lib/libsdsl.a -lpthread -Wall -g 

rmmTreeKary: 
	cd ../../libs;\
	g++ -c ../rmm-tree-optimized/rmM_tree_optimized.cpp  -I ~/include  /usr/local/lib/libsdsl.a -lpthread -Wall 

libRmmTreeKary: rmmTreeKary readBp
	cd ../../libs;\
	ar crv lib_rmm_kary.a rmM_tree_optimized.o read_bp.o

libRmmTreeBin: rmmTreeBin readBp
	cd ../../libs;\
	ar crv lib_rmm_bin.a rmM_tree_classic_v3.o read_bp.o

clean:
	rm *.o
	rm ../libs/*.o