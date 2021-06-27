all: 
	g++ -o rmMTreeTeste.bin ../rmM_tree_classic_v3.cpp Test-RMMtreeClassic.cpp  -I ~/include  /usr/local/lib/libsdsl.a -lgtest -lpthread -Wall 
#	valgrind --leak-check=full -s ./rmMTreeTeste.bin
	./rmMTreeTeste.bin

clean:
	rm *.o