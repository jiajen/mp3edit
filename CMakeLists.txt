#######################################################################
##                              Header                               ##
#######################################################################

## CMake Required Settings
cmake_minimum_required(VERSION 3.0)

## Library Variables
set(LIB_INC_DIR "..")
set(LIB_LIB_DIR "")
set(LIB_LINKED_LIBS "")

#######################################################################
##                         User Configurable                         ##
#######################################################################

## Project Name and Version
project(mp3edit VERSION 6.0.0 LANGUAGES CXX)

## Poco
# find_library(POCO_FOUNDATION PocoFoundation)
# set(LIB_LINKED_LIBS ${LIB_LINKED_LIBS} ${POCO_FOUNDATION})

## Gtkmm
find_package(PkgConfig)
pkg_check_modules(GTKMM3 gtkmm-3.0)
set(LIB_INC_DIR ${LIB_INC_DIR} ${GTKMM3_INCLUDE_DIRS})
set(LIB_LIB_DIR ${LIB_LIB_DIR} ${GTKMM3_LIBRARY_DIRS})
set(LIB_LINKED_LIBS ${LIB_LINKED_LIBS} ${GTKMM3_LIBRARIES})

## Gtkmm Internal GUI
file(READ ${PROJECT_SOURCE_DIR}/bin/${CMAKE_PROJECT_NAME}.glade GLADE_CONTENTS)
string(REGEX REPLACE " *\n *" "" GLADE_CONTENTS ${GLADE_CONTENTS})
string(REPLACE "\"" "\\\"" GLADE_CONTENTS ${GLADE_CONTENTS})

## C++17 Filesystem support is still experimental hence requiring the below
set(LIB_LINKED_LIBS ${LIB_LINKED_LIBS} -lstdc++fs)

#######################################################################
##                          Default Options                          ##
#######################################################################

## Binary Directory
set(EXECUTABLE_OUTPUT_PATH "${PROJECT_SOURCE_DIR}/bin")

## Configure Application Title
configure_file(
  "${PROJECT_SOURCE_DIR}/src/main.h.in"
  "${PROJECT_SOURCE_DIR}/src/main.h"
)

## Custom Compiler Flags
set(COMPILER_FLAGS_CUSTOM -pthread)

## Compiler Optimisations
set(COMPILER_FLAGS_OPTIMISATIONS -O2 -flto)

## Compiler Flags (Warnings)
## parentheses and cast-function-type are ignored as the GTK Headers has
## issues
set(COMPILER_FLAGS_WARNING -Wall -Wextra -Wfatal-errors
                           -Wno-parentheses -Wno-cast-function-type)

## Compiler Flags (C++ Version)
set(COMPILER_FLAGS_CXX_VERSION -std=c++17)

## Compiler Flags Merge
set(COMPILER_FLAGS
  ${COMPILER_FLAGS_CXX_VERSION}
  ${COMPILER_FLAGS_WARNING}
  ${COMPILER_FLAGS_CUSTOM}
  ${COMPILER_FLAGS_OPTIMISATIONS}
)

## Configure Compiler Flags and Linker Flags
add_compile_options(${COMPILER_FLAGS})
string(REPLACE ";" " " CMAKE_EXE_LINKER_FLAGS "${COMPILER_FLAGS}")

# Merge Library Variables
include_directories(${LIB_INC_DIR})
link_directories(${LIB_LIB_DIR})

## Dependencies
file(GLOB_RECURSE INC_FILES "${PROJECT_SOURCE_DIR}/src/*.h")
file(GLOB_RECURSE SRC_FILES "${PROJECT_SOURCE_DIR}/src/*.cc")

## Build
add_executable(${CMAKE_PROJECT_NAME} ${INC_FILES} ${SRC_FILES})

## Link Libraries
target_link_libraries(${CMAKE_PROJECT_NAME} ${LIB_LINKED_LIBS})
