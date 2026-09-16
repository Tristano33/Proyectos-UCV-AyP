CXX = g++
CXXFLAGS = -Wall -std=c++17 -O2

TARGET = bin/output/programa

DIR_OUTPUT = bin/output

SRC = main.cpp


compile: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRC)