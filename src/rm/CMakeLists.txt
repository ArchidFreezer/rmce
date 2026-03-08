find_package(boost_json REQUIRED CONFIG)
add_library(rm STATIC)
target_sources(rm PRIVATE "PersistentObjectManager.cpp" "PersistentObjectJsonManager.cpp")
add_subdirectory("game")
add_subdirectory("rule")
add_subdirectory("rule/serial")
add_subdirectory("rule/table")
target_compile_features(rm PUBLIC cxx_std_20)
target_include_directories(rm PUBLIC ${Boost_INCLUDE_DIRS}
                                     "${PROJECT_SOURCE_DIR}/include/archid"
                                     "${PROJECT_SOURCE_DIR}/include/rm"
                                     "${PROJECT_SOURCE_DIR}/include/rm/game"
                                     "${PROJECT_SOURCE_DIR}/include/rm/rule"
                                     "${PROJECT_SOURCE_DIR}/include/rm/rule/serial"
                                     "${PROJECT_SOURCE_DIR}/include/rm/rule/table")

target_link_libraries(rm PUBLIC 
    Boost::boost  # Header-only Boost libraries
    Boost::json
)

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
        "${PROJECT_SOURCE_DIR}/include/rm/rule/table")

target_link_libraries(rmce_rest PRIVATE
    Boost::beast
    Boost::system
    Boost::thread
    Boost::url
)

target_compile_features(rmce_rest PUBLIC cxx_std_20)