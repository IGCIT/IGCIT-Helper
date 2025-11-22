cmake_minimum_required(VERSION 3.23)

project(IGCITHelper VERSION 3.1 DESCRIPTION "IGCIT Helper" LANGUAGES CXX)

set(PROJECT_AUTHOR "IGCIT")
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 6.10 REQUIRED COMPONENTS Core Widgets)

qt_standard_project_setup()

qt_add_resources(QT_RESOURCE
	resources/resources.qrc
)

configure_file(resources/win.rc.in ${CMAKE_CURRENT_SOURCE_DIR}/win.rc)

qt_add_executable(${PROJECT_NAME}
	${QT_RESOURCE}

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

	mainwindow.ui
	win.rc
)

include(CheckIPOSupported)
check_ipo_supported(RESULT has_ipo OUTPUT ipo_error)
if (has_ipo)
	message(STATUS "${PROJECT_NAME}: IPO/LTO enabled")
	set_property(TARGET ${PROJECT_NAME} PROPERTY INTERPROCEDURAL_OPTIMIZATION TRUE)
endif ()

target_link_libraries(${PROJECT_NAME} PRIVATE Qt::Core Qt::Widgets Secur32)

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
