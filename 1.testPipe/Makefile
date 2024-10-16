testpipe: testpipe.cc
# 使用g++编译器编译源文件（testpipe.cc），生成目标文件（testpipe）
    # $@ 表示目标文件名，这里是testpipe
    # $^ 表示所有的依赖文件，这里是testpipe.cc
    # -std=c++11 指定编译标准为C++11
	g++ -o $@ $^ -std=c++11
.PHONY:clean
clean:
	rm -f testpipe