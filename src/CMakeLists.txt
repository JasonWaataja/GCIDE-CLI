set (SOURCES main.cc)
add_executable (gcide-cli ${SOURCES})
install (TARGETS gcide-cli DESTINATION bin)
