cmake_minimum_required(VERSION 3.5)

project(rpg VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CLANG_TIDY_TARGET  DIRECTORY "${PROJECT_SOURCE_DIR}")
option(CLANG_TIDY_OPTION OFF)
option(DEBUG_INFO OFF)

set_property(
    ${CLANG_TIDY_TARGET}
    PROPERTY WITH_CLANG_TIDY
    "${CLANG_TIDY_OPTION}"
)

set_property(
    ${CLANG_TIDY_TARGET}
    PROPERTY WITH_DEBUG_INFO
    "${DEBUG_INFO}"
)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

set(PROJECT_SOURCES
        Main.cpp
        MainWindow.cpp
        MainWindow.h
        MainWindow.ui
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(rpg
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        RpgUi.h
        Rpg.h
        Rpg.cpp

    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET rpg APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(rpg SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(rpg
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(rpg PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.rpg)
endif()
set_target_properties(rpg PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS rpg
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(rpg)
endif()

set(RPG_ASSET_DIR "${CMAKE_SOURCE_DIR}/assets" CACHE STRING "OS independent full path to assets directory")
add_compile_definitions(RPG_ASSET_DIR="${CMAKE_SOURCE_DIR}/assets")

get_property(COMPILE_WITH_CLANG_TIDY ${CLANG_TIDY_TARGET} PROPERTY WITH_CLANG_TIDY)

if(COMPILE_WITH_CLANG_TIDY)
find_program(
  CLANG_TIDY_EXE
  NAMES "clang-tidy"
  DOC "Path to clang-tidy executable"
  )
if(NOT CLANG_TIDY_EXE)
  message(STATUS "clang-tidy not found.")
else()
  message(STATUS "clang-tidy found: ${CLANG_TIDY_EXE}")
  set(DO_CLANG_TIDY "${CLANG_TIDY_EXE}" "-checks=*, -clang-analyzer-alpha.*, \
                     -modernize-use-trailing-return-type, -llvmlibc-restrict-system-libc-headers, -llvmlibc-implementation-in-namespace, \
                     -llvmlibc-callee-namespace, -llvmlibc-callee-namespace, -fuchsia-default-arguments-calls, -altera-unroll-loops, \
                     -altera-id-dependent-backward-branch")
endif()

set_target_properties(
    rpg PROPERTIES
  CXX_STANDARD 17
  CXX_STANDARD_REQUIRED ON
  COMPILE_FLAGS "${WARNING_FLAGS}"
)

if(CLANG_TIDY_EXE)
  set_target_properties(
    rpg PROPERTIES
    CXX_CLANG_TIDY "${DO_CLANG_TIDY}"
  )
endif()

endif()

# cmake -DCMAKE_BUILD_TYPE=DEBUG -G "Unix Makefiles" -D CLANG_TIDY_OPTION=ON -D CMAKE_CXX_COMPILER=/usr/bin/clang++ ..