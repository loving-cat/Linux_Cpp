.PHONY:all
all:testThread testthread

MyPthread:MyPthread.cc
	g++ -o $@ $^ -std=c++11 -lpthread
testThread:testThread.cc
	g++ -o $@ $^ -std=c++11 -lpthread
testthread:testthread.cc
	g++ -o $@ $^ -std=c++11 -lpthread
.PHONY:clean
clean:
	rm -f testThread testthread MyPthread