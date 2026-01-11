add_library(archidutils STATIC "StringUtils.h" "StringUtils.cpp")
target_compile_features(archidutils PUBLIC cxx_std_20)

add_library(ruledata STATIC "GameRuleData.h" "GameRuleDataCache.h" "ArmourType.h" "BookData.h" "CreatureSizeType.cpp" "CreatureSizeType.h" "CriticalTableType.cpp" "CriticalTableType.h" "CriticalType.cpp" "CriticalType.h" "LanguageCategoryData.h" "LanguageData.h" "LanguageDialectData.h" "RealmType.cpp" "RealmType.h" "ResistanceType.cpp" "ResistanceType.h" "SkillActionType.cpp" "SkillActionType.h" "SkillDevelopmentType.cpp" "SkillDevelopmentType.h" "SkillGroupType.cpp" "SkillGroupType.h" "SkillProgressionData.cpp" "SkillProgressionData.h" "SkillProgressionType.cpp" "SkillProgressionType.h" "SpellListType.cpp" "SpellListType.h" "StatLossType.cpp" "StatLossType.h" "StatType.cpp" "StatType.h")
target_compile_features(ruledata PUBLIC cxx_std_20)

add_library(dataparsers STATIC "BookDatafileParserJson.cpp" "BookDatafileParserJson.h" "DatafileParser.cpp" "DatafileParser.h" "DatafileParserJson.cpp" "DatafileParserJson.h" "DatafileParserXml.cpp" "DatafileParserXml.h" "LanguageCategoryDatafileParserJson.cpp" "LanguageCategoryDatafileParserJson.h" "LanguageDatafileParserJson.cpp" "LanguageDatafileParserJson.h")
target_compile_features(dataparsers PUBLIC cxx_std_20)
target_include_directories(dataparsers PUBLIC "${PROJECT_SOURCE_DIR}/../Libraries/boost_1_90_0")

# Add source to this project's executable.
add_executable (dataload "dataload.cpp")
target_compile_features(dataload PRIVATE cxx_std_20)
target_link_libraries(dataload PUBLIC archidutils ruledata dataparsers)
