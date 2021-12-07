CPP_FLAGS= -std=c++11 -O2 -g -Wall
LIBS= -I ~/include -L  ~/lib -lsdsl -ldivsufsort -ldivsufsort64

all: 
	g++ ../rmM_tree_classic_v3.cpp Test-rmMtreeClassic.cpp $(CPP_FLAGS) -o rmMTreeTeste  $(LIBS) -lpthread
	valgrind --leak-check=full -s ./rmMTreeTeste
	
clean:
	rm *.o
	rm ../libs/*.o
