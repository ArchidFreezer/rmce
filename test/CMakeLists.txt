include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/52eb8108c5bdec04579160ae17225d66034bd723.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_executable(
  dataload_test
 "SkillProgressionData_test.cpp"
 "../src/SkillProgressionData.h" "../src/SkillProgressionData.cpp" "../src/SkillProgressionType.h" "../src/SkillProgressionType.cpp" "../src/StringUtils.h" "../src/StringUtils.cpp")

# This needs to be below the add_executable line
target_compile_features(dataload_test PRIVATE cxx_std_20)

target_link_libraries(
  dataload_test
  GTest::gtest_main
)

target_include_directories(dataload_test PUBLIC "${PROJECT_SOURCE_DIR}/src")

include(GoogleTest)
gtest_discover_tests(dataload_test)
