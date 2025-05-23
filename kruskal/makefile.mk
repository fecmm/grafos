CC = gcc
CFLAGS = -Wall -Wextra -O2
LDFLAGS =

SRC_DIR = src
INCLUDE_DIR = include
BUILD_DIR = build
TARGET = $(BUILD_DIR)/kruskal

SRCS = $(SRC_DIR)/kruskal.c $(SRC_DIR)/main.c
OBJS = $(SRCS:$(SRC_DIR)/%.c=$(BUILD_DIR)/%.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c $(INCLUDE_DIR)/kruskal.h
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -I$(INCLUDE_DIR) -c $< -o $@

clean:
	rm -rf $(BUILD_DIR)

run: $(TARGET)
	@$(TARGET) -h

.PHONY: all clean run