add_library(rm STATIC)
target_sources(rm PRIVATE "PersistentObjectManager.cpp" "rule/parser/GameRuleBoostConverter.cpp")
add_subdirectory("game")
add_subdirectory("rule")
add_subdirectory("rule/parser")
add_subdirectory("rule/table")
target_compile_features(rm PUBLIC cxx_std_20)
target_include_directories(rm PUBLIC ${Boost_INCLUDE_DIRS}
                                     "${PROJECT_SOURCE_DIR}/include/archid"
                                     "${PROJECT_SOURCE_DIR}/include/rm"
                                     "${PROJECT_SOURCE_DIR}/include/rm/game"
                                     "${PROJECT_SOURCE_DIR}/include/rm/rule"
                                     "${PROJECT_SOURCE_DIR}/include/rm/rule/parser"
                                     "${PROJECT_SOURCE_DIR}/include/rm/rule/table")

target_link_libraries(rm PUBLIC 
    Boost::boost  # Header-only Boost libraries
)