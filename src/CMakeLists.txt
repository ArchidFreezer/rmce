# Add source to this project's executable.
add_executable (dataload "dataload.cpp"  "DatafileParser.h" "ArmourType.h" "BookData.h"  "DatafileParser.cpp" "BookDatafileParserJson.h" "BookDatafileParserJson.cpp" "CreatureSizeType.h" "StringUtils.h" "CriticalTableType.h" "CriticalType.h" "GameRuleData.h" "GameRuleDataCache.h" "LanguageCategoryData.h" "LanguageCategoryDatafileParserJson.h" "LanguageCategoryDatafileParserJson.cpp" "LanguageData.h" "LanguageDatafileParserJson.h" "LanguageDatafileParserJson.cpp" "DatafileParserXml.h" "DatafileParserXml.cpp" "DatafileParserJson.h" "DatafileParserJson.cpp" "LanguageDialectData.h" "RealmType.h" "ResistanceType.h" "SkillActionType.h" "SkillGroupType.h" "SkillProgressionType.h" "SpellListType.h" "SkillDevelopmentType.h" "StatType.h" "StatLossType.h" "SkillProgressionData.h" "SkillProgressionData.cpp" "SkillProgressionType.cpp" "CreatureSizeType.cpp" "StringUtils.cpp" "CriticalTableType.cpp" "CriticalType.cpp" "RealmType.cpp" "ResistanceType.cpp" "SpellListType.cpp" "StatLossType.cpp" "SkillActionType.cpp" "SkillDevelopmentType.cpp" "SkillGroupType.cpp" "StatType.cpp")

# This needs to be below the add_executable line
target_compile_features(dataload PRIVATE cxx_std_20)

target_include_directories(dataload PUBLIC
                           "${PROJECT_BINARY_DIR}"
                           "${PROJECT_SOURCE_DIR}/../Libraries/boost_1_90_0"
                           )