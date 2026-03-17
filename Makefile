CC := gcc
CFLAGS := -Wall -Wextra -pedantic -std=c11

SRC_DIR := src
BUILD_DIR := build
TARGET := loc

.PHONY: all build clean

all: build

build: $(BUILD_DIR)/$(TARGET)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(BUILD_DIR)/$(TARGET): | $(BUILD_DIR)
	mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) $(SRC_DIR)/main.c $(SRC_DIR)/fs_walk.c $(SRC_DIR)/loc_counter.c -o $@

clean:
	rm -rf $(BUILD_DIR)

