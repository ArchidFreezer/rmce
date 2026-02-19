add_library(ruledata STATIC "CreatureSizeType.cpp" "CriticalTableType.cpp" "CriticalType.cpp" "RealmType.cpp" "ResistanceType.cpp" "SkillActionType.cpp" "SkillDevelopmentType.cpp"    "SpellListType.cpp" "StatLossType.cpp" "StatType.cpp" "SkillCategoryData.cpp" "SkillProgressionTypeData.cpp" "GameRuleData.cpp" "ArmourType.cpp" "AttackSizeType.cpp"   "SkillData.cpp" "ProfessionData.cpp" "SpellUserType.cpp" "HabitatType.cpp" "EnvironmentType.cpp" "TrainingPackageData.cpp" "AnimalOutlookType.cpp" "CreatureMovementSpeedType.cpp" "ManoeuvreDifficultyType.cpp" "AnimalData.cpp" "CreatureBonusXpType.cpp" "CreatureConstitutionVarianceType.cpp" "CreatureLevelVarianceType.cpp" "TreasureValueType.cpp" "CriticalSizeTableType.cpp" "CriticalModifierType.cpp")
target_compile_features(ruledata PUBLIC cxx_std_20)
target_include_directories(ruledata PUBLIC "${PROJECT_SOURCE_DIR}/include/archidutils"
                                           "${PROJECT_SOURCE_DIR}/include/ruledata"
                                           "${PROJECT_SOURCE_DIR}/include/gameobject")



