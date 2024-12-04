CC = gcc
CFLAGS = -Wall -Wextra -g

# Directories and files
SRC = solver.c
BUILD_DIR = ../build
TARGET = $(BUILD_DIR)/solver

# Default target
.PHONY: all
all: $(TARGET) run

# Compile the program
$(TARGET): $(SRC)
	@mkdir -p $(BUILD_DIR) # Ensure build directory exists
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC)
	@echo "Compiled $(TARGET)"

# Run the program
.PHONY: run
run:
	@echo "Running $(TARGET)..."
	$(TARGET)

# Clean build directory
.PHONY: clean
clean:
	@rm -rf $(BUILD_DIR)
	@echo "Cleaned build directory."
