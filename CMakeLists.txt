cmake_minimum_required(VERSION 3.10)
project(img_rename)

set(CMAKE_CXX_STANDARD 17)

find_package (Boost REQUIRED COMPONENTS filesystem program_options)
include_directories(/usr/local/include/ImageMagick-7/)

file(GLOB SOURCES
    *.cpp
    )

add_executable(${PROJECT_NAME} ${SOURCES})
target_link_libraries(${PROJECT_NAME}
    ${Boost_PROGRAM_OPTIONS_LIBRARY}
    ${Boost_FILESYSTEM_LIBRARY}
    libMagick++-7.Q16HDRI.so
    )
install(TARGETS ${PROJECT_NAME} DESTINATION bin)

add_subdirectory(test)

