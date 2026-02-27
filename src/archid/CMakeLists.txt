add_library(archid STATIC "StringUtils.cpp" "Dice.cpp")
target_compile_features(archid PUBLIC cxx_std_20)
target_include_directories(archid PUBLIC "${PROJECT_SOURCE_DIR}/include/archid")
