cmake_minimum_required(VERSION 3.25 FATAL_ERROR)
project(VosgiEngine)

# Set C++ standard
set(CMAKE_CXX_STANDARD 23)

# Set build directory
set(CMAKE_BINARY_DIR "${CMAKE_CURRENT_SOURCE_DIR}/Build")

# Set path to additional CMake modules (optional)
# list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/Core/Public)

# Collect all source files from Core/Private
file(GLOB PRIVATE_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/Core/Private/*.cpp")
file(GLOB IMGUI_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/include/imgui/*.cpp")

# Add Assets folder
file(COPY Assets DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

# Include additional files
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

# Add OpenGL library
find_package(OpenGL REQUIRED)
if (NOT OPENGL_FOUND)
    message(FATAL_ERROR "OpenGL not found!")
endif ()

# Add GLEW library
find_package(GLEW REQUIRED)
if (NOT GLEW_FOUND)
    message(FATAL_ERROR "GLEW not found!")
endif ()

# Add GLFW library
find_package(GLFW3 REQUIRED)
if (NOT GLFW3_FOUND)
    message(FATAL_ERROR "GLFW not found!")
endif ()

# Add GLM library
find_package(GLM REQUIRED)
if (NOT GLM_FOUND)
    message(FATAL_ERROR "GLM not found!")
endif ()

# Add Assimp library
find_package(ASSIMP REQUIRED)
if (NOT ASSIMP_FOUND)
    message(FATAL_ERROR "ASSIMP not found!")
endif ()

# Include directories for GLFW, GLEW, and GLM
include_directories(${GLEW_INCLUDE_DIRS} ${GLFW3_INCLUDE_DIRS} ${GLM_INCLUDE_DIRS})

# List your source files
set(SOURCES
        main.cpp
        ${PRIVATE_SOURCES}
        ${IMGUI_SOURCES}
        )

# Create the executable
add_executable(${PROJECT_NAME} ${SOURCES})

# Link the libraries
target_link_libraries(${PROJECT_NAME}
        glfw
        glm::glm
        GLEW::GLEW
        ${OPENGL_LIBRARIES}
        ${ASSIMP_LIBRARIES}
        )

# On Windows, copy GLFW and GLEW DLLs to the output directory
if (WIN32)
    # Copy GLFW DLL
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${GLFW3_DLL}"
            $<TARGET_FILE_DIR:${PROJECT_NAME}>
            )

    # Copy GLEW DLL
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${GLEW_DLL}"
            $<TARGET_FILE_DIR:${PROJECT_NAME}>
            )
endif (WIN32)

# On macOS, link against Apple frameworks
if (APPLE)
    # Set the minimum macOS version (adjust if needed)
    set(CMAKE_OSX_DEPLOYMENT_TARGET "10.12" CACHE STRING "Minimum macOS version")

    # Link against Apple frameworks
    target_link_libraries(${PROJECT_NAME}
            "-framework OpenGL"
            "-framework Cocoa"
            "-framework IOKit"
            "-framework CoreVideo"
            )
endif (APPLE)

# On Linux, link against X11
if (UNIX AND NOT APPLE)
    target_link_libraries(${PROJECT_NAME} ${X11_LIBRARIES})
endif (UNIX AND NOT APPLE)