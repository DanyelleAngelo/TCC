all: wiki dna prot ctree clean

wiki:2
	python3 graph.py ../results/wiki 3000000

dna:
	python3 graph.py ../results/dna 3000000

prot:
	python3 graph.py ../results/prot 3000000

ctree:
	python3 graph.py ../results/ctree 3000000


clean:
	rm -rf __pycache__

