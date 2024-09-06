# ~Network/MiniChatServer/CMakeLists.txt
 if(NOT TARGET_NAME)
	get_filename_component(TARGET_NAME ${CMAKE_CURRENT_SOURCE_DIR} NAME)
 endif()

 file(GLOB_RECURSE TARGET_HEADER_FILES 	${CMAKE_CURRENT_SOURCE_DIR}/*.h
										${CMAKE_CURRENT_SOURCE_DIR}/*.hpp
										${CMAKE_CURRENT_SOURCE_DIR}/*.inl)
										
 file(GLOB_RECURSE TARGET_SOURCE_FILES 	${CMAKE_CURRENT_SOURCE_DIR}/*.c
										${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
										${CMAKE_CURRENT_SOURCE_DIR}/*.c++)
										
file(GLOB_RECURSE TARGET_EXTRA_FILES 	${CMAKE_CURRENT_SOURCE_DIR}/*.txt
										${CMAKE_CURRENT_SOURCE_DIR}/*.md)
										
 set(TARGET_FILES ${TARGET_HEADER_FILES} ${TARGET_SOURCE_FILES} ${TARGET_EXTRA_FILES})
 source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${TARGET_FILES})

 add_executable(${TARGET_NAME})
 
 target_sources(${TARGET_NAME} PRIVATE ${TARGET_FILES})
 target_include_directories(${TARGET_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/Header)
 target_include_directories(${TARGET_NAME} PRIVATE ${ADDITIONAL_INCLUDE_DIRECTORIES})
 target_link_libraries(${TARGET_NAME} PRIVATE ${ADDITIONAL_LINK_LIBRARY})


 if(MSVC)
	target_compile_options(${TARGET_NAME} PRIVATE /W4 /WX)
 else()
	message("not using MSVC - target_compile_options(${TARGET_NAME} - not set")
 endif()