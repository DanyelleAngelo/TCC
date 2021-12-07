CPP_FLAGS= -std=c++11  -Wall -O2 -g
LIBS= -I ~/include -L  ~/lib -lsdsl 
MACOS= "/Users/danyelleangelo/IFB/TCC/Implementation/"
MINT= "/media/danyelle/Data/IFB/TCC/Implementation/"

all: generate_libs fwdSearch clean

gitUpdate:
	git add .
	git commit -m "Atualizando profile"
	git push

#Profiles gerais, em um mesmo arquivo
fwdSearch:
	g++  $(CPP_FLAGS)  profile_fwdSearch.cpp -o profile_fwdSearch ../libs/lib_rmm_bin.a ../libs/lib_rmm_kary.a ../libs/lib_rmm_kary_v1.a $(LIBS) -lprofiler 
	CPU_PROFILE=profile_fwdSearch.out ./profile_fwdSearch
	pprof --callgrind ./profile_fwdSearch profile_fwdSearch.prof  > profile_fwdSearch.callgrind

findClose:
	g++  $(CPP_FLAGS) profile_findClose.cpp -o profile_findClose ../libs/lib_rmm_bin.a ../libs/lib_rmm_kary.a ../libs/lib_rmm_kary_v1.a $(LIBS) -lprofiler
	CPU_PROFILER=profile_findClose.out ./profile_findClose
	google-pprof --callgrind ./profile_findClose profile_findClose.prof  > profile_findClose.callgrind

update_path:
	sed -i 's| /home/danyelleangelo/TCC/Implementation/|/media/danyelle/Data/IFB/TCC/Implementation/|g' profile_fwdSearch.callgrind

#Bibliotecas
generate_libs:  readBp libRmmTreeKary0 libRmmTreeKary1 libRmmTreeBin

readBp: 
	cd ../libs;\
	g++ $(CPP_FLAGS) -c ../read_bp/read_bp.cpp  $(LIBS)

rmmTreeBin: 
	cd ../libs;\
	g++ $(CPP_FLAGS) -c ../rmm-tree-classic/rmM_tree_classic_v3.cpp  $(LIBS)

rmmTreeKary0: 
	cd ../libs;\
	g++  $(CPP_FLAGS) -c ../rmm-tree-optimized/rmM_tree_optimized.cpp $(LIBS)

rmmTreeKary1: 
	cd ../libs;\
	g++  $(CPP_FLAGS)  -c ../rmm-tree-optimized/v1.cpp $(LIBS)

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
	rm -f ../libs/*.o
	rm *.o
