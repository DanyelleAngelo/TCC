all: fwdSearch  gitUpdate clean

profile_bin: findClose_bin

profile_kary: findClose_kary0 findClose_kary1

gitUpdate:
	git add .
	git commit -m "Atualizando profile - findclose"
	git push

#Profiles da estrutura binária
findClose_bin:
	cd bin;\
	g++  p_bin_findClose.cpp -o p_bin_findClose ../../libs/lib_rmm_bin.a -std=c++11 -I ~/include -Wall /usr/local/lib/libsdsl.a -lprofiler -O2;\
	CPU_PROFILER=p_bin_findClose.out ./p_bin_findClose;\
	google-pprof --callgrind ./p_bin_findClose p_bin_findClose.prof  > p_bin_findClose.callgrind

#Profiles da estrutura k-ária
findClose_kary0:
	cd kary;\
	g++  p_kary_findClose.cpp -o p_kary_findClose  ../../libs/lib_rmm_kary.a -std=c++11 -I ~/include -Wall /usr/local/lib/libsdsl.a -lprofiler -O2;\
	CPU_PROFILER=p_kary_findClose.out ./p_kary_findClose;\
	google-pprof --callgrind ./p_kary_findClose p_kary_findClose.prof  > p_kary_findClose.callgrind;\

findClose_kary1:
	cd kary;\
	g++  p_kary_findClose_v1.cpp -o p_kary_findClose_v1  ../../libs/lib_rmm_kary_v1.a -std=c++11 -I ~/include -Wall /usr/local/lib/libsdsl.a -lprofiler -O2;\
	CPU_PROFILER=p_kary_findClose_v1.out ./p_kary_findClose_v1;\
	google-pprof --callgrind ./p_kary_findClose_v1 p_kary_findClose_v1.prof  > p_kary_findClose_v1.callgrind;\


#Profiles gerais, em um mesmo arquivo
fwdSearch:
	g++  profile_fwdSearch.cpp -o profile_fwdSearch ../libs/lib_rmm_bin.a ../libs/lib_rmm_kary.a ../libs/lib_rmm_kary_v1.a -std=c++11 -I ~/include -Wall /usr/local/lib/libsdsl.a -lprofiler -O2 -g
	CPU_PROFILER=profile_fwdSearch.out ./profile_fwdSearch
	google-pprof --callgrind ./profile_fwdSearch profile_fwdSearch.prof  > profile_fwdSearch.callgrind

findClose:
	g++  profile_findClose.cpp -o profile_findClose ../libs/lib_rmm_bin.a ../libs/lib_rmm_kary.a ../libs/lib_rmm_kary_v1.a -std=c++11 -I ~/include -Wall /usr/local/lib/libsdsl.a -lprofiler -O2
	CPU_PROFILER=profile_findClose.out ./profile_findClose
	google-pprof --callgrind ./profile_findClose profile_findClose.prof  > profile_findClose.callgrind

#Bibliotecas
generate_libs:  readBp libRmmTreeKary0 libRmmTreeKary1 libRmmTreeBin

readBp: 
	cd ../libs;\
	g++ -c ../read_bp/read_bp.cpp -I ~/include

rmmTreeBin: 
	cd ../libs;\
	g++ -c ../rmm-tree-classic/rmM_tree_classic_v3.cpp  -I ~/include  /usr/local/lib/libsdsl.a -lpthread -Wall -g 

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

libRmmTreeBin: rmmTreeBin readBp
	cd ../libs;\
	ar crv lib_rmm_bin.a rmM_tree_classic_v3.o read_bp.o

clean:
	rm *.o
	rm -f ../libs/*.o
