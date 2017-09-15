find_package (PkgConfig REQUIRED)

set (SOURCES
	main.cc
	dictionary_entry.cc
	dictionary_reader.cc
	options.cc
	word_gathering.cc
	util.cc
	xml.cc
	)
add_executable (gcide-cli ${SOURCES})

pkg_check_modules (LIBXMLPP REQUIRED libxml++-3.0)
include_directories (${LIBXMLPP_INCLUDE_DIRS})
# TODO: Find out if this is necessary.
link_directories (${LIBXMLPP_LIBRARY_DIRS})
# TODO: Find out if this is necessary.
add_definitions (${LIBXMLPP_CFLAGS_OTHER})
target_link_libraries (gcide-cli ${LIBXMLPP_LIBRARIES})

# Define variables for code to see.
set (GCIDE_CLI_VERSION_STRING "0.1.0")
set (GCIDE_CLI_CMAKE_INSTALL_PREFIX ${CMAKE_INSTALL_PREFIX})
configure_file (config.h.in config.h @ONLY)
include_directories (${CMAKE_CURRENT_BINARY_DIR})

install (TARGETS gcide-cli DESTINATION bin)
