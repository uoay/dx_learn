add_custom_target(Shaders)

set(shaderFiles VertexShader.hlsl PixelShader.hlsl)

set_source_files_properties(VertexShader.hlsl PROPERTIES ShaderType "vs")
set_source_files_properties(PixelShader.hlsl PROPERTIES ShaderType "ps")

set_source_files_properties(${shaderFiles} PROPERTIES ShaderModel "5_0")

foreach(file ${shaderFiles})
	get_filename_component(fileName ${file} NAME_WE)
	get_source_file_property(shaderType ${file} ShaderType)
	get_source_file_property(shaderModel ${file} ShaderModel)
	add_custom_command(
		TARGET Shaders
		COMMAND fxc.exe /T ${shaderType}_${shaderModel} /Fo ${CMAKE_BINARY_DIR}/shader/${fileName}.cso ${file}
		MAIN_DEPENDENCY ${file}
		WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
		VERBATIM
	)
endforeach()
