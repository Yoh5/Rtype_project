cmake_minimum_required(VERSION 3.22)
project(R_type)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (NOT WIN32)
    add_compile_options(-std=c++20 -Wall -Wextra -g3)
endif()

set(SOURCES_CLI
    ./Client/Main.cpp
)

set (SERVER_CLI
    ./Server/src/main.cpp
)

set (ADMIN_CLI
    ./admin_cli/ServerAdmin.cpp
)

set(CPACK_GENERATOR "ZIP")
set(CPACK_SOURCE_GENERATOR "ZIP")
set(CPACK_PACKAGE_DIRECTORY ${PROJECT_BINARY_DIR}/package)
set(CPACK_SOURCE_IGNORE_FILES .git/ .github/ .vscode/ .mypy_cache/
${CMAKE_BINARY_DIR}/ ${PROJECT_BINARY_DIR})
include(CPack)

include(build/conan_toolchain.cmake)
find_package(Boost REQUIRED COMPONENTS system filesystem date_time serialization)
find_package(Asio REQUIRED)
find_package(SFML 2.5.1 COMPONENTS graphics audio system REQUIRED)

add_executable(r-type_server ${SERVER_CLI})
add_executable(r-type_client ${SOURCES_CLI})
add_executable(r-type_admin_cli ${ADMIN_CLI})

if (NOT WIN32)
    set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR})
    set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})
    set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR})
endif()

target_link_libraries(r-type_server asio::asio sfml-graphics sfml-audio sfml-system sfml-network Boost::system)
target_link_libraries(r-type_client sfml-graphics sfml-audio sfml-window sfml-system asio::asio Boost::system)
target_link_libraries(r-type_admin_cli sfml-graphics sfml-audio sfml-window sfml-system asio::asio Boost::system)

if (WIN32)
    add_custom_command(TARGET r-type_admin_cli POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:r-type_admin_cli>
    ${CMAKE_BINARY_DIR}/../
)

    add_custom_command(TARGET r-type_client POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:r-type_client>
    ${CMAKE_BINARY_DIR}/../
)

    add_custom_command(TARGET r-type_server POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:r-type_server>
    ${CMAKE_BINARY_DIR}/../
    )
endif()