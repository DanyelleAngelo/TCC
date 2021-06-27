all: 
	g++ -o rmmTreeTeste.bin ../rmm_tree_classic_v3.cpp Test-RMMtreeClassic.cpp  -I ~/include  /usr/local/lib/libsdsl.a -lgtest -lpthread -Wall 
#	valgrind --leak-check=full -s ./rmmTreeTeste.bin
	./rmmTreeTeste.bin

clean:
	rm *.o