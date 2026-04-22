CXX = g++
CXXFLAGS = -Iinclude -Wall -std=c++17
LIBS = -lGL -lglfw -lGLEW -lassimp

HEADERS = $(wildcard include/*.hpp)
SOURCE = $(wildcard src/*.cpp)
TARGET = bin/app

$(TARGET): $(SOURCE) $(HEADERS)
	@mkdir -p bin
	$(CXX) $(CXXFLAGS) $(SOURCE) -o $(TARGET) $(LIBS)

clean:
	rm -f $(TARGET)
