# Name of the final executable file
TARGET = simulation

# Compiler
CC = g++

# Compilation flags
CFLAGS = -Wall -std=c++17

# Linking flags (specify the path to the Simlib library)
LDFLAGS = -lsimlib

# List of files
SOURCES = main.cpp data.cpp generator.cpp student.cpp
HEADERS = data.h generator.h student.h
OBJECTS = $(SOURCES:.cpp=.o)

# Recipe for building the executable file
$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o $(TARGET) $(LDFLAGS)

# Recipe for compiling source files
%.o: %.cpp $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

# Removing temporary files
clean:
	rm -f $(OBJECTS) $(TARGET)
