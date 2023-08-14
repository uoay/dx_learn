aux_source_directory(${CMAKE_SOURCE_DIR}/src sourceFiles)
add_executable(${PROJECT_NAME} WIN32 ${sourceFiles})
target_link_libraries(${PROJECT_NAME} PUBLIC d3dcompiler.lib D3D12.lib dxgi.lib)
target_link_libraries(${PROJECT_NAME} PUBLIC DirectX-Headers DirectX-Guids)