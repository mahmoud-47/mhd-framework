cmake_minimum_required(VERSION 3.16)

project(MyCppWebServer LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB_RECURSE SRC_FILES
    "${CMAKE_SOURCE_DIR}/*.cpp"
)

# Filter out any files from CMakeFiles directory (e.g., CMakeCXXCompilerId.cpp)
list(FILTER SRC_FILES EXCLUDE REGEX ".*CMakeFiles/.*")

# Find SQLite3
find_package(SQLite3 REQUIRED)

if (NOT SQLite3_FOUND)
    message(FATAL_ERROR "SQLite3 not found. Please install it with: sudo apt-get install libsqlite3-dev")
endif()

include_directories(
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/router
    ${CMAKE_SOURCE_DIR}/utils
    ${CMAKE_SOURCE_DIR}/utils/request
    ${CMAKE_SOURCE_DIR}/utils/render
    ${CMAKE_SOURCE_DIR}/controllers
    ${CMAKE_SOURCE_DIR}/database
    ${CMAKE_SOURCE_DIR}/server
    ${SQLite3_INCLUDE_DIRS}
)

add_executable(application ${SRC_FILES})

# Link against SQLite3
target_link_libraries(application ${SQLite3_LIBRARIES})
