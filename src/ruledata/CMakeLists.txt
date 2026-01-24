add_library(ruledata STATIC "CreatureSizeType.cpp" "CriticalTableType.cpp" "CriticalType.cpp" "RealmType.cpp" "ResistanceType.cpp" "SkillActionType.cpp" "SkillDevelopmentType.cpp" "SkillGroupType.cpp"   "SpellListType.cpp" "StatLossType.cpp" "StatType.cpp" "SkillCategoryData.cpp" "SkillProgressionTypeData.cpp" "GameRuleData.cpp" "ArmourType.cpp" "AnimalSizeType.cpp"  "table/TableRowNumberMatcherFactory.cpp" "SkillData.cpp")
target_compile_features(ruledata PUBLIC cxx_std_20)
target_include_directories(ruledata PUBLIC "${PROJECT_SOURCE_DIR}/include/archidutils"
                                           "${PROJECT_SOURCE_DIR}/include/ruledata")



