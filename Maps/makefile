# Simple Makefile for compiling main.cpp and Map.cpp

CXX = g++
CXXFLAGS = -Wall -std=c++20 -I/opt/homebrew/include
LDFLAGS = -L/opt/homebrew/lib -lraylib -framework OpenGL -framework Cocoa -framework IOKit -framework CoreVideo

TARGET = game

all: $(TARGET)

$(TARGET): main.o Map.o
	$(CXX) main.o Map.o -o $(TARGET) $(LDFLAGS)

main.o: main.cpp Map.h
	$(CXX) $(CXXFLAGS) -c main.cpp

Map.o: Map.cpp Map.h
	$(CXX) $(CXXFLAGS) -c Map.cpp

clean:
	rm -f *.o $(TARGET)

run: $(TARGET)
	./$(TARGET)
