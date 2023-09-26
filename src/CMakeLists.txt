add_subdirectory(graphics)

aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR} sourceFiles)

add_executable(${PROJECT_NAME} WIN32 ${sourceFiles})

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/include)

target_link_libraries(${PROJECT_NAME} PUBLIC Graphics)