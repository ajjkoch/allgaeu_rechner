cmake_minimum_required(VERSION 3.16)

# adapt project name
project(Test_Calculator  VERSION 1.0.0.0 DESCRIPTION "Calculator Unit Tests" LANGUAGES CXX)
enable_testing()

# IS_DEVELOPER_MODE OFF: build the plugin (.dll, .so , ... )
# IS_DEVELOPER_MODE ON : build a console executable (.exe, .o, ... ) that you can just run in that developer environment in a command shell.
set(IS_DEVELOPER_MODE ON)

set(CMAKE_INCLUDE_CURRENT_DIR OFF)
set(CMAKE_AUTOMOC ON)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# adapt names here
set(CLASSNAME MyTest)
set(SOURCES ../../../common/Util.h Test_Calculator.cpp )

include(../../../common/test_common.cmake)
