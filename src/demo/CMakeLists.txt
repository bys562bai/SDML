function(add_demo name)
    add_executable(${name} ${ARGN})
    target_link_libraries(${name} PRIVATE SDML_LIB)
endfunction()

add_demo(demo1 demo1.cpp)