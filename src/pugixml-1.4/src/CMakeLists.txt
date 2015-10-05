set( SRCS
    pugixml.cpp
    )

set(HEADERS
    pugixml.hpp
    pugiconfig.hpp
    )

if(UNIX)
  set(header_cmd "create_symlink")
else()
  set(header_cmd "copy_if_different")
endif()
foreach(headerFile ${HEADERS})
  set(from "${CMAKE_CURRENT_SOURCE_DIR}/${headerFile}")
  set(to "${CMAKE_BINARY_DIR}/include/UTM/${headerFile}")
  execute_process(COMMAND ${CMAKE_COMMAND} -E ${header_cmd} "${from}" "${to}")
endforeach()

add_library(xmllib SHARED ${SRCS})

