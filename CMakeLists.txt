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
add_executable (dataload "dataload.cpp"  "DatafileParser.h" "ArmourType.h" "BookData.h"  "DatafileParser.cpp" "BookDatafileParserJson.h" "BookDatafileParserJson.cpp" "CreatureSize.h" "StringUtils.h" "CriticalTableType.h" "CriticalType.h" "GameRuleData.h" "GameRuleDataCache.h" "LanguageCategoryData.h" "LanguageCategoryDatafileParserJson.h" "LanguageCategoryDatafileParserJson.cpp" "LanguageData.h" "LanguageDatafileParserJson.h" "LanguageDatafileParserJson.cpp" "DatafileParserXml.h" "DatafileParserXml.cpp" "DatafileParserJson.h" "DatafileParserJson.cpp" "LanguageDialectData.h" "Realm.h" "ResistanceType.h" "SkillAction.h" "SkillGroup.h" "SkillProgressionType.h" "SpellListType.h" "SkillDevelopmentType.h" "Stat.h")

target_include_directories(dataload PUBLIC
                           "${PROJECT_BINARY_DIR}"
                           "${PROJECT_SOURCE_DIR}/../Libraries/boost_1_90_0"
                           )

find_package(Doxygen REQUIRED COMPONENTS dot)

if (Doxygen_FOUND)
  set(DOXYGEN_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/docs)
  set(DOXYGEN_PROJECT_NAME "Rolemaster Character Editor")
  set(DOXYGEN_HAVE_DOT YES)
  doxygen_add_docs(project_docs ${PROJECT_SOURCE_DIR} ALL)
endif()
