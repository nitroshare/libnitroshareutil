# libnitroshareutil
# Copyright (C) 2013  Nathan Osman

cmake_minimum_required(VERSION 2.8.9)
project(nitroshareutil)

OPTION(BUILD_TESTS "Causes the test suite to be built" ON)

set(VERSION_MAJOR 0)
set(VERSION_MINOR 1)
set(VERSION_PATCH 0)
set(VERSION "${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}")

include(CXX11.cmake)
check_for_cxx11_compiler(CXX11_COMPILER)
if(NOT CXX11_COMPILER)
    message(FATAL_ERROR "Compiler does not support C++11")
endif()
enable_cxx11()

find_package(Qt5Core REQUIRED)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
include_directories(include)

set(SRC
    src/asynctask.cpp
    src/blockingasynctask.cpp
    src/directoryenumerator.cpp
    src/fileheader.cpp
    src/fileheaderlist.cpp
    src/parameterbuilder.cpp)
qt5_wrap_cpp(MOC
    include/nitroshare/util/asynctask.h
    include/nitroshare/util/blockingasynctask.h
    src/directoryenumerator.h)

add_library(nitroshareutil SHARED ${SRC} ${MOC})
set_target_properties(nitroshareutil PROPERTIES
    VERSION   ${VERSION}
    SOVERSION ${VERSION_MAJOR})

qt5_use_modules(nitroshareutil Core)

install(DIRECTORY include/nitroshare/util
    DESTINATION include/nitroshare)
install(TARGETS nitroshareutil
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    RUNTIME DESTINATION bin)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/nitroshareutil.pc.in
    ${CMAKE_CURRENT_BINARY_DIR}/nitroshareutil.pc @ONLY)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/nitroshareutil.pc
    DESTINATION lib/pkgconfig)

if(BUILD_TESTS)
    add_subdirectory(tests)
endif()
