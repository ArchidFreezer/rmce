find_package(boost_json REQUIRED CONFIG)
add_library(rm STATIC)
target_sources(rm PRIVATE "AppConfig.cpp" "PersistentObjectManager.cpp" "PersistentObjectSerializationManager.cpp")
add_subdirectory("game")
add_subdirectory("rule")
add_subdirectory("serial")
add_subdirectory("rule/table")
target_compile_features(rm PUBLIC cxx_std_23)
target_include_directories(rm PUBLIC ${Boost_INCLUDE_DIRS}
                                     "${PROJECT_SOURCE_DIR}/include/archid"
                                     "${PROJECT_SOURCE_DIR}/include/rm"
                                     "${PROJECT_SOURCE_DIR}/include/rm/game"
                                     "${PROJECT_SOURCE_DIR}/include/rm/rule"
                                     "${PROJECT_SOURCE_DIR}/include/rm/rule/table"
                                     "${PROJECT_SOURCE_DIR}/include/rm/serial")

target_link_libraries(rm PUBLIC 
    Boost::boost  # Header-only Boost libraries
    Boost::json
    spdlog::spdlog_header_only
)

# Enable debug logging in non-Release builds
if(CMAKE_BUILD_TYPE STREQUAL "Release")
    target_compile_definitions(rm PRIVATE SPDLOG_ACTIVE_LEVEL=SPDLOG_LEVEL_INFO)
else()
    target_compile_definitions(rm PRIVATE SPDLOG_ACTIVE_LEVEL=SPDLOG_LEVEL_TRACE)
endif()

# REST API Library
find_package(boost_beast REQUIRED CONFIG)
find_package(boost_system REQUIRED CONFIG)
find_package(boost_thread REQUIRED CONFIG)
find_package(boost_url REQUIRED CONFIG)

add_library(rmce_rest)

add_subdirectory("rest")

target_include_directories(rmce_rest
    PUBLIC
        ${Boost_INCLUDE_DIRS}
        "${PROJECT_SOURCE_DIR}/include/archid"
        "${PROJECT_SOURCE_DIR}/include/rm"
        "${PROJECT_SOURCE_DIR}/include/rm/game"
        "${PROJECT_SOURCE_DIR}/include/rm/rest"
        "${PROJECT_SOURCE_DIR}/include/rm/rule"
        "${PROJECT_SOURCE_DIR}/include/rm/rule/table"
        "${PROJECT_SOURCE_DIR}/include/rm/serial")

target_link_libraries(rmce_rest PRIVATE
    archid
    rm
    Boost::beast
    Boost::system
    Boost::thread
    Boost::url
    spdlog::spdlog_header_only
)

# Enable debug logging in non-Release builds
if(CMAKE_BUILD_TYPE STREQUAL "Release")
    target_compile_definitions(rmce_rest PRIVATE SPDLOG_ACTIVE_LEVEL=SPDLOG_LEVEL_INFO)
else()
    target_compile_definitions(rmce_rest PRIVATE SPDLOG_ACTIVE_LEVEL=SPDLOG_LEVEL_TRACE)
endif()

target_compile_features(rmce_rest PUBLIC cxx_std_23)