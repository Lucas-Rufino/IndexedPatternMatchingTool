CC=g++
pwd := $(CURDIR)
pathvar := ${PATH}
seqtest := 1

ipmt:
	$(CC) src/main.cpp -o ./bin/ipmt -std=c++11 -O2 && $(CC) src/main.cpp -o ./bin/ipmttimed -std=c++11 -O2  -DTIME_ME_FLAG


