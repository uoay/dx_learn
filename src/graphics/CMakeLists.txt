aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR} sourceFiles)
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/bindable sourceFiles)
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/descriptor_heap sourceFiles)
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/drawable sourceFiles)

add_library(Graphics STATIC ${sourceFiles})

target_include_directories(Graphics PUBLIC ${CMAKE_SOURCE_DIR}/include/graphics)

target_include_directories(Graphics PUBLIC ${CMAKE_SOURCE_DIR}/external/DirectX-Headers/include)

target_link_libraries(Graphics PRIVATE DirectX-Headers DirectX-Guids)

target_link_libraries(Graphics PRIVATE d3dcompiler.lib D3D12.lib dxgi.lib)