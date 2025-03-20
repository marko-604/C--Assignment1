COMPILER = g++
CXXFLAGS = -Wall -std=c++20 -I/opt/homebrew/include
LDFLAGS = -L/opt/homebrew/lib -lraylib -framework OpenGL -framework Cocoa -framework IOKit -framework CoreVideo

SRCS = main.cpp Maps/Map.cpp Towers/Tower.cpp Critters/Critter.cpp Observer/TowerObserver.cpp Observer/Subject.cpp Observer/Observer.cpp Observer/MapObserver.cpp Observer/CritterObserver.cpp  Towers/TowerDecorators/SniperDecorator.cpp Towers/TowerDecorators/BombDecorator.cpp Towers/TowerDecorators/FreezingDecorator.cpp Towers/TowerDecorators/TowerDecorator.cpp


OBJS = $(SRCS:.cpp=.o)
TARGET = game


$(TARGET): $(OBJS)
	$(CXX) $(OBJS) -o $(TARGET) $(LDFLAGS)

# Run the executable
run: $(TARGET)
	./$(TARGET)

# Clean up build files
clean:
	rm -f $(OBJS) $(TARGET)



