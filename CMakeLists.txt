cmake_minimum_required(VERSION 3.23)

project(IGCITHelper VERSION 2.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

enable_language("RC")

find_package(QT NAMES Qt6 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

set(PROJECT_SOURCES
    mainwindow.ui

    Logger.h

    include/win.h
    include/registryUtils.h
    include/registryUtils.cpp

    pages/DevicePage.h
    pages/DevicePage.cpp
    pages/ToolsPage/Widgets/TDRDelay.h
    pages/ToolsPage/Widgets/TDRDelay.cpp
    pages/ToolsPage/Widgets/SSU.h
    pages/ToolsPage/Widgets/SSU.cpp
    pages/ToolsPage/Widgets/CrashDumps.h
    pages/ToolsPage/Widgets/CrashDumps.cpp
    pages/ToolsPage/ToolsPage.h
    pages/ToolsPage/ToolsPage.cpp
    pages/LogPage.h
    pages/LogPage.cpp

    main.cpp
    mainwindow.cpp
    mainwindow.h
)

qt_add_executable(${PROJECT_NAME}
    MANUAL_FINALIZATION
    ${PROJECT_SOURCES}
    resources/IGCITHelper.rc
)

target_link_libraries(${PROJECT_NAME} PRIVATE Qt${QT_VERSION_MAJOR}::Widgets Secur32)

set_target_properties(${PROJECT_NAME} PROPERTIES
    WIN32_EXECUTABLE TRUE
)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND mt -manifest ${CMAKE_CURRENT_SOURCE_DIR}/resources/IGCITHelper.exe.manifest -outputresource:${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}.exe
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    COMMENT "add manifest"
)

install(TARGETS ${PROJECT_NAME}
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

qt_finalize_executable(${PROJECT_NAME})
