add_library(archidutils STATIC "StringUtils.cpp")
target_compile_features(archidutils PUBLIC cxx_std_20)
target_include_directories(archidutils PUBLIC "${PROJECT_SOURCE_DIR}/include/archidutils")
