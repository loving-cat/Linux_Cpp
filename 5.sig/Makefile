.PHONY:all
all:mykill myprocess

myprocess:process.cc
	g++ -o $@ $^ -std=c++

mykill:testsig.cc
	g++ -o $@ $^ -std=c++11
.PHONY:clean
clean:
	rm -rf mykill myprocess

