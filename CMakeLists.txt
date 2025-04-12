cmake_minimum_required(VERSION 3.15)
project(ifcg-engine)

# CMake settings
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
if (UNIX)
    add_custom_target(
        copy-compile-commands ALL
        ${CMAKE_COMMAND} -E copy_if_different
        ${CMAKE_BINARY_DIR}/compile_commands.json
        ${CMAKE_SOURCE_DIR}
    )
endif (UNIX)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${FLAGS}")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} ${FLAGS} -g3 -fsanitize=address,undefined")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} ${FLAGS} -O3")

set(FLAGS "-Wall")
if (CMAKE_COMPILER_IS_GNUXX)
    set(FLAGS "${FLAGS} -Wextra")
endif (CMAKE_COMPILER_IS_GNUXX)


# Flags
option(CMAKE_BUILD_TYPE "Build type" Release)
option(DEBUG "Enable debug mode" ON)
option(BUILD_TESTS "Build tests" ON)
if (DEBUG)
    message("DEBUG MODE ON")
    add_definitions(-DDEBUG)
else()
    message("DEBUG MODE OFF")
endif()


# Find packages
set(OpenGL_GL_PREFERENCE GLVND)
find_package(OpenGL REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(glm REQUIRED)
find_package(Doxygen OPTIONAL_COMPONENTS dot)

# =-=-=-=-=-=-= DOCS =-=-=-=-=-=-=
if (DOXYGEN_FOUND)
    message("Doxygen found. You can use `make docs`")

    add_custom_target(
        docs
        COMMAND ${DOXYGEN_EXECUTABLE}
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        VERBATIM
    )
else (DOXYGEN_FOUND)
    message("Doxygen not found. You won't be able to use `make docs`")
endif (DOXYGEN_FOUND)


# =-=-=-=-=-=-= LIBRARY =-=-=-=-=-=-=
if (UNIX)
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
endif (UNIX)

# Source files
set(SOURCE_DIR ${CMAKE_SOURCE_DIR}/src)
set(
    SOURCE_FILES
    ${SOURCE_DIR}/glad/glad.c
)

# Adding ifcg shared library
add_library(ifcg SHARED ${SOURCE_FILES})
target_compile_definitions(ifcg PUBLIC GLAD_GLAPI_EXPORT PRIVATE GLAD_GLAPI_EXPORT_BUILD)

# Libraries
target_link_libraries(
    ifcg
    PUBLIC
    ${OPENGL_LIBRARIES} glfw
)
if (UNIX)
    target_link_libraries(
        ifcg
        PUBLIC
        stdc++
    )
endif (UNIX)

# Header files
target_include_directories(
    ifcg
    PUBLIC
    ${CMAKE_SOURCE_DIR}/include
)


# =-=-=-=-=-=-= TESTS =-=-=-=-=-=-=
if (NOT BUILD_TESTS)
    return()
endif ()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# ------ MAIN ------

# Source files
set(
    MAIN_TEST_SOURCE_FILES
    ${CMAKE_SOURCE_DIR}/tests/main.cpp
    ${CMAKE_SOURCE_DIR}/tests/texture.cpp
)

# Adding executables for testing
add_executable(main-test ${MAIN_TEST_SOURCE_FILES})
add_dependencies(main-test ifcg)
target_compile_definitions(main-test PUBLIC GLAD_GLAPI_EXPORT)
target_compile_definitions(main-test PUBLIC PROJECT_ROOT_FOLDER="${CMAKE_SOURCE_DIR}")

# Libraries
if (UNIX)
    target_link_directories(
        main-test
        PUBLIC
        ./lib
    )
endif (UNIX)
target_link_libraries(
    main-test
    PUBLIC
    ifcg
    ${OPENGL_LIBRARIES} glfw
)
if (UNIX)
    target_link_libraries(
        main-test
        PUBLIC
        stdc++
    )
endif (UNIX)

# Header files
target_include_directories(
    main-test
    PUBLIC
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/tests 
)

if (WIN32 OR MSVC)
    add_custom_command(TARGET main-test POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
        $<TARGET_RUNTIME_DLLS:main-test>
        $<TARGET_FILE_DIR:main-test>
        COMMAND_EXPAND_LISTS
    )
endif ()
