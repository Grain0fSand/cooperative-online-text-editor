cmake_minimum_required(VERSION 3.0)
set(TARGET_NAME translator)

project(${TARGET_NAME})

MACRO(SUBDIRLIST result curdir)
	FILE(GLOB children RELATIVE ${curdir} ${curdir}/*)
	SET(dirlist "")
	FOREACH(child ${children})
		IF(IS_DIRECTORY ${curdir}/${child})
			LIST(APPEND dirlist ${curdir}${child})
		ENDIF()
	ENDFOREACH()
	SET(${result} ${dirlist})
ENDMACRO()


if(APPLE)
	set(Qt5Widgets_INCLUDE_DIRS /usr/local/Cellar/qt/5.13.1/include)
	set(Qt5Widgets_DEFINITIONS /usr/local/Cellar/qt/5.13.1/lib)
	set(Qt5Widgets_DIR /usr/local/Cellar/qt/5.13.1/lib/cmake/Qt5Widgets)
	set(Qt5PrintSupport_DIR /usr/local/Cellar/qt/5.13.0/lib/cmake/Qt5PrintSupport)
	set(Qt5Network_DIR /usr/local/Cellar/qt/5.13.0/lib/cmake/Qt5Network)
	SUBDIRLIST(CMAKE_PREFIX_PATH /usr/local/Cellar/qt/5.13.1/lib/cmake/)
   	#message(${CMAKE_PREFIX_PATH})
	message(include dir:${Qt5Widgets_INCLUDE_DIRS})
	message(definition dir:${Qt5Widgets_DEFINITIONS})
endif()


# Adding the qt binary library
include_directories(${Qt5Widgets_INCLUDE_DIRS})

# Use the compile definitions defined in the Qt 5 Widgets module
add_definitions(${Qt5Widgets_DEFINITIONS})

# Check the build type and ask the user to set concrete one
if(NOT CMAKE_BUILD_TYPE)
   set(CMAKE_BUILD_TYPE RelWithDebInfo)
   message(WARNING "CMAKE_BUILD_TYPE is not set, forcing to RelWithDebInfo")
endif()

# Set compiler flags
if(${CMAKE_CXX_COMPILER_ID} MATCHES "GNU" OR ${CMAKE_CXX_COMPILER_ID} MATCHES "Clang")
	set(CMAKE_CXX_FLAGS "-std=c++11 -Wall -Wextra")
	set(CMAKE_CXX_FLAGS_DEBUG "-O0 -g3")
	set(CMAKE_CXX_FLAGS_RELEASE "-O3")
	set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-O3 -g3")
	set(CMAKE_CXX_FLAGS_MINSIZEREL "-Os")
endif()



# package dependency list
find_package(Qt5PrintSupport REQUIRED)
find_package(Qt5Widgets REQUIRED)
find_package(Qt5Network REQUIRED)


# including directory
include_directories(
		${CMAKE_CURRENT_BINARY_DIR}
		${CMAKE_CURRENT_SOURCE_DIR}
		${Qt5Widgets_DIR}
)

# Instruct CMake to run moc automatically when needed
set(CMAKE_AUTOMOC ON)

# Source files
set(SOURCES
		main.cpp
		data_structure/action.cpp
		data_structure/action.h
		data_structure/remotecursor.cpp
		data_structure/remotecursor.h
		data_structure/crdt.cpp
		data_structure/crdt.h
		forms/loginwindow.cpp
		forms/loginwindow.h
		forms/mainwindow.cpp
		forms/mainwindow.h
		forms/mytextedit.cpp
		forms/mytextedit.h
		forms/usertag.cpp
		forms/usertag.h
		utility/image_utility.h
		utility/json_comunicator.cpp
		utility/json_comunicator.h
		utility/shared_editor.h
		web_requests/online_synchronizer.h
		web_requests/smtpclient.cpp
        web_requests/smtpclient.h
)

# User interface files
set(FORMS
		forms/mainwindow.ui
		forms/loginwindow.ui
)

# Resource files
set(RESOURCES
	resources.qrc
)

# Shared libraries
set(LIBRARIES
		Qt5::Widgets
		Qt5::Network
		Qt5::PrintSupport
)

# Generate additional sources with MOC and UIC
qt5_wrap_ui(UIC_SOURCES ${FORMS})
qt5_add_resources(RCC_SOURCES ${RESOURCES})

# Set target
add_executable(${TARGET_NAME} ${SOURCES} ${HEADERS} ${UIC_SOURCES} ${RCC_SOURCES})

# Link with libraries
target_link_libraries(${TARGET_NAME} ${LIBRARIES})

# Installation
install(TARGETS ${TARGET_NAME} RUNTIME DESTINATION bin)
install(FILES resources/${TARGET_NAME}.png DESTINATION share/icons/hicolor/48x48/apps)
install(FILES ${TARGET_NAME}.desktop DESTINATION share/applications)
