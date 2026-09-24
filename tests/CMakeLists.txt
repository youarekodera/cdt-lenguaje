# Test CMakeLists.txt

# Lexer tests
add_executable(test_lexer lexer/test_lexer.c)
target_include_directories(test_lexer PRIVATE ${CMAKE_SOURCE_DIR}/compiler/include)
target_link_libraries(test_lexer PRIVATE codetime)

# Add test
add_test(NAME lexer_test COMMAND test_lexer)
