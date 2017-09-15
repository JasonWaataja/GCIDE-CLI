find_package (PkgConfig REQUIRED)

set (SOURCES
	main.cc
	dictionary_entry.cc
	dictionary_reader.cc
	options.cc
	word_gathering.cc
	util.cc
	)
add_executable (gcide-cli ${SOURCES})

add_custom_command (TARGET gcide-cli POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_if_different
	${PROJECT_SOURCE_DIR}/gcide_xml-0.51
	${PROJECT_BINARY_DIR}/gcide_xml-0.51)

pkg_check_modules (LIBXMLPP REQUIRED libxml++-3.0)
include_directories (${LIBXMLPP_INCLUDE_DIRS})
# TODO: Find out if this is necessary.
link_directories (${LIBXMLPP_LIBRARY_DIRS})
# TODO: Find out if this is necessary.
add_definitions (${LIBXMLPP_CFLAGS_OTHER})
target_link_libraries (gcide-cli ${LIBXMLPP_LIBRARIES})

# find_package (Boost 1.60 COMPONENTS program_options REQUIRED)
# include_directories (${Boost_INCLUDE_DIRS})
# target_link_libraries (gcide-cli ${BOOST_LIBRARIES})

install (TARGETS gcide-cli DESTINATION bin)
