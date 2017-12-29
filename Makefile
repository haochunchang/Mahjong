CC=g++
OUTPUT=main
SOURCES=$(wildcard module/*.cpp)

all:
	$(CC) -std=c++11 $(SOURCES) ./main.cpp -o $(OUTPUT)
clean:
	rm $(OUTPUT)
