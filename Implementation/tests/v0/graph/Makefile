ITERATIONS=3000000

all: wiki dna prot ctree clean

wiki:
	python3 graph.py ../results/wiki $(ITERATIONS)

dna:
	python3 graph.py ../results/dna $(ITERATIONS)

prot:
	python3 graph.py ../results/prot $(ITERATIONS)

ctree:
	python3 graph.py ../results/ctree $(ITERATIONS)


clean:
	rm -rf __pycache__

