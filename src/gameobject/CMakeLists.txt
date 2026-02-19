add_library(gameobject STATIC "LanguageAbility.cpp" "LocationMatcher.cpp" "Location.cpp")
target_compile_features(gameobject PUBLIC cxx_std_20)
target_include_directories(gameobject PUBLIC "${PROJECT_SOURCE_DIR}/include/archidutils"
                                              "${PROJECT_SOURCE_DIR}/include/ruledata"
                                              "${PROJECT_SOURCE_DIR}/include/gameobject")