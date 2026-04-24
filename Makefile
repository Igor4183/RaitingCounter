CXX = g++-14
CXXFLAGS = -std=c++20 -Wall -Wextra -Iinclude -Iinclude/generator -mmacosx-version-min=26.0

SRC = $(shell find src -name "*.cpp")
OBJ = $(patsubst src/%.cpp, build/%.o, $(SRC))

TARGET = app

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^

build/%.o: src/%.cpp
	mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -rf build $(TARGET)