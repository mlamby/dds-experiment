cmake_minimum_required(VERSION 3.23)
project(dds-testing LANGUAGES C)

# Build all executables and libraries into the top-level /bin and /lib folders.
# This matches how cyclonedds is setup.
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")

# Dependencies managed via vcpkg
find_package(check CONFIG REQUIRED)

add_subdirectory(3rdparty/CycloneDDS)
add_subdirectory(src)
add_subdirectory(test)