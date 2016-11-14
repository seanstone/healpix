.PHONY: all
all:
	em++ -O1 -s NO_FILESYSTEM=1 -s USE_SDL=2 src/main.cpp -o main.js

.PHONY: release
release:
	em++ -O2 --closure 1 -s NO_FILESYSTEM=1 src/main.cpp -o main.js

.PHONY: debug
debug:
	em++ -s NO_FILESYSTEM=1 src/main.cpp -o main.js

.PHONY: clean
clean:
	rm -f *.js *.mem
