
# Compiler
CXX = g++
CXXFLAGS = -Wall -std=c++20 -I/opt/homebrew/include
LDFLAGS = -L/opt/homebrew/lib -lraylib -framework OpenGL -framework Cocoa -framework IOKit -framework CoreVideo

# Source and target definitions
SRCS = main.cpp Map.cpp
OBJS = $(SRCS:.cpp=.o)
TARGET = game

# Default target: compile the game
all: $(TARGET)

# Link object files into the final executable
$(TARGET): $(OBJS)
	$(CXX) $(OBJS) -o $(TARGET) $(LDFLAGS)

# Compile .cpp files to .o files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Run the executable
run: $(TARGET)
	./$(TARGET)

# Clean up build files
clean:
	rm -f $(OBJS) $(TARGET)



