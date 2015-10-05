set( FILES
    cfg1.xml
    cfg2.xml
    cfg3.xml
    cfg4.xml
    tm1.xml
    tm2.xml
    tm3.xml
    tm4.xml
    tm5.xml
    utm.xml
    )

# Set up the include directory and symlink/copy all headers to it.
# Note: Windows lacks symlinks and so the files must be copied.
if(UNIX)
  set(header_cmd "create_symlink")
else()
  set(header_cmd "copy_if_different")
endif()
foreach(xmlFile ${FILES})
  set(from "${CMAKE_CURRENT_SOURCE_DIR}/${xmlFile}")
  set(to "${CMAKE_BINARY_DIR}/data/${xmlFile}")
  execute_process(COMMAND ${CMAKE_COMMAND} -E ${header_cmd} "${from}" "${to}")
endforeach()
