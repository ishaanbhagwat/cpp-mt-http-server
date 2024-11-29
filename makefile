# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++17 -Wall -pthread

# Target executable name
TARGET = http_server

# Source files and object files
SRCS = src/main.cpp src/HttpResponse.cpp src/ThreadPool.cpp
OBJS = $(SRCS:.cpp=.o)

# Default target
all: $(TARGET)

# Rule to build the target
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJS)

# Rule to compile .cpp files into .o files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean up build artifacts
clean:
	rm -f $(OBJS) $(TARGET) *.o src/*.o