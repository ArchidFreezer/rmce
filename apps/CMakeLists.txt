#
# Dataload example
add_executable (dataload "dataload.cpp")
target_compile_features(dataload PRIVATE cxx_std_23)
target_link_libraries(dataload PUBLIC archid rm spdlog::spdlog_header_only)
target_include_directories(dataload PUBLIC "${PROJECT_SOURCE_DIR}/include/archid"
                                           "${PROJECT_SOURCE_DIR}/include/rm")

# Disable verbose logging in Release builds
if(CMAKE_BUILD_TYPE STREQUAL "Release")
    target_compile_definitions(dataload PRIVATE SPDLOG_ACTIVE_LEVEL=SPDLOG_LEVEL_INFO)
else()
    target_compile_definitions(dataload PRIVATE SPDLOG_ACTIVE_LEVEL=SPDLOG_LEVEL_TRACE)
endif()

# Copy config.json to the output directory after every build
add_custom_command(TARGET dataload POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
            "${PROJECT_SOURCE_DIR}/config.json"
            "$<TARGET_FILE_DIR:dataload>/config.json"
    COMMENT "Copying config.json to executable directory"
)


#
# Config example
add_executable (config_example "config_example.cpp")
target_compile_features(config_example PRIVATE cxx_std_23)
target_link_libraries(config_example PUBLIC rm spdlog::spdlog_header_only)
target_include_directories(config_example PUBLIC "${PROJECT_SOURCE_DIR}/include/archid"
                                                  "${PROJECT_SOURCE_DIR}/include/rm")

# Disable verbose logging in Release builds
if(CMAKE_BUILD_TYPE STREQUAL "Release")
    target_compile_definitions(config_example PRIVATE SPDLOG_ACTIVE_LEVEL=SPDLOG_LEVEL_INFO)
else()
    target_compile_definitions(config_example PRIVATE SPDLOG_ACTIVE_LEVEL=SPDLOG_LEVEL_TRACE)
endif()

# Copy config.json to the output directory after every build
add_custom_command(TARGET config_example POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
            "${PROJECT_SOURCE_DIR}/config.json"
            "$<TARGET_FILE_DIR:config_example>/config.json"
    COMMENT "Copying config.json to executable directory"
)

#
# Rest server
find_package(boost_system REQUIRED CONFIG)
find_package(boost_thread REQUIRED CONFIG)
add_executable (rest_server_example "rest_server_example.cpp")
target_link_libraries(rest_server_example
    PRIVATE
        rmce_rest
        Boost::system
        Boost::thread
        spdlog::spdlog_header_only
)

target_include_directories(rest_server_example PUBLIC "${PROJECT_SOURCE_DIR}/include/archid"
                                           "${PROJECT_SOURCE_DIR}/include/rm")

target_compile_features(rest_server_example PRIVATE cxx_std_23)

# Disable verbose logging in Release builds
if(CMAKE_BUILD_TYPE STREQUAL "Release")
    target_compile_definitions(rest_server_example PRIVATE SPDLOG_ACTIVE_LEVEL=SPDLOG_LEVEL_INFO)
else()
    target_compile_definitions(rest_server_example PRIVATE SPDLOG_ACTIVE_LEVEL=SPDLOG_LEVEL_TRACE)
endif()

# Copy config.json to the output directory after every build
add_custom_command(TARGET rest_server_example POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
            "${PROJECT_SOURCE_DIR}/config.json"
            "$<TARGET_FILE_DIR:rest_server_example>/config.json"
    COMMENT "Copying config.json to executable directory"
)

# Install the example
install(TARGETS rest_server_example
    RUNTIME DESTINATION bin
)

