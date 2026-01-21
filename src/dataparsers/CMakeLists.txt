add_library(dataparsers STATIC "BookDatafileParserJson.cpp"  "DatafileParserJson.cpp" "DatafileParserXml.cpp" "LanguageCategoryDatafileParserJson.cpp" "LanguageDatafileParserJson.cpp" "SkillProgressionTypeDatafileParserJson.cpp" "AttackTableDatafileParserXml.cpp" "AttackTableDatafileParserJson.cpp" "SpecialAttackTableDatafileParserJson.cpp" "SpellListDatafileParserXml.cpp" "SpellListDatafileParserJson.cpp" "SkillCategoryDatafileParserXml.cpp" "SkillCategoryDatafileParserJson.cpp")
target_compile_features(dataparsers PUBLIC cxx_std_20)
target_include_directories(dataparsers PUBLIC "${PROJECT_SOURCE_DIR}/../Libraries/boost_1_90_0"
                                              "${PROJECT_SOURCE_DIR}/include/archidutils"
                                              "${PROJECT_SOURCE_DIR}/include/ruledata"
                                              "${PROJECT_SOURCE_DIR}/include/dataparsers")