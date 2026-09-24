cmake_minimum_required(VERSION 3.10)
project(CodeTime VERSION 1.0.0 LANGUAGES C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra")

# Compiler directories
set(COMPILER_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/compiler/include")
set(COMPILER_SRC_DIR "${CMAKE_SOURCE_DIR}/compiler/src")

# Include directories
include_directories(${COMPILER_INCLUDE_DIR})

# Collect source files
file(GLOB LEXER_SOURCES "${COMPILER_SRC_DIR}/lexer/*.c")
file(GLOB PARSER_SOURCES "${COMPILER_SRC_DIR}/parser/*.c")
file(GLOB AST_SOURCES "${COMPILER_SRC_DIR}/ast/*.c")
file(GLOB SEMANTIC_SOURCES "${COMPILER_SRC_DIR}/semantic/*.c")
file(GLOB TYPES_SOURCES "${COMPILER_SRC_DIR}/types/*.c")
file(GLOB IR_SOURCES "${COMPILER_SRC_DIR}/ir/*.c")
file(GLOB CODEGEN_SOURCES "${COMPILER_SRC_DIR}/codegen/*.c")
file(GLOB DIAGNOSTICS_SOURCES "${COMPILER_SRC_DIR}/diagnostics/*.c")
file(GLOB RUNTIME_SOURCES "${COMPILER_SRC_DIR}/runtime/*.c")

set(MAIN_SOURCE "${COMPILER_SRC_DIR}/main.c")

# Create executable
add_executable(codetime
    ${MAIN_SOURCE}
    ${LEXER_SOURCES}
    ${PARSER_SOURCES}
    ${AST_SOURCES}
    ${SEMANTIC_SOURCES}
    ${TYPES_SOURCES}
    ${IR_SOURCES}
    ${CODEGEN_SOURCES}
    ${DIAGNOSTICS_SOURCES}
    ${RUNTIME_SOURCES}
)

# Installation
install(TARGETS codetime DESTINATION bin)

# Enable testing
enable_testing()
add_subdirectory(tests)
