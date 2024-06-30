
cmake_minimum_required(VERSION 3.12)
project(enet-starter)

# Add vendor/enet as a submodule
add_subdirectory(vendor/enet)

# Compile server.cpp as a separate binary
add_executable(server src/server.cpp)
include_directories(server vendor/enet/include)
target_link_libraries(server enet)

# Compile client.cpp as a separate binary
add_executable(client src/client.cpp)
include_directories(client vendor/enet/include)
target_link_libraries(client enet)

