cmake_minimum_required( VERSION 3.0.0 )

set( This 
    mpc_unit_tests
)

set( Sources
    mpc_unit_tests
    ../fake/LiquidCrystalFast
    ../fake/Serial
    ../fake/Arduino
    ../HardwareSerial
    ../LcdBuffer
    ../MidiMessage
    ../MidiMessageFIFOBuffer
    ../MidiMessageFilter
    ../MidiMessageTranslation
    ../MidiMessageTranslationMap
    ../tinyxml2
)

add_executable( ${This} ${Sources} )

target_link_libraries( 
    ${This} PUBLIC
    gtest_main
    gmock_main
)

get_filename_component( PARENT_DIR ${CMAKE_CURRENT_SOURCE_DIR} DIRECTORY )

target_include_directories( 
    ${This} PUBLIC
    ${PARENT_DIR}
    ${PARENT_DIR}/fake
)

add_test(
    NAME ${This}
    COMMAND ${This}
)
