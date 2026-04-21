CXX = g++
CXXFLAGS = -Iinclude -Wall -Wextra -std=c++17
LIBS = -lGL -lglfw -lGLEW

HEADERS = include/window.hpp
SOURCE = src/main.cpp src/window.cpp
TARGET = bin/app

$(TARGET): $(SOURCE) $(HEADERS)
	@mkdir -p bin
	$(CXX) $(CXXFLAGS) $(SOURCE) -o $(TARGET) $(LIBS)

clean:
	rm -f $(TARGET)
