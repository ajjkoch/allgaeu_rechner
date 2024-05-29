message(NOTICE "**** 1111111111111111 ****")

if( IS_DEVELOPER_MODE )
    add_compile_definitions( CONSOLE_TEST )
    find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core Test Gui)
    find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Test Gui)
    if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
        qt_add_executable(${PROJECT_NAME} MANUAL_FINALIZATION ${SOURCES} )
    else()
        if(ANDROID)
            add_library(${PROJECT_NAME} SHARED ${SOURCES} )
        else()
            add_executable(${PROJECT_NAME} ${SOURCES} )
        endif()
    endif()
    add_test(NAME ${PROJECT_NAME} COMMAND ${PROJECT_NAME})
    target_link_libraries(${PROJECT_NAME} PRIVATE
        Qt${QT_VERSION_MAJOR}::Core
        Qt${QT_VERSION_MAJOR}::Test
        Qt${QT_VERSION_MAJOR}::Gui
    )
    set_target_properties(${PROJECT_NAME} PROPERTIES
        MACOSX_BUNDLE_GUI_IDENTIFIER sie.test_plugine.com
        MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
        MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
        MACOSX_BUNDLE TRUE
#        WIN32_EXECUTABLE FALSE # If this is on you will not have any console output anymore!
    )
    install(TARGETS ${PROJECT_NAME}
        BUNDLE DESTINATION .
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    )

    if(QT_VERSION_MAJOR EQUAL 6)
        qt_finalize_executable(${PROJECT_NAME})
    endif()
else()
    remove_definitions( CONSOLE_TEST )
    remove_definitions( TEST_TOOL )
    find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core Test Gui)
    find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Test Gui)
    if(NOT ANDROID)
        # Configure version information
        configure_file(${CMAKE_SOURCE_DIR}/../version.rc.in ${CMAKE_BINARY_DIR}/version.rc @ONLY)
        set(OUTPUT_SUB_DIR out/build/${CMAKE_BUILD_TYPE})
        set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/${OUTPUT_SUB_DIR})
        set(DLL_FILE_PATH ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${PROJECT_NAME}${CMAKE_SHARED_LIBRARY_SUFFIX})
        set(TESTER_PLUGINS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../../SIEDPUApiInteractiveTestTool/${OUTPUT_SUB_DIR})
        message(NOTICE "dll file path: ${DLL_FILE_PATH}")
        message(NOTICE "  plugins dir: ${TESTER_PLUGINS_DIR}")
    endif()
    message(NOTICE "**** 2222222222222 ****")
    if(Qt${QT_VERSION_MAJOR} GREATER_EQUAL 6)
        message(NOTICE "**** 3333333333333 ****")
        if(ANDROID)
            target_include_directories( ${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR} )
            add_library(${PROJECT_NAME} MODULE ${SOURCES})
            set_target_properties(${PROJECT_NAME} PROPERTIES
                VERSION ${PROJECT_VERSION}
                SOVERSION ${PROJECT_VERSION}
            )
        else()
            qt_add_plugin(${PROJECT_NAME} CLASS_NAME ${CLASSNAME} ${SOURCES} ${CMAKE_BINARY_DIR}/version.rc)
        endif()
    else()
        message(NOTICE "**** 4444444444 ****")
        if(WIN32)
            add_library(${PROJECT_NAME} SHARED ${SOURCES} ${CMAKE_BINARY_DIR}/version.rc)
        else()
            message(NOTICE "**** 5555555555 ****")
            add_library(${PROJECT_NAME} SHARED ${SOURCES})
            set_target_properties(${PROJECT_NAME} PROPERTIES
                VERSION ${PROJECT_VERSION}
                SOVERSION ${PROJECT_VERSION}
            )
            message(NOTICE "**** 66666666 ****")
        endif()
    endif()
    target_link_libraries(${PROJECT_NAME} PRIVATE
        Qt${QT_VERSION_MAJOR}::Core
        Qt${QT_VERSION_MAJOR}::Test
        Qt${QT_VERSION_MAJOR}::Gui
    )
    set_target_properties(${PROJECT_NAME} PROPERTIES
        MACOSX_BUNDLE_GUI_IDENTIFIER sie.test_plugine.com
        MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
        MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
        MACOSX_BUNDLE TRUE
        #WIN32_EXECUTABLE FALSE # If this is on you will not have any console output anymore!
    )
    install(TARGETS ${PROJECT_NAME}
        BUNDLE DESTINATION .
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    )
    if(QT_VERSION_MAJOR EQUAL 6)
        qt_finalize_executable(${PROJECT_NAME})
    endif()
    if(NOT ANDROID)
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD COMMAND ${CMAKE_COMMAND} -E make_directory ${TESTER_PLUGINS_DIR})
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${DLL_FILE_PATH} ${TESTER_PLUGINS_DIR} )
    endif()
endif()
