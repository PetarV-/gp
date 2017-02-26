OBJS = build/gp.o build/gauss_multi.o build/demo.o

CC = clang++ -Iinclude/
CFLAGS = -std=c++11 -O3 -Wall -Wextra -Werror -Weffc++ -Wstrict-aliasing --pedantic -c
LFLAGS = -Wall
MKDIR_P = mkdir -p

bin/demo : $(OBJS)
	$(MKDIR_P) bin/
	$(CC) $(LFLAGS) $(OBJS) -o bin/demo

build/gp.o : src/gp.cpp
	$(MKDIR_P) build/
	$(CC) $(CFLAGS) src/gp.cpp -o build/gp.o

build/gauss_multi.o : src/gauss_multi.cpp
	$(MKDIR_P) build/
	$(CC) $(CFLAGS) src/gauss_multi.cpp -o build/gauss_multi.o

build/demo.o : demo.cpp
	$(MKDIR_P) build/
	$(CC) $(CFLAGS) demo.cpp -o build/demo.o

.PHONY : clean
clean : 
	rm -f build/*.o bin/demo &> /dev/null 