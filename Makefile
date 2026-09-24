# CodeTime Compiler Makefile

CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -D_POSIX_C_SOURCE=200809L -I./compiler/include
DEBUG_FLAGS = -g -O0
RELEASE_FLAGS = -O2
SRC_DIR = compiler/src
BUILD_DIR = build
BIN_DIR = bin
INSTALL_PREFIX = /usr/local

# Source files
LEXER_SOURCES = $(wildcard $(SRC_DIR)/lexer/*.c)
PARSER_SOURCES = $(wildcard $(SRC_DIR)/parser/*.c)
AST_SOURCES = $(wildcard $(SRC_DIR)/ast/*.c)
SEMANTIC_SOURCES = $(wildcard $(SRC_DIR)/semantic/*.c)
TYPES_SOURCES = $(wildcard $(SRC_DIR)/types/*.c)
IR_SOURCES = $(wildcard $(SRC_DIR)/ir/*.c)
CODEGEN_SOURCES = $(wildcard $(SRC_DIR)/codegen/*.c)
DIAGNOSTICS_SOURCES = $(wildcard $(SRC_DIR)/diagnostics/*.c)
RUNTIME_SOURCES = $(wildcard $(SRC_DIR)/runtime/*.c)
COMPILER_SOURCES = $(SRC_DIR)/compiler.c
TOKEN_SOURCES = $(SRC_DIR)/token.c

MAIN_SOURCE = $(SRC_DIR)/main.c

ALL_SOURCES = $(LEXER_SOURCES) $(PARSER_SOURCES) $(AST_SOURCES) \
              $(SEMANTIC_SOURCES) $(TYPES_SOURCES) $(IR_SOURCES) \
              $(CODEGEN_SOURCES) $(DIAGNOSTICS_SOURCES) $(RUNTIME_SOURCES) \
              $(COMPILER_SOURCES) $(TOKEN_SOURCES) $(MAIN_SOURCE)

# Object files
OBJECTS = $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(ALL_SOURCES))
OBJECTS += $(BUILD_DIR)/compiler.o
OBJECTS += $(BUILD_DIR)/token.o

# Target executable
TARGET = $(BIN_DIR)/codetime

.PHONY: all clean install uninstall test

all: $(TARGET)

$(TARGET): $(OBJECTS)
	@mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) $(RELEASE_FLAGS) $^ -o $@

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(RELEASE_FLAGS) -c $< -o $@

$(BUILD_DIR)/compiler.o: $(SRC_DIR)/compiler.c
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) $(RELEASE_FLAGS) -c $< -o $@

$(BUILD_DIR)/token.o: $(SRC_DIR)/token.c
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) $(RELEASE_FLAGS) -c $< -o $@

debug: CFLAGS += $(DEBUG_FLAGS)
debug: clean $(TARGET)

clean:
	rm -rf $(BUILD_DIR) $(BIN_DIR)

install: $(TARGET)
	install -d $(INSTALL_PREFIX)/bin
	install -m 755 $(TARGET) $(INSTALL_PREFIX)/bin/codetime

uninstall:
	rm -f $(INSTALL_PREFIX)/bin/codetime

test:
	@echo "Running tests..."
	@cd tests && ./run_tests.sh

.PHONY: help
help:
	@echo "CodeTime Compiler"
	@echo ""
	@echo "Targets:"
	@echo "  all      - Build the compiler (default)"
	@echo "  debug    - Build with debug symbols"
	@echo "  clean    - Remove build artifacts"
	@echo "  install  - Install codetime to $(INSTALL_PREFIX)/bin"
	@echo "  uninstall- Remove codetime from $(INSTALL_PREFIX)/bin"
	@echo "  test     - Run test suite"
	@echo "  help     - Show this help message"
