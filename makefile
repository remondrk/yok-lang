CXX=g++
CXXFLAGS=-std=c++17 -Wall -Wextra -pedantic
SRCS=$(wildcard src/*.cpp)
TARGET=bin/yok

all: $(TARGET)

$(TARGET): $(SRCS)
	@mkdir -p bin
	@mkdir -p src/generated
	python ./scripts/generate-parser-patterns.py ./src/generated/parser_patterns.hpp 
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRCS)

run:
	$(TARGET)

clean:
	rm -f $(TARGET)
