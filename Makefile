CC=g++
pwd := $(CURDIR)
pathvar := ${PATH}
seqtest := 1

ipmt:
	$(CC) src/main.cpp -o ./bin/ipmt -std=c++14 -O3
ipmt2:
	$(CC) src/main.cpp -o ./bin/ipmt2 -std=c++14 -O3 -DtestDefKey

test:
	make ipmt && make ipmt2 && make test1 && make test1so && make test1wu && make test2 && make test3 && make test4 && make test5 && rm lixo

test1:
	number=0
	time while [[ $$number -le $(seqtest) ]]; \
		do ipmt  -c -p data/pattern.txt data/shakespeare.txt  ; \
		((number = number + 1)); \
	done 

test1so:
	number=0
	time while [[ $$number -le $(seqtest) ]]; \
		do ipmt -a ShiftOr -c -p data/pattern.txt data/shakespeare.txt  ; \
		((number = number + 1)); \
	done 

test1wu:
	number=0
	time while [[ $$number -le $(seqtest) ]]; \
		do ipmt -a WuManber -c -p data/pattern.txt data/shakespeare.txt  ; \
		((number = number + 1)); \
	done 
	
test2:
	number=0
	time while [[ $$number -le $(seqtest) ]]; \
		do ipmt2  -c -p data/pattern.txt data/shakespeare.txt  ; \
		((number = number + 1)); \
	done 

test3:
	number=0
	time while [[ $$number -le $(seqtest) ]]; \
		do grep -c -p data/pattern.txt data/shakespeare.txt  ; \
		((number = number + 1)); \
	done 

test4:
	number=0
	time while [[ $$number -le $(seqtest) ]]; \
		do ggrep -c --file=data/pattern.txt data/shakespeare.txt  ; \
		((number = number + 1)); \
	done 

test5:
	number=0
	time while [[ $$number -le $(seqtest) ]]; \
		do ipmt -c -a AhoCorasik -p data/pattern.txt data/shakespeare.txt  ; \
		((number = number + 1)); \
	done


path: 
	export PATH=$(pathvar):$(pwd)/bin/
