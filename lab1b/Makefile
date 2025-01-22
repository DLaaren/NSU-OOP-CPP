# Compiler settings
CXX = g++
CXXFLAGS = -std=c++20 -g -Wall -I./gtest/include
LDFLAGS = -pthread

# Google Test Library (adjust path if necessary)
GTEST_LIB = -lgtest -lgtest_main

# Files and targets
SRC_FILE = CircularBuffer.h
TEST_FILE = tests.cpp
EXEC = test_exec

# Default target: build and run tests
all: $(EXEC)
	./$(EXEC)

# Rule to build the test executable
$(EXEC): $(SRC_FILE) $(TEST_FILE)
	$(CXX) $(CXXFLAGS) $(SRC_FILE) $(TEST_FILE) -o $(EXEC) $(LDFLAGS) $(GTEST_LIB)

# Clean the build directory
clean:
	rm -f $(EXEC)

# Test rule (for running tests without building the project)
test: $(EXEC)
	./$(EXEC)

.PHONY: all clean test
