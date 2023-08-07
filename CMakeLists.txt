cmake_minimum_required(VERSION 3.12)

project(Astra)

set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)
set(CMAKE_BUILD_TYPE "Debug")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/bin)
set(CMAKE_CURRENT_BINARY_DIR ${CMAKE_CURRENT_SOURCE_DIR}/build)
set(SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)
set(LIBRARY_DIR ${CMAKE_CURRENT_SOURCE_DIR}/lib)
set(VENDOR_DIR ${CMAKE_CURRENT_SOURCE_DIR}/vendor)

if (UNIX)
    find_package(OpenGL REQUIRED)
    find_package(X11 REQUIRED)
endif (UNIX)

if(WIN32)
    add_definitions(-DGLEW_STATIC)
endif (WIN32)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/lib/glfw-3.3.8)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/lib/glew-2.2.0/build/cmake)

include_directories(
    ${INCLUDE_DIR} 
    ${VENDOR_DIR}
    ${VENDOR_DIR}/imgui-1.89.8/
    ${VENDOR_DIR}/imgui-1.89.8/backends
    ${LIBRARY_DIR}/glew-2.2.0/include
)

file(GLOB SOURCE_FILES ${SOURCE_DIR}/*.cpp)

file(GLOB IMGUI_SOURCE_FILES 
    ${VENDOR_DIR}/imgui-1.89.8/*.cpp 
    ${VENDOR_DIR}/imgui-1.89.8/backends/imgui_impl_glfw.cpp
    ${VENDOR_DIR}/imgui-1.89.8/backends/imgui_impl_opengl3.cpp
)
list(APPEND SOURCE_FILES ${IMGUI_SOURCE_FILES})

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

if(UNIX)
    add_executable(${PROJECT_NAME} ${SOURCE_FILES})
    include_directories( ${OPENGL_INCLUDE_DIRS} )
    target_link_libraries(${PROJECT_NAME} OpenGL::GL X11 glfw glew)
endif (UNIX)

if(WIN32)
    add_executable(${PROJECT_NAME} WIN32 ${SOURCE_FILES}) # WIN32 to prevent terminal
    target_link_libraries(${PROJECT_NAME} glfw glew_s)
endif (WIN32)