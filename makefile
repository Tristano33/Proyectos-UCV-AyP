CXX = g++
CXXFLAGS = -Wall -std=c++17 -O2

TARGET = bin/output/programa

SRC = main.cpp

.PHONY: compile test clean

compile: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRC)

test: $(TARGET)
	bash test.sh

clean:
	rm -f $(TARGET) partida.sav
	rm -rf .pruebas
