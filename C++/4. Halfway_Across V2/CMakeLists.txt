cmake_minimum_required(VERSION 3.16)

project(Halfway_Across VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Multimedia)
include_directories(${Qt5Core_INCLUDE_DIRS} ${Qt5Gui_INCLUDE_DIRS} ${Qt5Multimedia_INCLUDE_DIRS})

set(PROJECT_SOURCES
        src/main.cpp
        src/mainwindow.cpp
        include/mainwindow.h
        src/mainwindow.ui
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(Halfway_Across
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        include/handling.h
        include/locations.h
        include/player.h
        include/world.h
        resources/resources.qrc
        src/locations.cpp
        src/player.cpp
        src/world.cpp


        src/handling/begin.cpp
        src/handling/begin.cpp

        src/handling/drink.cpp
        src/handling/drink.cpp
        src/handling/drop.cpp



        src/handling/handling.cpp


        src/handling/look.cpp
        src/handling/look.cpp
        src/handling/move.cpp
        src/handling/move.cpp



        src/handling/sit.cpp
        src/handling/sit.cpp
        src/handling/sleep.cpp
        src/handling/sleep.cpp
        src/handling/take.cpp


        src/handling/use.cpp
        src/handling/use.cpp



        src/handling/stand.cpp
        include/halfway.h
        src/handling/remove.cpp
        src/handling/wear.cpp
        src/handling/wait.cpp
        include/audio.h
        src/handling/audio.cpp
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET Halfway_Across APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(Halfway_Across SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(Halfway_Across
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(Halfway_Across PRIVATE Qt${QT_VERSION_MAJOR}::Widgets Qt6::Multimedia)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.Halfway_Across)
endif()
set_target_properties(Halfway_Across PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS Halfway_Across
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(Halfway_Across)
endif()
