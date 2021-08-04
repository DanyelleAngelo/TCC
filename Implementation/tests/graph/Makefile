all: wiki dna prot ctree clean

wiki:
	python3 graph.py ../results/wiki 500000

dna:
	python3 graph.py ../results/dna 1000000

prot:
	python3 graph.py ../results/prot 300000

ctree:
	python3 graph.py ../results/ctree 100000

clean:
	rm -rf __pycache__

