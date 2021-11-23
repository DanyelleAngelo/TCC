all: profile_kary gitUpdate clean

profile_kary: findClose_kary0 findClose_kary1

gitUpdate:
	git add .
	git commit -m "Atualizando profile - estrutura k-ária"
	git push

#Profiles da estrutura k-ária
findClose_kary0:
	g++  profile_findClose.cpp -o profile_findClose  ../libs/lib_rmm_kary.a -std=c++11 -I ~/include -Wall /usr/local/lib/libsdsl.a -lprofiler -O2 -g
	CPU_PROFILER=profile_findClose.out ./profile_findClose
	CPUPROFILER_FREQUENCY=10000	
	google-pprof --callgrind ./profile_findClose profile_findClose.prof  > profile_findClose.callgrind

findClose_kary1:
	g++  profile_findClose_v1.cpp -o profile_findClose_v1  ../libs/lib_rmm_kary_v1.a -std=c++11 -I ~/include -Wall /usr/local/lib/libsdsl.a -lprofiler -O2 -g
	CPU_PROFILER=profile_findClose_v1.out ./profile_findClose_v1
	CPUPROFILER_FREQUENCY=10000	
	google-pprof --callgrind ./profile_findClose_v1 profile_findClose_v1.prof  > profile_findClose_v1.callgrind

#Bibliotecas
generate_libs:  readBp libRmmTreeKary0 libRmmTreeKary1

readBp: 
	cd ../libs;\
	g++ -c ../read_bp/read_bp.cpp -I ~/include

rmmTreeKary0: 
	cd ../libs;\
	g++ -c ../rmm-tree-optimized/rmM_tree_optimized.cpp  -I ~/include  /usr/local/lib/libsdsl.a -lpthread -Wall 

rmmTreeKary1: 
	cd ../libs;\
	g++ -c ../rmm-tree-optimized/v1.cpp  -I ~/include  /usr/local/lib/libsdsl.a -lpthread -Wall 

libRmmTreeKary0: rmmTreeKary0 readBp
	cd ../libs;\
	ar crv lib_rmm_kary.a rmM_tree_optimized.o read_bp.o

libRmmTreeKary1: rmmTreeKary1 readBp
	cd ../libs;\
	ar crv lib_rmm_kary_v1.a v1.o read_bp.o

clean:
	rm *.o
	rm -f ../libs/*.o
