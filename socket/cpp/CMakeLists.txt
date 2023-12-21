cmake_minimum_required(VERSION 3.10)

project(socket_samples) #Project name

set(CMAKE_CXX_STANDARD 17) #setting C++ 17 standard

set (CMAKE_CXX_FLAGS "-lstdc++fs ")


#including GStreamer header files directory
include_directories(
  ./
)

#linking GStreamer library directory
link_directories(
)

#building target executable
add_executable(tcp_client tcp_client.cpp)

#building target executable
add_executable(tcp_server tcp_server.cpp)
