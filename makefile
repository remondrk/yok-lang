CXX=g++
CXXFLAGS=-std=c++17 -Wall -Wextra -pedantic
SRCS=$(wildcard src/*.cpp)
TARGET=bin/yok

all: $(TARGET)

$(TARGET): $(SRCS)
	@mkdir -p bin
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRCS)

run:
	$(TARGET)

clean:
	rm -f $(TARGET)