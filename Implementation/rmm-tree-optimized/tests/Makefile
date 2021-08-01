all: 
	g++ -o rmMTreeTeste ../rmM_tree_optimized.cpp Test-rmMTreeOptimized.cpp  -I ~/include  /usr/local/lib/libsdsl.a -lgtest -lpthread -Wall 
	valgrind --leak-check=full -s ./rmMTreeTeste

clean:
	rm *.o