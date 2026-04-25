CXX = g++
CXXFLAGS = -Iinclude -Wall -std=c++17
LIBS = -lGL -lglfw -lGLEW -lassimp

HEADERS = $(wildcard include/*)
SOURCE = $(wildcard src/*)
TARGET = bin/app

$(TARGET): $(SOURCE) $(HEADERS)
	@mkdir -p bin
	$(CXX) $(CXXFLAGS) $(SOURCE) -o $(TARGET) $(LIBS)

clean:
	rm -f $(TARGET)
