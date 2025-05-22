CXX = g++
CXXFLAGS = -std=c++11 -Wall -O2
TARGET = prim
SRC_DIR = src
BUILD_DIR = build

SRCS = $(SRC_DIR)/prim.cpp
OBJS = $(BUILD_DIR)/prim.o

all: $(BUILD_DIR)/$(TARGET)

$(BUILD_DIR)/$(TARGET): $(OBJS)
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) $^ -o $@

$(BUILD_DIR)/%.o: $(SRC_DIR)/%..cpp
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -rf $(BUILD_DIR)

test: all
	./$(BUILD_DIR)/$(TARGET) -f ../../testes/instances/agm_test.dat -i 1 -s

.PHONY: all clean test