
include_directories(${SDML_INC})

source_group("Header Files" FILES ${SDML_HEARDER}) 
add_library(SDML_LIB STATIC ${SDML_SRC} ${SDML_HEARDER})
target_link_libraries(SDML_LIB PUBLIC ${SDML_DEP_LIB})

add_subdirectory(demo)
add_subdirectory(test)