set(SRCS
    TMState.cpp
    TMTransition.cpp
    TuringMachine.cpp

    TMSimulatedState.cpp
    TMTape.cpp
    TuringMachineSimulator.cpp
    TuringMachineParser.cpp
    TuringMachineEncodingConverter.cpp

    )

set(HDRS
    TuringMachine.h

    TMSimulatedState.h
    TMTape.h
    TMState.h
    TMTransition.h
    TuringMachineSimulator.h
    TuringMachineParser.h
    TuringMachineEncodingConverter.h
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

add_library(tmlib SHARED ${SRCS})

target_link_libraries(tmlib xmllib)
