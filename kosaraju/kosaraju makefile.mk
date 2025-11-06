INCLUDE_DIR = headers
SRC_DIR = src
BUILD_DIR = build
TARGET = kosaraju

CC = gcc
CFLAGS = -Wall -Wextra -I$(INCLUDE_DIR)


SRCS = $(SRC_DIR)/main.c $(SRC_DIR)/kosaraju.c
OBJS = $(SRCS:$(SRC_DIR)/%.c=$(BUILD_DIR)/%.o)

.PHONY: all clean run

all: $(BUILD_DIR)/$(TARGET)

$(BUILD_DIR)/$(TARGET): $(OBJS)
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -o $@ $^

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c $(INCLUDE_DIR)/kosaraju.h
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -rf $(BUILD_DIR)

run: $(BUILD_DIR)/$(TARGET)
	@echo "Executando algoritmo..."
	@./$(BUILD_DIR)/$(TARGET) -h
