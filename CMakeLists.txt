# CMakeList.txt : CMake project for dataload, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.20)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("dataload")

# These need to be above the add_executable line
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED true)

# Add source to this project's executable.
add_executable (dataload "dataload.cpp"  "DatafileParser.h" "ArmourType.h" "BookData.h"  "DatafileParser.cpp" "BookDatafileParser.h" "BookDatafileParser.cpp" "CreatureSize.h" "StringUtils.h" "CriticalTableType.h" "CriticalType.h" "GameRuleData.h" "GameRuleDataCache.h"  "LanguageCategoryData.h" "LanguageCategoryDatafileParser.h" "LanguageCategoryDatafileParser.cpp")

target_include_directories(dataload PUBLIC
                           "${PROJECT_BINARY_DIR}"
                           "${PROJECT_SOURCE_DIR}/../Libraries/boost_1_90_0"
                           )
