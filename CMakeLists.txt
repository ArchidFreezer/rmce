# CMakeList.txt : CMake project for dataload, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.20...4.1)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project (dataload VERSION 0.1
                  DESCRIPTION "Rolemaster Character Editor"
                  LANGUAGES CXX)

enable_testing()
include(CTest)

find_package(Doxygen REQUIRED COMPONENTS dot)

if (Doxygen_FOUND)
  add_subdirectory(docs)
else()
  message(STATUS "Doxygen not found, not building docs")
endif()

# Build the libraries
add_subdirectory("src/archidutils")
add_subdirectory("src/dataparsers")
add_subdirectory("src/ruledata")

# Build the main executables
add_subdirectory("apps")

# Build the test code
add_subdirectory("test")