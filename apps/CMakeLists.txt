add_executable (dataload "dataload.cpp")
target_compile_features(dataload PRIVATE cxx_std_20)
target_link_libraries(dataload PUBLIC archid rm spdlog::spdlog)
target_include_directories(dataload PUBLIC "${PROJECT_SOURCE_DIR}/include/archid"
                                           "${PROJECT_SOURCE_DIR}/include/rm")

# Disable verbose logging in Release builds
if(CMAKE_BUILD_TYPE STREQUAL "Release")
    target_compile_definitions(dataload PRIVATE SPDLOG_ACTIVE_LEVEL=SPDLOG_LEVEL_INFO)
else()
    target_compile_definitions(dataload PRIVATE SPDLOG_ACTIVE_LEVEL=SPDLOG_LEVEL_TRACE)
endif()


# Rest server
find_package(boost_system REQUIRED CONFIG)
find_package(boost_thread REQUIRED CONFIG)
add_executable (rest_server_example "rest_server_example.cpp")
target_link_libraries(rest_server_example
    PRIVATE
        rmce_rest
        Boost::system
        Boost::thread
        spdlog::spdlog
)

target_include_directories(rest_server_example PUBLIC "${PROJECT_SOURCE_DIR}/include/archid"
                                           "${PROJECT_SOURCE_DIR}/include/rm")

target_compile_features(rest_server_example PRIVATE cxx_std_20)

# Disable verbose logging in Release builds
if(CMAKE_BUILD_TYPE STREQUAL "Release")
    target_compile_definitions(rest_server_example PRIVATE SPDLOG_ACTIVE_LEVEL=SPDLOG_LEVEL_INFO)
else()
    target_compile_definitions(rest_server_example PRIVATE SPDLOG_ACTIVE_LEVEL=SPDLOG_LEVEL_TRACE)
endif()

# Install the example
install(TARGETS rest_server_example
    RUNTIME DESTINATION bin
)

