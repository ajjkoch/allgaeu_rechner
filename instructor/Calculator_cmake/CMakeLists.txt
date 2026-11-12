## Copyright (c) 2026 Koch, Achim Johannes Josef — MIT License

cmake_minimum_required(VERSION 3.16)

file(STRINGS "${CMAKE_CURRENT_SOURCE_DIR}/../../VERSION" CALCULATOR_VERSION LIMIT_COUNT 1)
if(NOT CALCULATOR_VERSION MATCHES "^[0-9]+\\.[0-9]+\\.[0-9]+$")
    message(FATAL_ERROR "VERSION must contain MAJOR.MINOR.PATCH")
endif()

project(Calculator VERSION ${CALCULATOR_VERSION} DESCRIPTION "Qt Calculator" LANGUAGES CXX)

remove_definitions(CONSOLE_TEST)

enable_testing()

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_C_STANDARD 14)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(ANDROID)
    find_package(Qt6 6 REQUIRED COMPONENTS Core Widgets)
else()
    find_package(Qt6 6 REQUIRED COMPONENTS Core Widgets Test)
endif()

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
    AlpineTheme.h
    AlpineTheme.cpp
    AllgaeuButton.h
    AllgaeuButton.cpp
    mainwindow.ui
    alpine_resources.qrc
    ../../common/Util.h
)

qt_add_executable(${PROJECT_NAME} MANUAL_FINALIZATION ${PROJECT_SOURCES})

target_include_directories(${PROJECT_NAME}
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(${PROJECT_NAME} PUBLIC
    Qt6::Core
    Qt6::Widgets
)

if(ANDROID)
    # Android requires a monotonically increasing integer version code.
    math(EXPR CALCULATOR_VERSION_CODE
        "${PROJECT_VERSION_MAJOR} * 10000 + ${PROJECT_VERSION_MINOR} * 100 + ${PROJECT_VERSION_PATCH}")

    set_target_properties(${PROJECT_NAME} PROPERTIES
        QT_ANDROID_PACKAGE_NAME "de.ajjkoch.calculator"
        QT_ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android"
        QT_ANDROID_MIN_SDK_VERSION 23
        QT_ANDROID_TARGET_SDK_VERSION 35
        QT_ANDROID_VERSION_NAME "${PROJECT_VERSION}"
        QT_ANDROID_VERSION_CODE ${CALCULATOR_VERSION_CODE}
    )
endif()

install(TARGETS ${PROJECT_NAME}
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

qt_finalize_executable(${PROJECT_NAME})

# Qt Designer custom-widget plugin (desktop only).
# The widget itself is also compiled into the Android application above.
if(NOT ANDROID)
    find_package(Qt6 COMPONENTS UiPlugin QUIET)
    if(TARGET Qt6::UiPlugin)
        qt_add_plugin(AllgaeuButtonDesignerPlugin
            CLASS_NAME AllgaeuButtonPlugin
            AllgaeuButtonPlugin.h
            AllgaeuButtonPlugin.cpp
            AllgaeuButtonPlugin.json
            AllgaeuButton.h
            AllgaeuButton.cpp
            alpine_resources.qrc
        )
        target_include_directories(AllgaeuButtonDesignerPlugin
            PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}
        )
        target_link_libraries(AllgaeuButtonDesignerPlugin PRIVATE
            Qt6::Core
            Qt6::Widgets
            Qt6::UiPlugin
        )
        set_target_properties(AllgaeuButtonDesignerPlugin PROPERTIES
            LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/designer"
            RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/designer"
        )

        # Keep the regular CMake install rule for explicit installs/packages.
        install(TARGETS AllgaeuButtonDesignerPlugin
            LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}/qt6/plugins/designer
            RUNTIME DESTINATION ${CMAKE_INSTALL_LIBDIR}/qt6/plugins/designer
        )

        # Also deploy the freshly built plugin to the active Qt installation
        # after each build, so Qt Designer can load it without a manual install.
        # Override this cache path when Designer uses a custom plugin directory.
        set(ALLGAEU_QT_DESIGNER_PLUGIN_DIR "" CACHE PATH
            "Qt Designer plugin directory (defaults to QT_INSTALL_PLUGINS/designer)")

        if(NOT ALLGAEU_QT_DESIGNER_PLUGIN_DIR)
            find_program(QT_PATHS_EXECUTABLE
                NAMES qtpaths6 qtpaths
                HINTS "${QT_HOST_PATH}/bin" "${Qt6_DIR}/../../../bin"
            )
            if(QT_PATHS_EXECUTABLE)
                execute_process(
                    COMMAND "${QT_PATHS_EXECUTABLE}" --query QT_INSTALL_PLUGINS
                    RESULT_VARIABLE QT_PATHS_RESULT
                    OUTPUT_VARIABLE QT_INSTALL_PLUGINS_DIR
                    OUTPUT_STRIP_TRAILING_WHITESPACE
                    ERROR_QUIET
                )
                if(QT_PATHS_RESULT EQUAL 0 AND QT_INSTALL_PLUGINS_DIR)
                    set(ALLGAEU_QT_DESIGNER_PLUGIN_DIR
                        "${QT_INSTALL_PLUGINS_DIR}/designer")
                endif()
            endif()
        endif()

        # Qt Creator may use a different Qt/Designer installation than the
        # standalone Designer. Configure its plugin directory explicitly when
        # the embedded Designer does not use the Qt installation above.
        set(ALLGAEU_QT_CREATOR_PLUGIN_DIR "" CACHE PATH
            "Additional Qt Creator Designer plugin directory (optional)")

        set(ALLGAEU_DESIGNER_PLUGIN_DIRS)
        if(ALLGAEU_QT_DESIGNER_PLUGIN_DIR)
            list(APPEND ALLGAEU_DESIGNER_PLUGIN_DIRS
                "${ALLGAEU_QT_DESIGNER_PLUGIN_DIR}")
        else()
            message(WARNING
                "Could not determine standalone Qt Designer plugin directory. "
                "Set ALLGAEU_QT_DESIGNER_PLUGIN_DIR to deploy it.")
        endif()

        if(ALLGAEU_QT_CREATOR_PLUGIN_DIR)
            list(APPEND ALLGAEU_DESIGNER_PLUGIN_DIRS
                "${ALLGAEU_QT_CREATOR_PLUGIN_DIR}")
        endif()

        if(ALLGAEU_DESIGNER_PLUGIN_DIRS)
            list(REMOVE_DUPLICATES ALLGAEU_DESIGNER_PLUGIN_DIRS)
            set(ALLGAEU_DESIGNER_DEPLOY_COMMANDS)
            foreach(PLUGIN_DIR IN LISTS ALLGAEU_DESIGNER_PLUGIN_DIRS)
                message(STATUS
                    "AllgaeuButton Designer plugin will be deployed to: ${PLUGIN_DIR}")
                list(APPEND ALLGAEU_DESIGNER_DEPLOY_COMMANDS
                    COMMAND "${CMAKE_COMMAND}" -E make_directory "${PLUGIN_DIR}"
                    COMMAND "${CMAKE_COMMAND}" -E copy_if_different
                        "$<TARGET_FILE:AllgaeuButtonDesignerPlugin>"
                        "${PLUGIN_DIR}/$<TARGET_FILE_NAME:AllgaeuButtonDesignerPlugin>"
                )
            endforeach()

            add_custom_command(TARGET AllgaeuButtonDesignerPlugin POST_BUILD
                ${ALLGAEU_DESIGNER_DEPLOY_COMMANDS}
                COMMENT "Deploying AllgaeuButtonDesignerPlugin to configured Designer directories"
                VERBATIM
            )
        endif()
    else()
        message(STATUS "Qt6 UiPlugin not found: Designer plugin target is disabled")
    endif()
endif()

# GUI interaction tests use Qt Test and are intentionally desktop-only.
if(NOT ANDROID)
    qt_add_executable(CalculatorGuiTests
        mainwindow.h
        mainwindow.cpp
        AlpineTheme.h
        AlpineTheme.cpp
        AllgaeuButton.h
        AllgaeuButton.cpp
        mainwindow.ui
        alpine_resources.qrc
        UnitTests/Test_MainWindow.cpp
    )

    target_include_directories(CalculatorGuiTests
        PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}
    )

    target_link_libraries(CalculatorGuiTests PRIVATE
        Qt6::Core
        Qt6::Widgets
        Qt6::Test
    )

    add_test(NAME CalculatorGuiTests COMMAND CalculatorGuiTests)
    set_tests_properties(CalculatorGuiTests PROPERTIES
        ENVIRONMENT "QT_QPA_PLATFORM=offscreen"
    )
endif()
