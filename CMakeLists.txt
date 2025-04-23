# ~Network/CMakeLists.txt

cmake_minimum_required(VERSION 3.24 FATAL_ERROR) # PATH_EQUAL

get_filename_component(PROJECT_NAME ${CMAKE_CURRENT_SOURCE_DIR} NAME)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_REQUIRED True)
project(${PROJECT_NAME})

set(TARGET_NAME Network)
add_subdirectory(NetworkLibrary)
set(ADDITIONAL_INCLUDE_DIRECTORIES ${ADDITIONAL_INCLUDE_DIRECTORIES} ${${TARGET_NAME}_HEADER_DIRECTORY})
set(ADDITIONAL_LINK_LIBRARY ${ADDITIONAL_LINK_LIBRARY} ${TARGET_NAME})

unset(TARGET_NAME)
add_subdirectory(MiniChatClient)
add_subdirectory(MiniChatServer)

if(MSVC)
	set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT MiniChatServer)
endif()
