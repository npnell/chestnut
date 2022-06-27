cmake_minimum_required(VERSION 3.10)

project(chestnut)

add_subdirectory(extern)

set(SOURCES
    "${PROJECT_SOURCE_DIR}/src/main.cpp"
    "${PROJECT_SOURCE_DIR}/src/include/chip8.h"
    "${PROJECT_SOURCE_DIR}/src/include/renderer.h"
    "${PROJECT_SOURCE_DIR}/src/include/shader.h"
)

set(LIBS glfw3 glad)

add_executable(${PROJECT_NAME} ${SOURCES})

target_link_libraries(${PROJECT_NAME} ${LIBS})

target_include_directories(${PROJECT_NAME}
    PUBLIC "${PROJECT_SOURCE_DIR}/src/include"
    PUBLIC "${PROJECT_SOURCE_DIR}/extern/include"
)

target_link_directories(${PROJECT_NAME}
    PUBLIC "${PROJECT_SOURCE_DIR}/extern/lib"
)