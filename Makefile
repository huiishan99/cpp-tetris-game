CXX ?= g++
CXXFLAGS ?= -std=c++17 -Wall -Wextra -O2
TARGET ?= main.exe
SOURCES := $(wildcard src/*.cpp)

.PHONY: all clean run

all: $(TARGET)

$(TARGET): $(SOURCES)
	$(CXX) $(CXXFLAGS) $(SOURCES) -o $(TARGET)

run: $(TARGET)
	./$(TARGET)

clean:
	del /Q $(TARGET) 2>NUL || exit 0
