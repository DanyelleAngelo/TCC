all: wiki dna prot ctree clean
#dna prot ctree clean

wiki:
	python3 graph.py ../results/wiki 1000000

dna:
	python3 graph.py ../results/dna 100000000

prot:
	python3 graph.py ../results/prot 3000000

ctree:
	python3 graph.py ../results/ctree 1000000

clean:
	rm -rf __pycache__

