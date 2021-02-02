cmake_minimum_required(VERSION 3.17 FATAL_ERROR)
project(RayTracer LANGUAGES CXX VERSION 0.0.1)

file(GLOB source_files "${CMAKE_SOURCE_DIR}/src/*.cpp")

add_executable(RayTracer ${source_files})

set_target_properties(RayTracer
	PROPERTIES
		CXX_STANDARD 17
		CXX_STANDARD_REQUIRED YES
		BUILD_TYPE Debug
)