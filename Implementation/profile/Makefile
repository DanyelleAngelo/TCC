all: fwdSearch clean
	
fwdSearch: libRmmTreeKary libRmmTreeBin
	g++  profile_fwdSearch.cpp -o profile_fwdSearch lib_rmm_bin.a lib_rmm_kary.a -std=c++11 -I ~/include -Wall /usr/local/lib/libsdsl.a -lprofiler
	CPU_PROFILER=fwdSearch.out ./profile_fwdSearch
	google-pprof --callgrind ./profile_fwdSearch fwdSearch.prof > fwdSearch.callgrind
	kcachegrind fwdSearch.callgrind

#Bibliotecas
readBp: 
	g++ -c ../read_bp/read_bp.cpp -I ~/include

rmmTreeBin: 
	g++ -c ../rmm-tree-classic/rmM_tree_classic_v3.cpp -I ~/include  /usr/local/lib/libsdsl.a -lpthread -Wall -g

rmmTreeKary: 
	g++ -c ../rmm-tree-optimized/rmM_tree_optimized.cpp  -I ~/include  /usr/local/lib/libsdsl.a -lpthread -Wall 

libRmmTreeKary: rmmTreeKary readBp
	ar crv lib_rmm_kary.a rmM_tree_optimized.o read_bp.o

libRmmTreeBin: rmmTreeBin readBp
	ar crv lib_rmm_bin.a rmM_tree_classic_v3.o read_bp.o

clean:
	rm *.o
