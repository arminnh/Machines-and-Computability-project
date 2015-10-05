# define DATADIR for tests that need input files
add_definitions(-DDATADIR="${CMAKE_SOURCE_DIR}/data/")

include_directories(${CMAKE_BINARY_DIR}/include)

set(files
    tm1Test.xml
    cfg1Test.xml
    cfg2Test.xml
    expected.dot
    )

if(UNIX)
  set(header_cmd "create_symlink")
else()
  set(header_cmd "copy_if_different")
endif()
foreach(xmlFile ${files})
  set(from "${CMAKE_CURRENT_SOURCE_DIR}/${xmlFile}")
  set(to "${CMAKE_BINARY_DIR}/data/${xmlFile}")
  execute_process(COMMAND ${CMAKE_COMMAND} -E ${header_cmd} "${from}" "${to}")
endforeach()
