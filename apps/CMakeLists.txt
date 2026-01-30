add_executable (dataload "dataload.cpp")
target_compile_features(dataload PRIVATE cxx_std_20)
target_link_libraries(dataload PUBLIC archidutils ruledata dataparsers gameobject)
target_include_directories(dataload PUBLIC "${PROJECT_SOURCE_DIR}/include/archidutils"
                                           "${PROJECT_SOURCE_DIR}/include/ruledata"
                                           "${PROJECT_SOURCE_DIR}/include/gameobject"
                                           "${PROJECT_SOURCE_DIR}/include/dataparsers")
