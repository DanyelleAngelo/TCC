all: profile_bin profile_kary

profile_bin: findClose_bin

profile_kary: findClose_kary

gitUpdate:
	git add .
	git commit
	git push

#Profiles da estrutura binária
findClose_bin:
	cd bin;\
	g++  p_bin_findClose.cpp -o p_bin_findClose ../../libs/lib_rmm_bin.a -std=c++11 -I ~/include -Wall /usr/local/lib/libsdsl.a -lprofiler -O2;\
	CPU_PROFILER=p_bin_findClose.out ./p_bin_findClose;\
	google-pprof --callgrind ./p_bin_findClose p_bin_findClose.prof  > p_bin_findClose.callgrind

#Profiles da estrutura k-ária
findClose_kary:
	cd kary;\
	g++  p_kary_findClose.cpp -o p_kary_findClose  ../../libs/lib_rmm_kary.a -std=c++11 -I ~/include -Wall /usr/local/lib/libsdsl.a -lprofiler -O2;\
	CPU_PROFILER=p_kary_findClose.out ./p_kary_findClose;\
	google-pprof --callgrind ./p_kary_findClose p_kary_findClose.prof  > p_kary_findClose.callgrind;\

#Profiles gerais, em um mesmo arquivo
fwdSearch:
	g++  profile_fwdSearch.cpp -o profile_fwdSearch ../libs/lib_rmm_bin.a ../libs/lib_rmm_kary.a -std=c++11 -I ~/include -Wall /usr/local/lib/libsdsl.a -lprofiler -O2
	CPU_PROFILER=profile_fwdSearch.out ./profile_fwdSearch
	google-pprof --callgrind ./profile_fwdSearch profile_fwdSearch.prof  > profile_fwdSearch.callgrind

findClose:
	g++  profile_findClose.cpp -o profile_findClose ../libs/lib_rmm_bin.a ../libs/lib_rmm_kary.a -std=c++11 -I ~/include -Wall /usr/local/lib/libsdsl.a -lprofiler -O2
	CPU_PROFILER=profile_findClose.out ./profile_findClose
	google-pprof --callgrind ./profile_findClose profile_findClose.prof  > profile_findClose.callgrind

#Bibliotecas
generate_libs:  readBp libRmmTreeKary libRmmTreeBin

readBp: 
	cd ../../libs;\
	g++ -c ../read_bp/read_bp.cpp -I ~/include 

rmmTreeBin: 
	cd ../libs;\
	g++ -c ../rmm-tree-classic/rmM_tree_classic_v3.cpp  -I ~/include  /usr/local/lib/libsdsl.a -lpthread -Wall -g 

rmmTreeKary: 
	cd ../libs;\
	g++ -c ../rmm-tree-optimized/rmM_tree_optimized.cpp  -I ~/include  /usr/local/lib/libsdsl.a -lpthread -Wall 

libRmmTreeKary: rmmTreeKary readBp
	cd ../libs;\
	ar crv lib_rmm_kary.a rmM_tree_optimized.o read_bp.o

libRmmTreeBin: rmmTreeBin readBp
	cd ../libs;\
	ar crv lib_rmm_bin.a rmM_tree_classic_v3.o read_bp.o

clean:
	rm *.o
	rm -f ../libs/*.o
