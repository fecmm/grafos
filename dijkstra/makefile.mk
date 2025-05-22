CXX = g++
CXXFLAGS = -std=c++11 -Wall -O2
TARGET = dijkstra
SRC_DIR = src
BUILD_DIR = build
SRCS = $(SRC_DIR)/dijkstra.cpp
OBJS = $(BUILD_DIR)/dijkstra.o

all: $(BUILD_DIR)/$(TARGET)

$(BUILD_DIR)/$(TARGET): $(OBJS)
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) $^ -o $@

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -rf $(BUILD_DIR)

test: all
	./$(BUILD_DIR)/$(TARGET) -f ../bat1/instances/sp_test.dat -i 1

.PHONY: all clean test