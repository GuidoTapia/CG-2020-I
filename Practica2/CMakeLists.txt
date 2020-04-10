cmake_minimum_required(VERSION 3.9.x FATAL_ERROR)
project(Practica2)

#For adding source files
file(GLOB_RECURSE SOURCE_FILES
	${CMAKE_SOURCE_DIR}/src/*.c
	${CMAKE_SOURCE_DIR}/src/*.cpp)

#Add headers
file(GLOB_RECURSE HEADER_FILES
	${CMAKE_SOURCE_DIR}/include/*.h
	${CMAKE_SOURCE_DIR}/include/*.hpp)

#Add libraries
link_directories(${CMAKE_SOURCE_DIR}/lib)

#Configure assets header file
configure_file(src/helpers/RootDir.h.in src/helpers/RootDir.h)
include_directories(${CMAKE_BINARY_DIR}/src)

add_executable(${PROJECT_NAME} ${HEADER_FILES} ${SOURCE_FILES})

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

# OpenGL
find_package(OpenGL REQUIRED)

# GLFW
find_package(GLFW3 REQUIRED)
message(STATUS "Found GLFW3 in ${GLFW3_INCLUDE_DIR}")

# GLAD
add_library(GLAD "thirdparty/glad.c")


set(LIBS glfw3 opengl32 GLAD)

# Define the include DIRs
include_directories(
	"${CMAKE_SOURCE_DIR}/src"
	"${CMAKE_SOURCE_DIR}/include"
)

# Define the link libraries
target_link_libraries(${PROJECT_NAME} ${LIBS})

if(MSVC_IDE)
	# Macro to preserve source files hierarchy in the IDE
	macro(GroupSources curdir)
		file(GLOB children RELATIVE ${PROJECT_SOURCE_DIR}/${curdir} ${PROJECT_SOURCE_DIR}/${curdir}/*)

		foreach(child ${children})
			if(IS_DIRECTORY ${PROJECT_SOURCE_DIR}/${curdir}/${child})
				GroupSources(${curdir}/${child})
			else()
				string(REPLACE "/" "\\" groupname ${curdir})
				string(REPLACE "src" "Sources" groupname ${groupname})
				source_group(${groupname} FILES ${PROJECT_SOURCE_DIR}/${curdir}/${child})
			endif()
		endforeach()
	endmacro()

	# Run macro
	GroupSources(src)
endif()




