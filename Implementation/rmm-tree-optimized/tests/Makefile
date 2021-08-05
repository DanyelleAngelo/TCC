all:  v1

v0:
	g++ -o rmMTreeTeste ../rmM_tree_optimized.cpp Test-rmMTreeOptimized.cpp  -I ~/include  /usr/local/lib/libsdsl.a -lgtest -lpthread -Wall 
	valgrind --leak-check=full -s ./rmMTreeTeste

v1:
	g++ -o rmMTreeTeste1_1 ../rmM_tree_optimized1_1.cpp Test-rmMTreeOptimized.cpp  -I ~/include  /usr/local/lib/libsdsl.a -lgtest -lpthread -Wall 
	valgrind --leak-check=full -s ./rmMTreeTeste1_1

clean:
	rm *.o