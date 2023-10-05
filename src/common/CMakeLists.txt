aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR} sourceFiles)

add_library(Common STATIC ${sourceFiles})

target_include_directories(Common PUBLIC ${CMAKE_SOURCE_DIR}/include/common)