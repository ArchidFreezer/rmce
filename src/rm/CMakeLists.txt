add_library(rm STATIC)
add_subdirectory("game")
add_subdirectory("rule")
add_subdirectory("rule/parser")
add_subdirectory("rule/table")
target_compile_features(rm PUBLIC cxx_std_20)
target_include_directories(rm PUBLIC "${PROJECT_SOURCE_DIR}/../Libraries/boost_1_90_0"
                                     "${PROJECT_SOURCE_DIR}/include/archidutils"
                                     "${PROJECT_SOURCE_DIR}/include/rm"
                                     "${PROJECT_SOURCE_DIR}/include/rm/game"
                                     "${PROJECT_SOURCE_DIR}/include/rm/rule"
                                     "${PROJECT_SOURCE_DIR}/include/rm/rule/parser"
                                     "${PROJECT_SOURCE_DIR}/include/rm/rule/table")
