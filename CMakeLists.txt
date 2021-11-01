cmake_minimum_required(VERSION 3.10)
project(img_rename VERSION 0.2.0)

set(CMAKE_CXX_STANDARD 17)

configure_file(src/img_rename.h.in img_rename.h)

find_package (Boost REQUIRED COMPONENTS filesystem program_options)

include_directories(/usr/local/include/ImageMagick-7/)

file(GLOB SOURCES
    src/main.cpp
    src/FileItem.cpp
    src/FileItem.h
    src/Formatter.cpp
    src/Formatter.h
    src/ImageNaming.cpp
    src/ImageNaming.h
    src/Log.cpp
    src/Log.h
    )

add_executable(${PROJECT_NAME} ${SOURCES})
target_link_libraries(${PROJECT_NAME}
    ${Boost_PROGRAM_OPTIONS_LIBRARY}
    ${Boost_FILESYSTEM_LIBRARY}
    libMagick++-7.Q16HDRI.so
    )
target_include_directories(${PROJECT_NAME} PUBLIC
    "${PROJECT_BINARY_DIR}"
    )
install(TARGETS ${PROJECT_NAME} DESTINATION bin)

add_subdirectory(test)

