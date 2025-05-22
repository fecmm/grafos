CXX = g++
CXXFLAGS = -std=c++11 -Wall -O2
TARGET = kruskal
SRC_DIR = src
BUILD_DIR = build

SRCS = $(SRC_DIR)/kruskal.cpp
OBJS = $(BUILD_DIR)/kruskal.o

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
	./$(BUILD_DIR)/$(TARGET) -f ../../testes/instances/agm_test.dat -s

.PHONY: all clean test