set(DOXYGEN_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/docs)
set(DOXYGEN_PROJECT_NAME "Rolemaster Character Editor")
set(DOXYGEN_HAVE_DOT YES)
set(DOXYGEN_QUIET YES)
doxygen_add_docs(docs                 
                 ALL
                 ${PROJECT_SOURCE_DIR}/include
                 WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/include)