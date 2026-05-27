CXX = g++-14
CC = gcc-14

CXXFLAGS = -std=c++20 -Wall -Wextra -Iinclude -Iinclude/generator -Iinclude/parser -mmacosx-version-min=26.0
CFLAGS = -Wall -Wextra -Iinclude -Iinclude/generator -Iinclude/parser -mmacosx-version-min=26.0

CPP_SRC = $(shell find src -name "*.cpp")
C_SRC = $(shell find src -name "*.c")

CPP_OBJ = $(patsubst src/%.cpp, build/%.o, $(CPP_SRC))
C_OBJ = $(patsubst src/%.c, build/%.o, $(C_SRC))

TARGET = app
OBJ = $(CPP_OBJ) $(C_OBJ)

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^

build/%.o: src/%.cpp
	mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

build/%.o: src/%.c
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf build $(TARGET)