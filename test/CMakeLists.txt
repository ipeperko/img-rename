set(BINARY ${CMAKE_PROJECT_NAME}_test)

find_package (Boost REQUIRED COMPONENTS filesystem unit_test_framework program_options)
include_directories (${Boost_INCLUDE_DIRS} ../)

set(TEST_SOURCES
    ../FileItem.cpp
    ../Formatter.cpp
    ../ImageNaming.cpp
    ../Log.cpp)

add_executable(${BINARY} test.cpp ${TEST_SOURCES})
target_link_libraries(${BINARY}
    dbm
    ${Boost_FILESYSTEM_LIBRARY}
    ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
    ${Boost_PROGRAM_OPTIONS_LIBRARY}
    libMagick++-7.Q16HDRI.so
    )
add_test(NAME ${BINARY} COMMAND ${BINARY})