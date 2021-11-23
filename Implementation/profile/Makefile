CPP_FLAGS= -O2 -g -Wall

all: generate_libs fwdSearch  gitUpdate clean

profile_bin: findClose_bin

profile_kary: findClose_kary0 findClose_kary1

gitUpdate:
	git add .
	git commit -m "Atualizando profile - findclose"
	git push

#Profiles gerais, em um mesmo arquivo
fwdSearch:
	g++  profile_fwdSearch.cpp $(CPP_FLAGS) -o profile_fwdSearch ../libs/lib_rmm_bin.a ../libs/lib_rmm_kary.a ../libs/lib_rmm_kary_v1.a -std=c++11 -I ~/include -Wall /usr/local/lib/libsdsl.a -lprofiler 
	CPU_PROFILER=profile_fwdSearch.out ./profile_fwdSearch
	CPUPROFILER_FREQUENCY=10000	
	google-pprof --callgrind ./profile_fwdSearch profile_fwdSearch.prof  > profile_fwdSearch.callgrind

findClose:
	g++  profile_findClose.cpp $(CPP_FLAGS) -o profile_findClose ../libs/lib_rmm_bin.a ../libs/lib_rmm_kary.a ../libs/lib_rmm_kary_v1.a -std=c++11 -I ~/include -Wall /usr/local/lib/libsdsl.a -lprofiler
	CPU_PROFILER=profile_findClose.out ./profile_findClose
	google-pprof --callgrind ./profile_findClose profile_findClose.prof  > profile_findClose.callgrind

#Bibliotecas
generate_libs:  readBp libRmmTreeKary0 libRmmTreeKary1 libRmmTreeBin

readBp: 
	cd ../libs;\
	g++ -c ../read_bp/read_bp.cpp $(CPP_FLAGS) -I ~/include

rmmTreeBin: 
	cd ../libs;\
	g++ -c ../rmm-tree-classic/rmM_tree_classic_v3.cpp $(CPP_FLAGS) -I ~/include  /usr/local/lib/libsdsl.a -lpthread  

rmmTreeKary0: 
	cd ../libs;\
	g++ -c ../rmm-tree-optimized/rmM_tree_optimized.cpp $(CPP_FLAGS) -I ~/include  /usr/local/lib/libsdsl.a -lpthread  

rmmTreeKary1: 
	cd ../libs;\
	g++ -c ../rmm-tree-optimized/v1.cpp $(CPP_FLAGS)  -I ~/include  /usr/local/lib/libsdsl.a -lpthread 

libRmmTreeKary0: rmmTreeKary0 readBp
	cd ../libs;\
	ar crv lib_rmm_kary.a rmM_tree_optimized.o read_bp.o

libRmmTreeKary1: rmmTreeKary1 readBp
	cd ../libs;\
	ar crv lib_rmm_kary_v1.a v1.o read_bp.o

libRmmTreeBin: rmmTreeBin readBp
	cd ../libs;\
	ar crv lib_rmm_bin.a rmM_tree_classic_v3.o read_bp.o

clean:
	rm *.o
	rm -f ../libs/*.o
