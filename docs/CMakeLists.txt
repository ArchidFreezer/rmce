# docs/CMakeLists.txt

# 1. Define the files and directories to scan
# We include 'include' for your headers and the specific .dox file
set(DOXYGEN_INPUT_FILES 
    "${PROJECT_SOURCE_DIR}/include" 
    "${CMAKE_CURRENT_SOURCE_DIR}/namespaces.dox"
)

# 2. Configure Doxygen behavior via CMake variables
set(DOXYGEN_PROJECT_NAME "Rolemaster Character Editor")
set(DOXYGEN_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/docs")
set(DOXYGEN_HAVE_DOT YES)
set(DOXYGEN_QUIET YES)

# 3. Handle the .dox extension
# This tells Doxygen to treat .dox files as C++ code
set(DOXYGEN_EXTENSION_MAPPING "dox=C++")
set(DOXYGEN_FILE_PATTERNS "*.cpp" "*.h" "*.hpp" "*.dox")

# 4. Create the target
# 'ALL' means it builds when you build the solution in Visual Studio
doxygen_add_docs(docs
    ${DOXYGEN_INPUT_FILES}
    ALL
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
    COMMENT "Generating API documentation"
)