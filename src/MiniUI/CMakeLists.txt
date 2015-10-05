set(SRCS
    MiniUI.cpp
    )

set(HDRS
    MiniUI.h
    )

if(UNIX)
  set(header_cmd "create_symlink")
else()
  set(header_cmd "copy_if_different")
endif()
foreach(headerFile ${HDRS})
  set(from "${CMAKE_CURRENT_SOURCE_DIR}/${headerFile}")
  set(to "${CMAKE_BINARY_DIR}/include/UTM/${headerFile}")
  execute_process(COMMAND ${CMAKE_COMMAND} -E ${header_cmd} "${from}" "${to}")
endforeach()

add_library(uilib SHARED ${SRCS})
target_link_libraries(uilib cfglib pdalib tmlib uilib)
