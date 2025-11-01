cmake_minimum_required(VERSION 4.1.2)
set(CMAKE_EXPERIMENTAL_CXX_IMPORT_STD "d0edc3af-4c50-42ea-a356-e2862fe7a444")
project(SerEx VERSION 0.1.0 LANGUAGES CXX)

# C++ Standard + Modules settings.
set(CMAKE_CXX_STANDARD 26)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_MODULE_STD ON)

# Collect source files.
file(GLOB_RECURSE MODULE_SOURCES CONFIGURE_DEPENDS src/*.cpp)
add_library(${PROJECT_NAME} STATIC ${MODULE_SOURCES})

target_sources(
        ${PROJECT_NAME} PUBLIC FILE_SET CXX_MODULES FILES ${MODULE_SOURCES})

target_compile_options(
        ${PROJECT_NAME} PRIVATE -fmodules-ts)

target_compile_features(
        ${PROJECT_NAME} PUBLIC cxx_std_26)


# Install the library + export information
install(
        TARGETS SerEx
        EXPORT SerExTargets
        FILE_SET CXX_MODULES DESTINATION lib/cmake/SerEx/modules)

install(
        EXPORT SerExTargets
        NAMESPACE SerEx::
        DESTINATION lib/cmake/SerEx)

include(
        CMakePackageConfigHelpers)

write_basic_package_version_file(
        ${CMAKE_CURRENT_BINARY_DIR}/SerExConfigVersion.cmake
        VERSION ${PROJECT_VERSION}
        COMPATIBILITY SameMajorVersion)

configure_package_config_file(
        ${CMAKE_CURRENT_SOURCE_DIR}/cmake/SerExConfig.cmake.in
        ${CMAKE_CURRENT_BINARY_DIR}/SerExConfig.cmake
        INSTALL_DESTINATION lib/cmake/SerEx)

install(
        FILES
        ${CMAKE_CURRENT_BINARY_DIR}/SerExConfig.cmake
        ${CMAKE_CURRENT_BINARY_DIR}/SerExConfigVersion.cmake
        DESTINATION lib/cmake/SerEx)