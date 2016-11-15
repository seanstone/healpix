INC = -I ../vector -I ./include
EM_FLAGS = --std=c++14 -s ASYNCIFY=1

.PHONY: all
all:
	em++ $(EM_FLAGS) $(INC) -O2 src/main.cpp -o main.html

.PHONY: release
release:
	em++ $(EM_FLAGS) $(INC) -O2 --closure 1 src/main.cpp -o main.js

.PHONY: debug
debug:
	em++ $(EM_FLAGS) $(INC) src/main.cpp -o main.js

.PHONY: clean
clean:
	rm -f *.js *.mem
