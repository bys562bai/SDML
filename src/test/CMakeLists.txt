function(add_test name)
    add_executable(${name} ${ARGN})
    target_link_libraries(${name} PRIVATE SDML_LIB)
endfunction()

# add_test(test_buffer test_lexer_number.cpp)
foreach(n ${SDML_TEST_NAMES})
    add_test(${n} ${n}.cpp)
endforeach()
