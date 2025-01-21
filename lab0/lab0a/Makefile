# Compiler and flags
CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++20

# Target executable name
TARGET = lab0a

# Source files
SRCS = main.cpp module1.cpp module2.cpp

# Object files (replace .cpp with .o)
OBJS = $(SRCS:.cpp=.o)

# Default rule to build the target
all: $(TARGET)

# Rule to link object files into the executable
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(TARGET)

# Rule to compile source files into object files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean rule to remo
