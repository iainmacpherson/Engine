cmake_minimum_required(VERSION 3.10)

project(Engine)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 11)

# Add your source files
set(SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/include/glad/glad.c
    ${CMAKE_CURRENT_SOURCE_DIR}/include/stbi/stb_image.cpp

    src/main.cpp
    src/shader_program.cpp
    src/camera.cpp
)

# Add your header files
set(HEADERS
    src/shader_program.hpp
    src/camera.hpp
)

# Set the include directories
include_directories(include)

# Create the executable
add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

# Set additional compiler flags if needed
if (CMAKE_BUILD_TYPE STREQUAL "DEBUG")
    target_compile_options(${PROJECT_NAME} PRIVATE /MTd /Zi /Od /W4)
endif()
if(CMAKE_BUILD_TYPE STREQUAL "RELEASE")
    target_compile_options(${PROJECT_NAME} PRIVATE /O2)
endif()

# Set additional linker flags if needed
# find_package()
# target_link_libraries(${PROJECT_NAME} PRIVATE your_library)
target_link_libraries(${PROJECT_NAME} PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/libs/glfw3.lib")

