all:
	g++ -o test_dataset.bin ../rmm-tree-classic/rmM_tree_classic_v3.cpp read_bp/read_bp.cpp test_dataset.cpp  -I ~/include  /usr/local/lib/libsdsl.a -lgtest -lpthread -Wall
	./test_dataset.bin read_bp/test_input.txt
	
clean:
	rm *.o
