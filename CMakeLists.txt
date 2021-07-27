cmake_minimum_required(VERSION 3.0)
project(grid CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-O2 -Wall")
set(CMAKE_CXX_FLAGS_DEBUG "-O0 -g -Wall")

find_package(GTest REQUIRED)

add_executable(gtest_${PROJECT_NAME}
	test/test_${PROJECT_NAME}.cpp
	src/util.cpp
	)
target_include_directories(gtest_${PROJECT_NAME}
		PUBLIC
			include
			)
target_link_libraries(gtest_${PROJECT_NAME}
		PRIVATE
			GTest::GTest
			GTest::Main
			)
