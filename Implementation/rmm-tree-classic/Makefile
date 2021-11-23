all: generate_libs profile_bin gitUpdate clean

gitUpdate:
	git add .
	git commit -m "Atualizando profile - estrutura binároa"
	git push

#Profiles da estrutura binária
findClose_bin:
	g++  profile_findClose.cpp -o profile_findClose ../../libs/lib_rmm_bin.a -std=c++11 -I ~/include -Wall /usr/local/lib/libsdsl.a -lprofiler -O2;\
	CPU_PROFILER=profile_findClose.out ./profile_findClose;\
	google-pprof --callgrind ./profile_findClose profile_findClose.prof  > profile_findClose.callgrind

#Bibliotecas
generate_libs:  readBp llibRmmTreeBin

readBp: 
	cd ../libs;\
	g++ -c ../read_bp/read_bp.cpp -I ~/include

rmmTreeBin: 
	cd ../libs;\
	g++ -c ../rmm-tree-classic/rmM_tree_classic_v3.cpp  -I ~/include  /usr/local/lib/libsdsl.a -lpthread -Wall -g 

libRmmTreeBin: rmmTreeBin readBp
	cd ../libs;\
	ar crv lib_rmm_bin.a rmM_tree_classic_v3.o read_bp.o

clean:
	rm *.o
	rm -f ../libs/*.o
