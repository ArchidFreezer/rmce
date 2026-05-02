# CMakeList.txt : CMake project for dataload, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.20...4.1)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Enable vcpkg integration
set(CMAKE_TOOLCHAIN_FILE "${CMAKE_SOURCE_DIR}/vcpkg_local/scripts/buildsystems/vcpkg.cmake" CACHE STRING "Vcpkg toolchain file")

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

if(WIN32)
    add_compile_definitions(_WIN32_WINNT=0x0A00)  # Windows 10
    # Or for Windows 7: _WIN32_WINNT=0x0601
endif()

project (RoleMasterCharacterEditor VERSION 0.1
                  DESCRIPTION "Rolemaster Character Editor"
                  LANGUAGES CXX)

if(MSVC)
  add_compile_options(/utf-8)
endif()

enable_testing()
include(CTest)

find_package(Doxygen REQUIRED COMPONENTS dot)

find_package(Boost REQUIRED COMPONENTS system)
find_package(Boost REQUIRED COMPONENTS thread)
find_package(spdlog REQUIRED)

if (Doxygen_FOUND)
  add_subdirectory(docs)
else()
  message(STATUS "Doxygen not found, not building docs")
endif()

# Build the libraries
add_subdirectory("src/archid")
add_subdirectory("src/rm")

# Build the main executables
add_subdirectory("apps")
add_subdirectory("sandbox")

# Build the test code
add_subdirectory("test")