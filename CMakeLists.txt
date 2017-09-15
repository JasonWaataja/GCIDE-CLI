cmake_minimum_required (VERSION 2.9)

project (gcide-cli)

add_subdirectory (src)
add_subdirectory (man)

install (DIRECTORY gcide_xml-0.51 DESTINATION share/gcide-cli)
