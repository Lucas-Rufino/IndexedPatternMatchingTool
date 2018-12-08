CC=g++
pwd := $(CURDIR)
pathvar := ${PATH}
seqtest := 1

ipmt:
	$(CC) src/main.cpp -o ./bin/ipmt -std=c++11 -O2
ipmt2:
	$(CC) src/main.cpp -o ./bin/ipmt2 -std=c++11 -O2 -DtestDefKey


