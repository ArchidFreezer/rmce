add_executable (dataload "dataload.cpp")
target_compile_features(dataload PRIVATE cxx_std_20)
target_link_libraries(dataload PUBLIC archid rm)
target_include_directories(dataload PUBLIC "${PROJECT_SOURCE_DIR}/include/archid"
                                           "${PROJECT_SOURCE_DIR}/include/rm")
