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
add_executable (dataload "src/dataload.cpp"  "src/DatafileParser.h" "src/ArmourType.h" "src/BookData.h"  "src/DatafileParser.cpp" "src/BookDatafileParserJson.h" "src/BookDatafileParserJson.cpp" "src/CreatureSizeType.h" "src/StringUtils.h" "src/CriticalTableType.h" "src/CriticalType.h" "src/GameRuleData.h" "src/GameRuleDataCache.h" "src/LanguageCategoryData.h" "src/LanguageCategoryDatafileParserJson.h" "src/LanguageCategoryDatafileParserJson.cpp" "src/LanguageData.h" "src/LanguageDatafileParserJson.h" "src/LanguageDatafileParserJson.cpp" "src/DatafileParserXml.h" "src/DatafileParserXml.cpp" "src/DatafileParserJson.h" "src/DatafileParserJson.cpp" "src/LanguageDialectData.h" "src/RealmType.h" "src/ResistanceType.h" "src/SkillActionType.h" "src/SkillGroupType.h" "src/SkillProgressionType.h" "src/SpellListType.h" "src/SkillDevelopmentType.h" "src/StatType.h" "src/StatLossType.h" "src/SkillProgressionData.h" "src/SkillProgressionData.cpp" "src/SkillProgressionType.cpp" "src/CreatureSizeType.cpp" "src/StringUtils.cpp" "src/CriticalTableType.cpp" "src/CriticalType.cpp" "src/RealmType.cpp" "src/ResistanceType.cpp" "src/SpellListType.cpp" "src/StatLossType.cpp" "src/SkillActionType.cpp" "src/SkillDevelopmentType.cpp" "src/SkillGroupType.cpp" "src/StatType.cpp")

target_include_directories(dataload PUBLIC
                           "${PROJECT_BINARY_DIR}"
                           "${PROJECT_SOURCE_DIR}/../Libraries/boost_1_90_0"
                           )

find_package(Doxygen REQUIRED COMPONENTS dot)

if (Doxygen_FOUND)
  add_subdirectory(docs)
else()
  message(STATUS "Doxygen not found, not building docs")
endif()
