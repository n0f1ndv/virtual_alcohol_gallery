LIBS = -lGL -lglfw -lGLEW

# Headers = 
SOURCE = src/main.cpp

app: $(SOURCE) # $(Headers)
	g++ -o bin/app $(SOURCE) $(LIBS)