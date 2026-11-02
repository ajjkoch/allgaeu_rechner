cmake_minimum_required(VERSION 3.16)

project(Calculator VERSION 1.0.0.0 DESCRIPTION "Qt Calculator" LANGUAGES CXX)

remove_definitions(CONSOLE_TEST)

# tell CMake to use CTest extension
enable_testing()

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_C_STANDARD 14)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 6 REQUIRED COMPONENTS Core Widgets)

include(GNUInstallDirs)

message(NOTICE "\n--------------------------------------------------------------------------------")
message(NOTICE "   project version: ${PROJECT_VERSION}")
message(NOTICE "  qt version major: ${QT_VERSION_MAJOR}")
message(NOTICE "       prefix path: ${CMAKE_PREFIX_PATH}")
message(NOTICE "runtime output dir: ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")
message(NOTICE "current binary dir: ${CMAKE_CURRENT_BINARY_DIR}")
message(NOTICE "--------------------------------------------------------------------------------\n")

set(PROJECT_SOURCES
    main.cpp
    mainwindow.h
    mainwindow.cpp
    mainwindow.ui
    ../../common/Util.h
)

qt_add_executable( ${PROJECT_NAME} MANUAL_FINALIZATION ${PROJECT_SOURCES} )

target_include_directories( ${PROJECT_NAME}
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries( ${PROJECT_NAME} PUBLIC
    Qt6::Core
    Qt6::Widgets)

install( TARGETS ${PROJECT_NAME}
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

qt_finalize_executable( ${PROJECT_NAME} )
