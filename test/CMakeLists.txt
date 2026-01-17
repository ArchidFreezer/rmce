include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/52eb8108c5bdec04579160ae17225d66034bd723.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_executable(dataload_test "SkillProgressionTypeData_test.cpp" "SkillCategoryData_test.cpp" "Dice_test.cpp" "NumberRange_test.cpp" "StringUtils_test.cpp" "RuleDataEnums_test.cpp" "TableRow_test.cpp" "TableColumnCreatureSizeMatcher_test.cpp" "LookupTable_test.cpp" "BoundIntRowLookupTable_test.cpp" "AttackTable_test.cpp")
target_compile_features(dataload_test PRIVATE cxx_std_20)
target_include_directories(dataload_test PUBLIC "${PROJECT_SOURCE_DIR}/src")
target_link_libraries(
  dataload_test
  archidutils ruledata dataparsers
  GTest::gtest_main
)


include(GoogleTest)
gtest_discover_tests(dataload_test)
