add_library(archid STATIC "StringUtils.cpp" "Dice.cpp")
target_compile_features(archid PUBLIC cxx_std_20)
target_include_directories(archid PUBLIC "${PROJECT_SOURCE_DIR}/include/archid")

target_link_libraries(archid PRIVATE
    spdlog::spdlog_header_only
)

# Enable debug logging in non-Release builds
if(CMAKE_BUILD_TYPE STREQUAL "Release")
    target_compile_definitions(archid PRIVATE SPDLOG_ACTIVE_LEVEL=SPDLOG_LEVEL_INFO)
else()
    target_compile_definitions(archid PRIVATE SPDLOG_ACTIVE_LEVEL=SPDLOG_LEVEL_TRACE)
endif()


