all: 
	g++ -o rmMTreeTeste ../rmM_tree_classic_v3.cpp Test-rmMtreeClassic.cpp  -I ~/include  /usr/local/lib/libsdsl.a -lgtest -lpthread -Wall 
	valgrind --leak-check=full -s ./rmMTreeTeste
	
clean:
	rm *.o
